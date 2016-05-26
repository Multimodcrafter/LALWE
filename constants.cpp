#include "constants.h"
#include <map>
#include <sstream>
#include <iomanip>
#include "stdint.h"
#define sint int32_t

const std::map<std::string,const sint> Constants::OP_CODES = {
    {"nop",0x00},
    {"mov",0x05},
    {"hlt",0x0A},
    {"call",0x10},
    {"ret",0x15}
};

const std::map<std::string,const sint> Constants::REG_NAMES = {
    {"akku",0},
    {"pc",1},
    {"opc",1 << 1},
    {"arg",1 << 2},
    {"id1",1 << 3},
    {"id2",1 << 4},
    {"fla",1 << 5},
    {"stp",1 << 6},
    {"frp",1 << 7},
    {"dap",1 << 8}
};

const std::set<std::string> Constants::ASSEMBLY_INST = {
    "define",
    "function",
    "endfunction",
    ":",
    "entrypoint"
};

std::map<sint, std::string> Constants::MNEMONICS;

std::string Constants::getMnemonic(sint opcode) {
    if(Constants::MNEMONICS.empty()) {
        for(auto it = Constants::OP_CODES.begin(); it != Constants::OP_CODES.end(); ++it) {
            Constants::MNEMONICS.insert(std::make_pair(it->second,it->first));
        }
    }
    return Constants::MNEMONICS.at((opcode/10)*10);
}

std::string Constants::intToHex(sint i)
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(sint)*2)
         << std::hex << i;
  return stream.str();
}

Constants::Constants()
{

}
