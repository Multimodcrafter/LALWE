#include "stdint.h"
#define sint int32_t
#include "assembler.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include "logger.h"
#include "constants.h"

Assembler::Assembler() {

}

void Assembler::assemble(std::string code, RAM* mem) {
    std::vector<std::string> lines = splitString(code,'\n');
    varbel_names.clear();
    subroutines.clear();

    sint address = 0;

    //first iteration to get all the information about all subroutines and labels
    for(std::string e : lines) {
        Logger::loggerInst->debug(e);
        std::vector<std::string> words = splitString(e,' ');

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size() * 2 - 1; //function call requires two (OP_Code + Argument) + 2 times #parameters + one (function address) memory slots
        else if(words.size() > 0
                && Constants::ASSEMBLY_INST.find(words.at(0)) == Constants::ASSEMBLY_INST.end()) address += 2; //normal op code requires two memory slots (OP-Code + Argument)

        //current subroutine definition
        bool isFunctionDef = false;
        bool isLabel = false;
        sint paramcount = -1;
        std::string name = "";
        subroutine s;

        for(unsigned int i = 0; i < words.size(); ++i) {
            std::string w = words.at(i);
            if(w == "function") isFunctionDef = true; //curent line is a function def
            else if(isFunctionDef) {
                if(i == 1) { //first word after "function" is equal to the name of the subroutine
                    if(checkIdentifier(w)) name = w;
                    else return; //error: invalid identifier!!
                    paramcount = 0;
                } else if(i > 1) {
                    ++paramcount;
                    if(checkIdentifier(w)) s.param_names.insert(std::make_pair(w,paramcount)); //map param_name to position of parameter
                    else return; //error: invalid identifier!!
                }
            } else if(w == ":") {
                isLabel = true; //current line is a label def
            } else if(isLabel) {
                varbel_names.insert(std::make_pair(w,address + 2));
            } else break; //current line doesn't contain a function def or label
            Logger::loggerInst->debug(w);
        }

        if(paramcount > -1) { //curent line was a function def
            s.address = address + 2;
            s.param_count = paramcount;
            subroutines.insert(std::make_pair(name,s)); //store all the information about the subroutine for later use
            Logger::loggerInst->debug("Subroutine stored!");
        }
    }

    //second iteration to acctually assemble the code

    int state = 0; //where in the code are we? 0 = main code, 1 = subroutine
    subroutine* curr_s = nullptr; //stores the current subroutine information
    std::string curr_s_name = "";
    int globalvarcount = 0; //how many global vars have been defined so far
    // how many local vars have been defined so far (in case of state == 1)
    // start value of 2 is necessary because 0=return-address, 1=previous frame pointer and 2=previous stack pointer
    int localvarcount = 2;
    address = 1; //reset the address counter;
    mem->setValueAt(0,0x2); //set the default entrypoint to one in case it hasn't been set in the code

    for(int i = 0; i < lines.size(); ++i) {
        std::string e = lines.at(i);
        std::vector<std::string> words = splitString(e,' ');
        Logger::loggerInst->debug("Word count: ",words.size());
        if(words.size() > 0) {
            if(words.at(0) == "define") {
                if(state == 0) {
                    if(words.size() > 1 && checkIdentifier(words.at(1))) {
                        varbel_names.insert(std::make_pair(words.at(1),globalvarcount));
                        ++globalvarcount; //global vars are zero basedly indexed since the data pointer points to the first available ram slot
                    } else if(words.size() == 1) {
                        Logger::loggerInst->error("Missing identifier after keyword 'define' on line ", i + 1);
                        return;
                    } else return;
                } else {
                    if(words.size() > 1 && checkIdentifier(words.at(1))) {
                        ++localvarcount; //local vars are three basedly indexed since the frame pointer points to the return address and the previous frame and stack pointer value follow
                        curr_s->local_var_names.insert(std::make_pair(words.at(1),localvarcount));
                    } else if(words.size() == 1) {
                        Logger::loggerInst->error("Missing identifier after keyword 'define' on line ", i + 1);
                        return;
                    } else return;
                }
            } else if(words.at(0) == "endfunction") {
                //reset everything to state 0
                localvarcount = 2;
                curr_s = nullptr;
                curr_s_name = "";
                state = 0;
            } else if(words.at(0) == "function") {
                //init everything necessary for state 1
                state = 1;
                curr_s = &subroutines.at(words.at(1));
                curr_s_name = words.at(1);
            } else if(words.at(0) == "entrypoint") {
                //set the entrypoint address
                mem->setValueAt(0,address + 1);
            /*} else if(words.at(0) == ":") {
                //if identifier is valid, map the preceding address to it
                if(state == 0 && checkIdentifier(words.at(1))) {
                    varbel_names.insert(std::make_pair(words.at(1),address + 1));
                } else break;*/
            } else {
                if(words.at(0).at(0) == '#') {
                    continue; //line is a comment -> continue with next line
                } else if(words.at(0) == "call") {
                    if(words.size() > 1) {
                        if(subroutines.find(words.at(1)) != subroutines.end()) {
                            if(words.size() == subroutines.at(words.at(1)).param_count + 2) {
                                //mnemonic with more than two arguments (currently only possible when calling a function)
                                mem->setValueAt(address + 1, Constants::OP_CODES.at(words.at(0))); //store opcode for calling a function
                                mem->setValueAt(address + 2, subroutines.at(words.at(1)).param_count); //store amount of parameters
                                for(int i = 0; i < subroutines.at(words.at(1)).param_count * 2; i += 2) { //store all the parameters
                                    addressCompound adcp = getAddress(words.at(2+i/2),state, curr_s_name);
                                    mem->setValueAt(address + 3 + i, adcp.op_add); //store type of address
                                    mem->setValueAt(address + 4 + i, adcp.address); //store the address maped to the identifier
                                }
                                mem->setValueAt(address + subroutines.at(words.at(1)).param_count * 2 + 3, subroutines.at(words.at(1)).address);
                            } else if (words.size() > subroutines.at(words.at(1)).param_count + 2) {
                                Logger::loggerInst->error("Too many parameters provided for function '" + words.at(1) + "' on line ", i + 1);
                                return;
                            } else {
                                Logger::loggerInst->error("Not enough parameters provided for function '" + words.at(1) + "' on line ", i + 1);
                                return;
                            }
                        } else {
                            Logger::loggerInst->error("Undefined function '" + words.at(1) + "' called on line ", i + 1);
                        }
                    } else {
                        Logger::loggerInst->error("Missing function-identifier after mnemonic 'call' on line ", i + 1);
                        return;
                    }
                } else if(words.size() == 1) {
                    //if mnemonic has no arguments, store the apropriate op-code and a null argument
                    if(Constants::OP_CODES.find(words.at(0)) != Constants::OP_CODES.end()) {
                        mem->setValueAt(address + 1, Constants::OP_CODES.at(words.at(0)));
                        mem->setValueAt(address + 2, 0x0);
                    } else {
                        Logger::loggerInst->error("Unrecognized mnemonic on line ", i + 1);
                        return;
                    }
                } else if(words.size() == 2) {
                    //mnemonic with one argument
                    if(Constants::OP_CODES.find(words.at(0)) != Constants::OP_CODES.end()) {
                        addressCompound adcp = getAddress(words.at(1), state, curr_s_name);
                        if(adcp.valid) {
                            mem->setValueAt(address + 1, Constants::OP_CODES.at(words.at(0)) + adcp.op_add);
                            mem->setValueAt(address + 2, adcp.address);
                        } else return;
                    } else {
                        Logger::loggerInst->error("Unrecognized mnemonic on line ", i + 1);
                        return;
                    }
                } else if(words.size() == 3) {
                    if(words.at(0) == "mov") {
                        //mnemonic with two arguments (currently MOV is the only instruction with exactly two arguments, therefore the arg vals can just be added together with a shift of eleven)
                        mem->setValueAt(address + 1,Constants::OP_CODES.at(words.at(0)));
                        addressCompound adcp1 = getAddress(words.at(1), state, curr_s_name);
                        addressCompound adcp2 = getAddress(words.at(2), state, curr_s_name);
                        if(adcp1.valid && adcp2.valid) {
                            if(adcp1.op_add == adcp2.op_add && adcp1.address == adcp2.address) Logger::loggerInst->warning("Mov instruction without effect on line ", i + 1);
                            if(adcp1.op_add != Constants::ADR_REG || adcp2.op_add != Constants::ADR_REG) Logger::loggerInst->warning("Non-register-address argument will be treated as register-address anyways for mov instruction on line ", i+1);
                            mem->setValueAt(address + 2, adcp1.address + (adcp2.address << 11));
                        } else return;
                    } else {
                        Logger::loggerInst->error("Too many arguments for mnemonic '" + words.at(0) + "' on line ", i + 1);
                        return;
                    }
                } else {
                    //something weird happened
                    Logger::loggerInst->error("Syntax error on line ", i + 1);
                    return;
                }
            }
        }

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size() * 2 - 1; //function call requires two (OP_Code + Argument) + 2 times #parameters + one (function address) memory slots
        else if(words.size() > 0
                && Constants::ASSEMBLY_INST.find(words.at(0)) == Constants::ASSEMBLY_INST.end()) address += 2; //normal op code requires two memory slots (OP-Code + Argument)
    }
    mem->setValueAt(1,address + 1); //set the value the datapointer should be set to
    emit assemblyDone();
}

