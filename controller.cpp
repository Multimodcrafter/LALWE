#include "controller.h"
#include "constants.h"

Controller::Controller()
{
    registers = new Register[11];
}

sint Controller::getRegisterVal(sint reg) {
    for(int i = 0; i <= 10; ++i) {
        if((reg & (1 << i)) != 0) return registers[i].getValue();
    }
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

void Controller::init() {
    sint pc = ram.getValueAt(0);
    sint dp = ram.getValueAt(1);
    sint sp = ram.getMaxAddress();
    this->setRegisterVal(Constants::REG_PC, pc);
    this->setRegisterVal(Constants::REG_DPT, dp);
    this->setRegisterVal(Constants::REG_SPT, sp);
    this->setRegisterVal(Constants::REG_FPT, sp);
}

sint Controller::fetchInstruction() {
    sint pc = this->getRegisterVal(Constants::REG_PC);
    sint inst = ram.getValueAt(pc);
    sint arg = ram.getValueAt(pc+1);
    this->setRegisterVal(Constants::REG_OPC, inst);
    this->setRegisterVal(Constants::REG_ARG, arg);
    this->setRegisterVal(Constants::REG_PC, pc + 2);
    return inst;
}

void Controller::callFunction() {

}

void Controller::returnFunction() {

}
