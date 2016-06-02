#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <map>
#include <set>
#include "stdint.h"
#define sint int32_t

class Constants
{
public:
    Constants();

    /** Stores the OP-Codes' hexvalues needed to assemble the program into the RAM*/
    static const std::map<std::string,const sint> OP_CODES;
    /** Stores the numeric values used to identify the processor registers*/
    static const std::map<std::string,const sint> REG_NAMES;
    /** Stores all the assembler instructions which don't acctually get assembled*/
    static const std::set<std::string> ASSEMBLY_INST;

    /** constants used to determine if a numeric value is a variable, register or parameter*/
    static const int ADR_GLOBAL = 1;
    static const int ADR_LOCAL = 2;
    static const int ADR_REG = 3;
    static const int ADR_PARAMETER = 4;

    /** constants for easier register derefrencing*/
    static const int REG_PC = 1;
    static const int REG_OPC = 1 << 1;
    static const int REG_ARG = 1 << 2;
    static const int REG_ACC = 1 << 3;
    static const int REG_FLA = 1 << 4;
    static const int REG_IND1 = 1 << 5;
    static const int REG_IND2 = 1 << 6;
    static const int REG_IN = 1 << 7;
    static const int REG_DPT = 1 << 8;
    static const int REG_SPT = 1 << 9;
    static const int REG_FPT = 1 << 10;

    /** getter for the dissassembly*/
    static std::string getMnemonic(sint);
    /** formater function for hex values*/
    static std::string intToHex(sint);

private:

    /** Stores the hexvalues' coresponding mnemonics for disassembly*/
    static std::map<sint, std::string> MNEMONICS;
};

#endif // CONSTANTS_H
