#include "stdint.h"
#define sint int32_t
#include "ram.h"
#include "constants.h"
#include "logger.h"
#define arraysize (1 << 7)

RAM::RAM() : memory(new sint [arraysize])
{
    for(int i = 0; i < arraysize; ++i) {
        memory[i] = 0;
    }
}

sint RAM::getValueAt(sint addr) {
    if(addr < arraysize)
    return memory[addr];
    else {
        Logger::error("Address out of range.");
        return 0;
    }
}

void RAM::setValueAt(sint addr, sint value) {
    if(addr < arraysize) memory[addr] = value;
    else {
        Logger::error("Address out of range.");
    }
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
        result += "\t";
        result += Constants::intToHex(memory[i + 32]);
        result += "\t";
        result += Constants::intToHex(memory[i + 48]);
        result += "\t";
        result += Constants::intToHex(memory[i + 64]);
        result += "\t";
        result += Constants::intToHex(memory[i + 80]);
        result += "\t";
        result += Constants::intToHex(memory[i + 96]);
        result += "\t";
        result += Constants::intToHex(memory[i + 112]);
    }
    Logger::debug(result);
}
