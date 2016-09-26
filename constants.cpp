#include "constants.h"
#include <map>
#include <sstream>
#include <iomanip>
#include "stdint.h"
#define sint int32_t

const std::map<std::string,const sint> Constants::OP_CODES = {
    {"nop",NOP},
    {"mov",MOV},
    {"hlt",HLT},
    {"call",CALL},
    {"ret",RET},
    {"ld",LD},
    {"ldi",LDI},
    {"sto",STO},
    {"add",ADD},
    {"addi",ADDI},
    {"sub",SUB},
    {"subi",SUBI},
    {"mul",MUL},
    {"muli",MULI},
    {"div",DIV},
    {"divi",DIVI},
    {"dec",DEC},
    {"inc",INC},
    {"neg",NEG},
    {"not",NOT},
    {"and",AND},
    {"andi",ANDI},
    {"or",OR},
    {"ori",ORI},
    {"xor",XOR},
    {"xori",XORI},
    {"rol",ROL},
    {"roli",ROLI},
    {"ror",ROR},
    {"rori",RORI},
    {"shl",SHL},
    {"shli",SHLI},
    {"shr",SHR},
    {"shri",SHRI},
    {"cmp",CMP},
    {"cmpi",CMPI},
    {"jg",JG},
    {"jge",JGE},
    {"jl",JL},
    {"jle",JLE},
    {"je",JE},
    {"jmp",JMP},
    {"jo",JO},
    {"jno",JNO},
    {"jp",JP},
    {"jnp",JNP},
    {"jz",JZ},
    {"jnz",JNZ},
    {"js",JS},
    {"jns",JNS},
    {"jc",JC},
    {"jnc",JNC},
    {"in",RIN},
    {"out",WOUT},
};

const std::map<std::string,const sint> Constants::REG_NAMES = {
    {"pc",Constants::REG_PC},
    {"opc",Constants::REG_OPC},
    {"arg",Constants::REG_ARG},
    {"acc",Constants::REG_ACC},
    {"fla",Constants::REG_FLA},
    {"id1",Constants::REG_IND1},
    {"id2",Constants::REG_IND2},
    {"in",Constants::REG_IN},
    {"dpt",Constants::REG_DPT},
    {"spt",Constants::REG_SPT},
    {"fpt",Constants::REG_FPT}
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
    return Constants::MNEMONICS.at(opcode);
}

std::string Constants::intToHex(sint i)
{
  std::stringstream stream;
  stream << "0x"
         << std::setfill ('0') << std::setw(sizeof(sint)*2)
         << std::hex << i;
  return stream.str();
}

QSettings Constants::settings("config.ini",QSettings::IniFormat);

const int Constants::ANIM_MIN_DELAY = Constants::settings.value("settings/animation_minimum_delay",10).toInt();
const int Constants::ANIM_MAX_DELAY = Constants::settings.value("settings/animation_maximum_delay",1000).toInt();
const int Constants::RAM_VIEW_CELL_AMOUNT = Constants::settings.value("settings/ram_view_cell_amount",15).toInt();

int Constants::ANIM_DELAY_MILIS = Constants::ANIM_MIN_DELAY + (Constants::ANIM_MAX_DELAY - Constants::ANIM_MIN_DELAY) / 2;

long long int Constants::ARRAYSIZE = (1 << 31) - 1;

Constants::Constants()
{

}
