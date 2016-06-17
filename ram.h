#ifndef RAM_H
#define RAM_H
#include "stdint.h"
#include <string>
#define sint int32_t

class RAM
{
public:
    RAM();

    /** returns the value inside the RAM at the specified location*/
    sint getValueAt(sint addr);

    /** stores the given value inside the RAM at the specified location*/
    void setValueAt(sint addr, sint value);

    /** returns the address range*/
    sint getMaxAddress();

    /** prints out a certain range of the RAM for deugging purposes*/
    void debugRam();

private:
    sint* memory;
};

#endif // RAM_H
