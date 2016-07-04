#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "stdint.h"
#define sint int32_t

#include <map>
#include <string>
#include "ram.h"
#include <vector>

class Assembler : public QObject
{
    Q_OBJECT
signals:
    void assemblyDone();
public:
    Assembler();

    /** Assembles the code into the RAM*/
    void assemble(std::string code, RAM* mem);
    /** Disassembles the RAM and returns the code*/
    std::string disassemble(RAM* mem);

private:
    /** While assembling, this holds the adresses belonging to each variable name and jumplable*/
    std::map<std::string,sint> varbel_names;

    /** Object for subroutine data*/
    struct subroutine{
        sint param_count;
        sint address;

        std::map<std::string,sint> param_names;
        std::map<std::string,sint> local_var_names;

        subroutine(sint pc,sint addr) : param_count(pc), address(addr) {}
        subroutine() {}
    };

    /** While assembling, this stores all the information about subroutine calls (adress and parameter count)*/
    std::map<std::string,subroutine> subroutines;

    /** Splits a string into a vector at all delimiter-character occurences*/
    std::vector<std::string> splitString(std::string str,char delimiter);

    /** Checks if a label-, variable- or subroutinename is valid*/
    bool checkIdentifier(std::string identifier);

    /** Stores an address and if its local, global, a parameter or a register*/
    struct addressCompound{
        sint address = 0;
        int op_add = 0;
        bool valid = false;

        addressCompound() {}
    };

    /** Returns the adress mapped to an identifier, based on the curent state of the assembler (global or inside a function)*/
    addressCompound getAddress(std::string idf, int state, std::string sub);

    sint isNumeric(std::string idf);
};

#endif // ASSEMBLER_H
