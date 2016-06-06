#include "controller.h"
#include "constants.h"
#include "logger.h"
#include <sstream>

Controller::Controller()
{
    registers = new Register[11];

    ram = new RAM();
}

sint Controller::getRegisterVal(sint reg) {
    for(int i = 0; i <= 10; ++i) {
        if((reg & (1 << i)) != 0) return registers[i].getValue();
    }
    Logger::error("Inexistent register dereferenced.");
    return 0;
}

void Controller::setRegisterVal(sint reg, sint val) {
    for(int i = 0; i <= 10; ++i) {
        if((reg & (1 << i)) != 0) {
            registers[i].setValue(val);
            break;
        }
    }
}

void Controller::loadRamValDir(sint addr) {
    this->setRegisterVal(Constants::REG_IN,ram->getValueAt(addr));
}

void Controller::loadRamValInd(sint addr, sint pos) {
    switch(pos) {
        case 1:
            this->setRegisterVal(Constants::REG_IND1,ram->getValueAt(addr));
            this->setRegisterVal(Constants::REG_IN,
                                 ram->getValueAt(this->getRegisterVal(Constants::REG_IND1)));
            break;
        case 2:
            this->setRegisterVal(Constants::REG_IND2,ram->getValueAt(addr));
            this->setRegisterVal(Constants::REG_IN,
                                 ram->getValueAt(this->getRegisterVal(Constants::REG_IND2)));
            break;
        default:
            Logger::error("Inexistent indirect register chosen while loading value from ram->");
            return;
    }
}

void Controller::storeRamVal(sint addr) {
    ram->setValueAt(addr,this->getRegisterVal(Constants::REG_ACC));
}

sint Controller::calcAddress(sint addr, sint mode) {
    switch(mode) {
        case Constants::ADR_GLOBAL:
            return this->getRegisterVal(Constants::REG_DPT) + addr;
        case Constants::ADR_LOCAL:
        {
            sint result = this->getRegisterVal(Constants::REG_FPT) - addr;
            if(this->getRegisterVal(Constants::REG_SPT) > result) {
                this->setRegisterVal(Constants::REG_SPT,result + 1);
            }
            return result;
        }
        case Constants::ADR_PARAMETER:
            return this->getRegisterVal(Constants::REG_FPT) + addr;
        case Constants::ADR_REG:
            return -addr;
        default:
            return addr;
    }
}

void Controller::init() {
    sint pc = ram->getValueAt(0);
    this->setRegisterVal(Constants::REG_PC, pc);
    sint dp = ram->getValueAt(1);
    this->setRegisterVal(Constants::REG_DPT, dp);
    sint sp = ram->getMaxAddress();
    this->setRegisterVal(Constants::REG_SPT, sp);
    this->setRegisterVal(Constants::REG_FPT, sp);
}

sint Controller::fetchInstruction() {
    sint pc = this->getRegisterVal(Constants::REG_PC);
    sint inst = ram->getValueAt(pc);
    this->setRegisterVal(Constants::REG_OPC, inst);
    sint arg = ram->getValueAt(pc+1);
    this->setRegisterVal(Constants::REG_ARG, arg);
    this->setRegisterVal(Constants::REG_PC, pc + 2);
    return inst;
}

void Controller::callFunction() {
    sint param_count = this->getRegisterVal(Constants::REG_ARG);
    sint pc = this->getRegisterVal(Constants::REG_PC);
    sint stp = this->getRegisterVal(Constants::REG_SPT);
    for(sint i = param_count; i > 0; --i) {
        sint add_type = ram->getValueAt(pc + 2 + i*2);
        sint address = ram->getValueAt(pc + 3 + i*2);
        address = this->calcAddress(address,add_type);
        sint value;
        if(address >= 0) {
            value = ram->getValueAt(address);
        } else {
            value = this->getRegisterVal(-address);
        }
        ram->setValueAt(stp - param_count + i, value);
    }
    ram->setValueAt(stp - param_count, ram->getValueAt(pc + 4 + param_count*2));
    ram->setValueAt(stp - param_count - 1, stp);
    ram->setValueAt(stp - param_count - 2, this->getRegisterVal(Constants::REG_FPT));
    this->setRegisterVal(Constants::REG_FPT,stp - param_count);
    this->setRegisterVal(Constants::REG_SPT,stp - param_count - 3);
}

void Controller::returnFunction() {
    this->setRegisterVal(Constants::REG_PC, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT)));
    this->setRegisterVal(Constants::REG_SPT, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT) - 1));
    this->setRegisterVal(Constants::REG_FPT, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT) - 2));
}

void Controller::debugProcessor() {
    ram->debugRam();
    std::stringstream ss;
    ss << "Registers:" << std::endl;
    ss << "PC: " << Constants::intToHex(registers[0].getValue()) << "\tOPC: " << Constants::intToHex(registers[1].getValue()) << "\tARG: " << registers[2].getValue() << std::endl;
    ss << "ACC: " << registers[3].getValue() << "\tFLA: " << registers[4].getValue() << "\tIND1: " << Constants::intToHex(registers[5].getValue()) << std::endl;
    ss << "IND2: " << Constants::intToHex(registers[6].getValue()) << "\tIN: " << registers[7].getValue() << "\tDPT: " << registers[8].getValue() << std::endl;
    ss << "SPT: " << Constants::intToHex(registers[9].getValue()) << "\tFPT: " << Constants::intToHex(registers[10].getValue()) << std::endl;

    Logger::debug(ss.str());
}

RAM* Controller::getRam() {
    return ram;
}
