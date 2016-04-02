#include "stdint.h"
#define sint int32_t
#include "ram.h"

RAM::RAM() : memory(new sint [(1 << 27)])
{

}

sint RAM::getValueAt(sint addr) {
    return memory[addr];
}

void RAM::setValueAt(sint addr, sint value) {
    memory[addr] = value;
}
