#include "stdint.h"
#define sint int32_t
#include "ram.h"
#include "constants.h"
#include "logger.h"
#define arraysize (1 << 5)

RAM::RAM() : memory(new sint [arraysize])
{
    for(int i = 0; i < arraysize; ++i) {
        memory[i] = 0;
    }
}

sint RAM::getValueAt(sint addr) {
    return memory[addr];
}

void RAM::setValueAt(sint addr, sint value) {
    memory[addr] = value;
}

sint RAM::getMaxAddress() {
    return (arraysize) - 1;
}

void RAM::debugRam() {
    std::string result = "RAM contents:";
    for(int i = 0; i < 16; ++i) {
        result += "\n";
        result += Constants::intToHex(memory[i]);
        result += "\t";
        result += Constants::intToHex(memory[i + 16]);
    }
    Logger::debug(result);
}
