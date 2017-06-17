#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <map>
#include <set>
#include <chrono>
#include <thread>
#include <QSettings>
#include "stdint.h"
#define sint int32_t

#define NOP 0x000
#define MOV 0x010
#define HLT 0x020
#define CALL 0x030
#define RET 0x040
#define LDI 0x050
#define LD 0x060
#define STO 0x070
#define ADD 0x080
#define ADDI 0x090
#define DIV 0x0A0
#define DIVI 0x0B0
#define MUL 0x0C0
#define MULI 0x0D0
#define SUB 0x0E0
#define SUBI 0x0F0
#define DEC 0x100
#define INC 0x110
#define NEG 0x120
#define NOT 0x130
#define AND 0x140
#define ANDI 0x150
#define OR 0x160
#define ORI 0x170
#define XOR 0x180
#define XORI 0x190
#define ROL 0x1A0
#define ROLI 0x1B0
#define ROR 0x1C0
#define RORI 0x1D0
#define SHL 0x1E0
#define SHLI 0x1F0
#define SHR 0x200
#define SHRI 0x210
#define CMP 0x220
#define CMPI 0x230
#define JG 0x240
#define JGE 0x250
#define JL 0x260
#define JLE 0x270
#define JE 0x280
#define JMP 0x290
#define JO 0x2A0
#define JNO 0x2B0
#define JP 0x2C0
#define JNP 0x2D0
#define JZ 0x2E0
#define JNZ 0x2F0
#define JS 0x300
#define JNS 0x310
#define JC 0x320
#define JNC 0x330
#define RIN 0x340
#define WOUT 0x350

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
    static const int ADR_ABSOLUTE = 6;
    static const int ADR_GLOBAL = 2;
    static const int ADR_LOCAL = 3;
    static const int ADR_REG = 4;
    static const int ADR_PARAMETER = 5;
    static const int VAL_ABSOLUTE = 1;
    static const int VAL_GLOBAL = 7;
    static const int VAL_LOCAL = 8;
    static const int VAL_REG = 9;
    static const int VAL_PARAMETER = 10;

    /** constants for easier register derefrencing*/
    static const int REG_PC = 1;
    static const int REG_OPC = 1 << 5;
    static const int REG_ARG = 1 << 6;
    static const int REG_ACC = 1 << 4;
    static const int REG_FLA = 1 << 10;
    static const int REG_IND1 = 1 << 1;
    static const int REG_IND2 = 1 << 2;
    static const int REG_IN = 1 << 3;
    static const int REG_DPT = 1 << 7;
    static const int REG_SPT = 1 << 8;
    static const int REG_FPT = 1 << 9;

    /** constants for flagpositions*/
    static const int FLA_OVERFLOW = 1 << 1;
    static const int FLA_SIGN = 1 << 4;
    static const int FLA_ZERO = 1 << 2;
    static const int FLA_PARITY = 1 << 3;
    static const int FLA_CARRY = 1;

    /** constants for comparison result*/
    static const int CMP_LESS = 0;
    static const int CMP_EQUAL = 1;
    static const int CMP_GREATER = 2;

    static const int ANIM_MAX_DELAY;
    static const int ANIM_MIN_DELAY;
    static int ANIM_DELAY_MILIS;

    #define sleep if(doAnimations) {\
        idle = true;\
        while(!continueAnim) \
        {\
            std::this_thread::sleep_for(std::chrono::milliseconds(10));\
        }\
        idle = false;\
        continueAnim = false;\
    }

    static const int RAM_VIEW_CELL_AMOUNT;

    static long long int ARRAYSIZE;

    /** getter for the dissassembly*/
    static std::string getMnemonic(sint opcode);
    /** formater function for hex values*/
    static std::string intToHex(sint i);

private:

    /** Stores the hexvalues' coresponding mnemonics for disassembly*/
    static std::map<sint, std::string> MNEMONICS;

    static QSettings settings;
};

#endif // CONSTANTS_H
