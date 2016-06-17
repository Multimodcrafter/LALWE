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

    void And(sint value);

    void Or(sint value);

    void Xor(sint value);

    void rotL(sint value);

    void rotR(sint value);

    void shiL(sint value);

    void shiR(sint value);

    void comp(sint value);
private:
    Controller* controller;

    sint countBits(sint value);
};

#endif // ALU_H
