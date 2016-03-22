#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#define uint unsigned long int
#define sint long int

#include <map>
#include <string>

class Assembler
{
public:
    Assembler();

    /**Assembles the code into the RAM*/
    void assemble(std::string);
    /**Disassembles the RAM and returns the code*/
    std::string disassemble();

private:
    /**Stores the OP-Codes' hexvalues needed to assemble the program into the RAM*/
    static const std::map<std::string,const unsigned long int> OP_Codes;

    /**While assembling, this holds the adresses belonging to each variable name and jumplable*/
    std::map<std::string,uint> varbel_names;

    struct subroutine{
        uint param_count;
        uint address;

        subroutine(uint pc,uint addr) : param_count(pc), address(addr) {}
    };

    /**While assembling, this stores all the information about subroutine calls (adress and parameter count)*/
    std::map<std::string,subroutine> subroutines;
};

#endif // ASSEMBLER_H
