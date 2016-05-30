#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "controller.h"
#include "alu.h"

class Processor
{
public:
    Processor();

    /** runs the programm stored in the ram*/
    void runProgram();

private:
    Controller controller;

    ALU alu;
};

#endif // PROCESSOR_H
