#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#define sint long int

#include <map>
#include <string>
#include "ram.h"

class Assembler
{
public:
    Assembler();

    /**Assembles the code into the RAM*/
    void assemble(std::string, RAM&);
    /**Disassembles the RAM and returns the code*/
    std::string disassemble(RAM&);

private:
    /**Stores the OP-Codes' hexvalues needed to assemble the program into the RAM*/
    static const std::map<std::string,const sint> OP_Codes;
    /**Stores the hexvalues' coresponding mnemonics for disassembly*/
    static const std::map<sint, const std::string> mnemonics;

    /**While assembling, this holds the adresses belonging to each variable name and jumplable*/
    std::map<std::string,sint> varbel_names;

    struct subroutine{
        sint param_count;
        sint address;

        subroutine(sint pc,sint addr) : param_count(pc), address(addr) {}
    };

    /**While assembling, this stores all the information about subroutine calls (adress and parameter count)*/
    std::map<std::string,subroutine> subroutines;
};

#endif // ASSEMBLER_H
