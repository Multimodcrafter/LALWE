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

    /** getter for the dissassembly*/
    static std::string getMnemonic(sint);
    /** formater function for hex values*/
    static std::string intToHex(sint);

private:

    /** Stores the hexvalues' coresponding mnemonics for disassembly*/
    static std::map<sint, std::string> MNEMONICS;
};

#endif // CONSTANTS_H
