#include "stdint.h"
#define sint int32_t
#include "ram.h"
#include "constants.h"
#include "logger.h"

RAM::RAM() : memory(new sint [(1 << 27)])
{

}

sint RAM::getValueAt(sint addr) {
    return memory[addr];
}

void RAM::setValueAt(sint addr, sint value) {
    memory[addr] = value;
}

sint RAM::getMaxAddress() {
    return sizeof(memory) / sizeof(sint) - 1;
}

void RAM::debugRam() {
    std::string result = "RAM contents:";
    for(int i = 0; i < 15; ++i) {
        result += "\n";
        result += Constants::intToHex(memory[i]);
    }
    Logger::debug(result);
}
