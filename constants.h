#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <map>
#include <set>
#include "stdint.h"
#define sint int32_t

#define NOP 0x000
#define MOV 0x010
#define HLT 0x020
#define CALL 0x030
#define RET 0x040
#define LD1 0x050
#define LD2 0x060
#define LDE 0x070
#define LDA 0x080
#define STO 0x090
#define ADD 0x0A0
#define ADDI 0x0B0
#define DIV 0x0C0
#define DIVI 0x0D0
#define MUL 0x0E0
#define MULI 0x0F0
#define SUB 0x100
#define SUBI 0x110

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

    /** constants for flagpositions*/
    static const int FLA_OVERFLOW = 1;
    static const int FLA_SIGN = 1 << 1;
    static const int FLA_ZERO = 1 << 2;
    static const int FLA_PARITY = 1 << 3;
    //static const int FLA_CARRY = 1 << 4;

    /** getter for the dissassembly*/
    static std::string getMnemonic(sint opcode);
    /** formater function for hex values*/
    static std::string intToHex(sint i);

private:

    /** Stores the hexvalues' coresponding mnemonics for disassembly*/
    static std::map<sint, std::string> MNEMONICS;
};

#endif // CONSTANTS_H
