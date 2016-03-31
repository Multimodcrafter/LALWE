#define uint unsigned long int
#define sint long int
#include "assembler.h"
#include <string>
#include <map>

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

}

std::string Assembler::disassemble(RAM& mem) {

}
