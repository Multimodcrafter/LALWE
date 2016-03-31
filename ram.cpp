#define sint long int
#include "ram.h"

RAM::RAM() : memory(new sint [1 << 30])
{

}

sint RAM::getValueAt(sint addr) {
    return memory[addr];
}

void RAM::setValueAt(sint addr, sint value) {
    memory[addr] = value;
}
