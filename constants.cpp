#include "constants.h"
#include <map>
#include "stdint.h"
#define sint int32_t

const std::map<std::string,const sint> Constants::OP_CODES = {
    {"nop",0x00},
    {"mov",0x10},
    {"hlt",0x20},
    {"call",0x30},
    {"ret",0x40}
};

const std::map<std::string,const sint> Constants::REG_NAMES = {
    {"akku",0b0},
    {"pc",0b1},
    {"opc",0b10},
    {"arg",0b100}
};

const std::map<sint,const std::string> Constants::MNEMONICS = {
    {0x00,"nop"},
    {0x10,"mov"},
    {0x20,"hlt"},
    {0x30,"call"},
    {0x40,"ret"}
};

const std::set<std::string> Constants::ASSEMBLY_INST = {
    "define",
    "function",
    "endfunction",
    ":",
    "entrypoint"
};

Constants::Constants()
{

}
