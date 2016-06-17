#ifndef REGISTER_H
#define REGISTER_H
#include "stdint.h"
#define sint int32_t

class Register
{
public:
    Register();

    sint getValue();

    void setValue(sint);

private:
    sint value;
};

#endif // REGISTER_H