std::string Assembler::disassemble(RAM* mem) {
    return "";
}

std::vector<std::string> Assembler::splitString(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while(std::getline(ss,token,delimiter)) {
        if(token.length() > 0)
        result.push_back(token);
    }

    return result;
}

bool Assembler::checkIdentifier(std::string identifier) {
    //identifier is allowed to contain "A-Z", "a-z", "0-9" and "_" and must not begin with a number
    if(std::regex_match(identifier,std::regex("^[A-Za-z_][A-Za-z0-9_]*"))) {
        //identifier is not allowed to be equal to an opcode or assembler instruction
        if(Constants::OP_CODES.find(identifier) == Constants::OP_CODES.end() && Constants::ASSEMBLY_INST.find(identifier) == Constants::ASSEMBLY_INST.end()) {
            //identifier is not allowed to be equal to a register name
            if(Constants::REG_NAMES.find(identifier) == Constants::REG_NAMES.end()) {
                if(varbel_names.find(identifier) != varbel_names.end()) Logger::loggerInst->warning("Global identifier '" + identifier + "' overriden!");
                return true;
            }
        }
    }
    Logger::loggerInst->error("Identifier '" + identifier + "' must only contain A-Za-z0-9_ and must not begin with a number nor be a mnemonic or register name!");
    return false;
}

