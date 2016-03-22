#define uint unsigned long int
#define sint long int
#include "assembler.h"
#include <string>
#include <map>

const std::map<std::string,const unsigned long int> Assembler::OP_Codes = {
    {"nop",0x00},
    {"mov",0x10},
    {"hlt",0x20},
    {"call",0x30},
    {"ret",0x40}
};

Assembler::Assembler() {

}

void Assembler::assemble(std::string code) {

}

std::string Assembler::disassemble() {

}
