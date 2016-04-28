#include "stdint.h"
#define sint int32_t
#include "assembler.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <regex>
#include "logger.h"

const std::map<std::string,const sint> Assembler::OP_Codes = {
    {"nop",0x00},
    {"mov",0x10},
    {"hlt",0x20},
    {"call",0x30},
    {"ret",0x40}
};

const std::map<std::string,const sint> Assembler::reg_names = {
    {"akku",0b0},
    {"pc",0b1},
    {"opc",0b10},
    {"arg",0b100}
};

const std::map<sint,const std::string> Assembler::mnemonics = {
    {0x00,"nop"},
    {0x10,"mov"},
    {0x20,"hlt"},
    {0x30,"call"},
    {0x40,"ret"}
};

Assembler::Assembler() {

}

void Assembler::assemble(std::string code, RAM& mem) {
    std::vector<std::string> lines = splitString(code,'\n');
    sint address = -1;

    //first iteration to get all the information about all subroutines
    for(std::string e : lines) {
        Logger::debug(e);
        std::vector<std::string> words = splitString(e,' ');

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size(); //function call requires two + #parameters memory slots
        else if(words.size() > 0 && words.at(0) != "function"
                && words.at(0) != "define"
                && words.at(0).at(0) != ':'
                && words.at(0) != "endfunction"
                && words.at(0) != "entrypoint") address += 2; //normal op code requires two memory slots (OP-Code + Argument)

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
            s.address = address;
            s.param_count = paramcount;
            subroutines.insert(std::make_pair(name,s)); //store all the information about the subroutine for later use
            Logger::debug("Subroutine stored!");
        }
    }

    //second iteration to acctually assemble the code

    int state = 0; //where in the code are we? 0 = main code, 1 = subroutine
    subroutine& curr_s; //stores the current subroutine information
    int globalvarcount = 0; //how many global vars have been defined so far
    int localvarcount = 0; // how many local vars have been defined so far (in case of state == 1)
    address = 0; //reset the address counter;
    mem.setValueAt(0,0x1); //set the default entrypoint to one in case it hasn't been set in the code

    for(std::string e : lines) {
        std::vector<std::string> words = splitString(e,' ');
        if(words.size() > 0) {
            switch(words.at(0)) {
            case "define":
                if(state == 0) {
                    if(checkIdentifier(words.at(1))) {
                        varbel_names.insert(std::make_pair(words.at(1),globalvarcount));
                        ++globalvarcount; //global vars are zero basedly indexed since the data pointer points to the first available ram slot
                    } else break;
                } else {
                    if(checkIdentifier(words.at(1))) {
                        ++localvarcount; //local vars are one basedly indexed since the frame pointer points to the return address
                        curr_s.local_var_names.insert(std::make_pair(words.at(1),localvarcount));
                    } else break;
                }
                break;
            case "endfunction":
                //reset everything to state 0
                localvarcount = 0;
                curr_s = nullptr_t;
                state = 0;
                break;
            case "function":
                //init everything necessary for state 1
                state = 1;
                curr_s = subroutines.at(words.at(1));
                break;
            case "entrypoint":
                //set the entrypoint address
                mem.setValueAt(0,address + 1);
                break;
            default:
                mem.setValueAt(address + 1,OP_Codes.at(words.at(0)));
                break;
            }
        }

        //increase addresspointer apropriately
        if(words.size() > 0 && words.at(0) == "call") address += words.size(); //function call requires two + #parameters memory slots
        else if(words.size() > 0 && words.at(0) != "function"
                && words.at(0) != "define"
                && words.at(0).at(0) != ':'
                && words.at(0) != "endfunction"
                && words.at(0) != "entrypoint") address += 2; //normal op code requires two memory slots (OP-Code + Argument)
    }
}

std::string Assembler::disassemble(RAM& mem) {

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
    //identifier is allowed to contain "A-Z", "a-z", "0-9" and "_"
    if(std::regex_match(identifier,std::regex("[A-Za-z0-9_]+"))) {
        //identifier is not allowed to be equal to an opcode
        if(OP_Codes.find(identifier) == OP_Codes.end()) {
            //identifier is not allowed to be equal to a register name
            if(reg_names.find(identifier) == reg_names.end())
                return true;
        }
    }
    Logger::error("Invalid identifier in code!");
    return false;
}
