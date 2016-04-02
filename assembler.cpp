#include "stdint.h"
#define sint int32_t
#include "assembler.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include "logger.h"

const std::map<std::string,const sint> Assembler::OP_Codes = {
    {"nop",0x00},
    {"mov",0x10},
    {"hlt",0x20},
    {"call",0x30},
    {"ret",0x40}
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
    for(std::string e : lines) {
        Logger::debug(e);
        std::vector<std::string> words = splitString(e,' ');
        for(std::string w : words) {
            Logger::debug(w);
        }
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
