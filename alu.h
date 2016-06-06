#ifndef ALU_H
#define ALU_H
#include "controller.h"


class ALU
{
public:
    ALU(Controller* cont);

    void add(sint value);

    void subtract(sint value);

    void multiply(sint value);

    void divide(sint value);
private:
    Controller* controller;

    sint countBits(sint value);
};

#endif // ALU_H