Assembler::addressCompound Assembler::getAddress(std::string idf, int state, std::string sub) {
    Logger::loggerInst->debug("Processing identifier: " + idf);
    addressCompound result;
    result.valid = false;
    bool isValue = idf[0] != '&';
    idf = isValue ? idf : idf.substr(1);
    if(std::regex_match(idf,std::regex("^[0-9]([xb0-9])?([0-9])*"))) {
        //if identifier is a number, it serves as the effective address/value
        std::stringstream ss(idf);
        sint effective_address;
        ss >> effective_address;
        result.address = effective_address;
        result.op_add = isValue ? Constants::VAL_ABSOLUTE : Constants::ADR_ABSOLUTE;
        result.valid = true;
    } else if(idf.find('[') != std::string::npos) {
        std::string actual_idf = idf.substr(4,idf.find(']') - 4);
        std::string type = idf.substr(0,3);
        bool numeric = isNumeric(actual_idf) > -1;
        if(state == 1 && (type == "loc" || type == "par")) {
            //if state is 1, local variables and parameters overwrite global ones
            subroutine* s = &subroutines.at(sub);
            if(type == "loc" && (s->local_var_names.find(actual_idf) != s->local_var_names.end() || numeric)) {
                result.address = numeric ? isNumeric(actual_idf) : s->local_var_names.at(actual_idf);
                result.op_add = isValue ? Constants::VAL_LOCAL : Constants::ADR_LOCAL;
                result.valid = true;
            } else if(type == "par" && (s->param_names.find(actual_idf) != s->param_names.end() || numeric)) {
                result.address = numeric ? isNumeric(actual_idf) : s->param_names.at(actual_idf);
                result.op_add = isValue ? Constants::VAL_PARAMETER : Constants::ADR_PARAMETER;
                result.valid = true;
            }
        }
        if(!result.valid) {
            //if state is 0 only global variables and label names are visible
            //if state is 1 and no local variables and parameters are found, global ones are searched instead
            if((type == "dpt" || type == "lab") && (varbel_names.find(actual_idf) != varbel_names.end() || numeric)) {
                result.address = numeric ? isNumeric(actual_idf) : varbel_names.at(actual_idf);
                result.op_add = isValue ? Constants::VAL_GLOBAL : Constants::ADR_GLOBAL;
                result.op_add = type == "lab" ? Constants::VAL_ABSOLUTE : result.op_add;
                result.valid = true;
            } else if(type == "reg" && (Constants::REG_NAMES.find(actual_idf) != Constants::REG_NAMES.end() || numeric)) {
                result.address = numeric ? isNumeric(actual_idf) : Constants::REG_NAMES.at(actual_idf);
                result.op_add = isValue ? Constants::VAL_REG : Constants::ADR_REG;
                result.valid = true;
            } else {
                Logger::loggerInst->error("Undefined identifier: " + idf); //identifier has not been defined (yet)!
            }
        }
    } else {
        Logger::loggerInst->error("Badly formated identifier: " + idf);
    }
    Logger::loggerInst->debug("Found address: ", result.address);
    return result;
}

sint Assembler::isNumeric(std::string idf) {
    if(std::regex_match(idf,std::regex("^[0-9]([xb0-9])?([0-9])*"))) {
        std::stringstream ss(idf);
        sint effective_address;
        ss >> effective_address;
        return effective_address;
    }
    return -1;
}
