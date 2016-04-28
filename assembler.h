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
    /**Stores the numeric values used to identify the processor registers*/
    static const std::map<std::string,const sint> reg_names;
    /**Stores the hexvalues' coresponding mnemonics for disassembly*/
    static const std::map<sint, const std::string> mnemonics;

    /**While assembling, this holds the adresses belonging to each variable name and jumplable*/
    std::map<std::string,sint> varbel_names;

    /**Object for subroutine data*/
    struct subroutine{
        sint param_count;
        sint address;

        std::map<std::string,sint> param_names;
        std::map<std::string,sint> local_var_names;

        subroutine(sint pc,sint addr) : param_count(pc), address(addr) {}
        subroutine() {}
    };

    /**While assembling, this stores all the information about subroutine calls (adress and parameter count)*/
    std::map<std::string,subroutine> subroutines;

    /**Splits a string into a vector at all delimiter-character occurences*/
    std::vector<std::string> splitString(std::string,char);

    /**checks if a label-, variable- or subroutinename is valid*/
    bool checkIdentifier(std::string);
};

#endif // ASSEMBLER_H
