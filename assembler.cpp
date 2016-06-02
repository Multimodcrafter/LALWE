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

void Assembler::assemble(std::string code, RAM& mem) {
    std::vector<std::string> lines = splitString(code,'\n');
    sint address = 0;

    //first iteration to get all the information about all subroutines
    for(std::string e : lines) {
        Logger::debug(e);
        std::vector<std::string> words = splitString(e,' ');

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size() * 2 - 1; //function call requires two (OP_Code + Argument) + 2 times #parameters + one (function address) memory slots
        else if(words.size() > 0
                && Constants::ASSEMBLY_INST.find(words.at(0)) == Constants::ASSEMBLY_INST.end()) address += 2; //normal op code requires two memory slots (OP-Code + Argument)

        //current subroutine definition
        bool isFunctionDef = false;
        sint paramcount = -1;
        std::string name = "";
        subroutine s;

        for(unsigned int i = 0; i < words.size(); ++i) {
            std::string w = words.at(i);
            if(w == "function") isFunctionDef = true; //curent line is a function def
            else if(isFunctionDef) {
                if(i == 1) { //first word after "function" is equal to the name of the subroutine
                    if(checkIdentifier(w)) name = w;
                    else break; //error: invalid identifier!!
                    paramcount = 0;
                } else if(i > 1) {
                    ++paramcount;
                    if(checkIdentifier(w))s.param_names.insert(std::make_pair(w,paramcount)); //map param_name to position of parameter
                    else break; //error: invalid identifier!!
                }
            } else break; //current line doesn't contain a function def
            Logger::debug(w);
        }

        if(paramcount > -1) { //curent line was a function def
            s.address = address + 2;
            s.param_count = paramcount;
            subroutines.insert(std::make_pair(name,s)); //store all the information about the subroutine for later use
            Logger::debug("Subroutine stored!");
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
    mem.setValueAt(0,0x1); //set the default entrypoint to one in case it hasn't been set in the code

    for(std::string e : lines) {
        std::vector<std::string> words = splitString(e,' ');
        if(words.size() > 0) {
            if(words.at(0) == "define") {
                if(state == 0) {
                    if(checkIdentifier(words.at(1))) {
                        varbel_names.insert(std::make_pair(words.at(1),globalvarcount));
                        ++globalvarcount; //global vars are zero basedly indexed since the data pointer points to the first available ram slot
                    } else break;
                } else {
                    if(checkIdentifier(words.at(1))) {
                        ++localvarcount; //local vars are three basedly indexed since the frame pointer points to the return address and the previous frame and stack pointer value folow
                        curr_s->local_var_names.insert(std::make_pair(words.at(1),localvarcount));
                    } else break;
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
                mem.setValueAt(0,address + 1);
            } else if(words.at(0) == ":") {
                //if identifier is valid, map the preceding address to it
                if(state == 0 && checkIdentifier(words.at(1))) {
                    varbel_names.insert(std::make_pair(words.at(1),address + 1));
                } else break;
            } else {
                if(words.size() == 1) {
                    //if mnemonic has no arguments, store the apropriate op-code and a null argument
                    mem.setValueAt(address + 1, Constants::OP_CODES.at(words.at(0)));
                    mem.setValueAt(address + 2, 0x0);
                } else if(words.at(0) == "call") {
                    //mnemonic with more than two arguments (currently only possible when calling a function)
                    mem.setValueAt(address + 1, Constants::OP_CODES.at(words.at(0))); //store opcode for calling a function
                    mem.setValueAt(address + 2, subroutines.at(words.at(1)).param_count); //store amount of parameters
                    for(int i = 0; i < subroutines.at(words.at(1)).param_count * 2; i += 2) { //store all the parameters
                        addressCompound adcp = getAdress(words.at(2+i),state, curr_s_name);
                        mem.setValueAt(address + 3 + i, adcp.op_add); //store type of address
                        mem.setValueAt(address + 4 + i, adcp.address); //store the address maped to the identifier
                    }
                    mem.setValueAt(address + subroutines.at(words.at(1)).param_count * 2 + 3, subroutines.at(words.at(1)).address);
                } else if(words.size() == 2) {
                    //mnemonic with one argument
                    addressCompound adcp = getAdress(words.at(1), state, curr_s_name);
                    mem.setValueAt(address + 1, Constants::OP_CODES.at(words.at(0)) + adcp.op_add);
                    mem.setValueAt(address + 2, adcp.address);
                } else if(words.size() == 3) {
                    //mnemonic with two arguments (currently MOV is the only instruction with exactly two arguments, therefore the arg vals can just be added together with a shift of eleven)
                    mem.setValueAt(address + 1,Constants::OP_CODES.at(words.at(0)));
                    addressCompound adcp1 = getAdress(words.at(1), state, curr_s_name);
                    addressCompound adcp2 = getAdress(words.at(2), state, curr_s_name);
                    mem.setValueAt(address + 2, adcp1.address + (adcp2.address << 11));
                } else {
                    //something weird happened
                    Logger::error("Syntax error!");
                }
            }
        }

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size() * 2 - 1; //function call requires two (OP_Code + Argument) + 2 times #parameters + one (function address) memory slots
        else if(words.size() > 0
                && Constants::ASSEMBLY_INST.find(words.at(0)) == Constants::ASSEMBLY_INST.end()) address += 2; //normal op code requires two memory slots (OP-Code + Argument)
    }
    mem.setValueAt(1,address); //set the value the datapointer should be set to
}

std::string Assembler::disassemble(RAM& mem) {
    return "";
}

std::vector<std::string> Assembler::splitString(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while(std::getline(ss,token,delimiter)) {
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
            if(Constants::REG_NAMES.find(identifier) == Constants::REG_NAMES.end())
                return true;
        }
    }
    Logger::error("Invalid identifier in code!");
    return false;
}

Assembler::addressCompound Assembler::getAdress(std::string idf, int state, std::string sub) {
    Logger::debug("Processing identifier: " + idf);
    addressCompound result;
    result.valid = false;
    if(std::regex_match(idf,std::regex("^[0-9]"))) {
        //if identifier is a number, it serves as the effective address/value
        std::stringstream ss(idf);
        sint effective_address;
        ss >> effective_address;
        result.address = effective_address;
        result.op_add = 0;
        result.valid = true;
    } else if(state == 1) {
        //if state is 1, local variables and parameters overwrite global ones
        subroutine* s = &subroutines.at(sub);
        if(s->local_var_names.find(idf) != s->local_var_names.end()) {
            result.address = s->local_var_names.at(idf);
            result.op_add = Constants::ADR_LOCAL;
            result.valid = true;
        } else if(s->param_names.find(idf) != s->param_names.end()) {
            result.address = s->param_names.at(idf);
            result.op_add = Constants::ADR_PARAMETER;
            result.valid = true;
        }
    }
    if(!result.valid) {
        //if state is 0 only global variables and label names are visible
        //if state is 1 and no local variables and parameters are found, global ones are searched instead
        if(varbel_names.find(idf) != varbel_names.end()) {
            result.address = varbel_names.at(idf);
            result.op_add = Constants::ADR_GLOBAL;
            result.valid = true;
        } else if(Constants::REG_NAMES.find(idf) != Constants::REG_NAMES.end()) {
            result.address = Constants::REG_NAMES.at(idf);
            result.op_add = Constants::ADR_REG;
            result.valid = true;
        } else {
            Logger::error("Undefined Identifier!"); //identifier has not been defined (yet)!
        }
    }
    Logger::debug("Found address: ", result.address);
    return result;
}
