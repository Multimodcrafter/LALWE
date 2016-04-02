#ifndef RAM_H
#define RAM_H
#include "stdint.h"
#define sint int32_t

class RAM
{
public:
    RAM();
    sint getValueAt(sint);
    void setValueAt(sint, sint);

private:
    sint* memory;
};

#endif // RAM_H
