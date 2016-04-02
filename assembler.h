#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "stdint.h"
#define sint int32_t

#include <map>
#include <string>
#include "ram.h"
#include <vector>

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

    /**Object for subroutine data*/
    struct subroutine{
        sint param_count;
        sint address;

        subroutine(sint pc,sint addr) : param_count(pc), address(addr) {}
    };

    /**While assembling, this stores all the information about subroutine calls (adress and parameter count)*/
    std::map<std::string,subroutine> subroutines;

    std::vector<std::string> splitString(std::string,char);
};

#endif // ASSEMBLER_H
