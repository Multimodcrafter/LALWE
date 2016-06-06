#include "processor.h"
#include "constants.h"
#include "stdint.h"

Processor::Processor()
{
    controller = new Controller();
    alu = new ALU(controller);
}

void Processor::runProgram() {
    controller->init();
    controller->debugProcessor();
    while(controller->getRegisterVal(Constants::REG_PC) < controller->getRegisterVal(Constants::REG_DPT)) {
        sint instruction = controller->fetchInstruction(); //fetch the next instruction
        sint normalized_inst = instruction & 0xfffffff0; //remove the instruction mode
        sint instruction_mode = instruction - normalized_inst; //get the instruction mode (i.e. how to treat the address)
        sint address = 0;
        sint value = 0;
        switch(normalized_inst) {
            case MOV:
            {
                sint argument = controller->getRegisterVal(Constants::REG_ARG);
                sint reg1 = 0;
                sint reg2 = 0;
                //get the two registers from the argument
                //the source lies in the first ten bits
                //the destination lies in the second ten bits
                for(int i = 0; i <= 10; ++i) {
                    if(((argument & 0b1111111111) & (1 << i)) != 0) {
                        reg1 = (1 << i);
                    }
                    if(((argument >> 11) & (1 << i)) != 0) {
                        reg2 = (1 << i);
                    }
                }
                controller->setRegisterVal(reg2,controller->getRegisterVal(reg1)); //move the value
            }
                break;
            case HLT:
                return; //program should terminate -> exit the execution procedure
            case CALL:
                controller->callFunction();
                break;
            case RET:
                controller->returnFunction();
                break;
            case LD1:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValInd(address,1);
                } else {
                    controller->loadRamValInd(controller->getRegisterVal(-address),1);
                }
                break;
            case LD2:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValInd(address,2);
                } else {
                    controller->loadRamValInd(controller->getRegisterVal(-address),2);
                }
                break;
            case LDE:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValDir(address);
                } else {
                    controller->loadRamValDir(controller->getRegisterVal(-address));
                }
                break;
            case LDA:
                controller->setRegisterVal(Constants::REG_IN,controller->getRegisterVal(Constants::REG_ARG));
                break;
            case STO:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->storeRamVal(address);
                } else {
                    controller->storeRamVal(controller->getRegisterVal(-address));
                }
                break;
            case ADD:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValDir(address);
                } else {
                    controller->loadRamValDir(controller->getRegisterVal(-address));
                }
                alu->add(controller->getRegisterVal(Constants::REG_IN));
                break;
            case ADDI:
                value = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                alu->add(value);
                break;
            case SUB:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValDir(address);
                } else {
                    controller->loadRamValDir(controller->getRegisterVal(-address));
                }
                alu->subtract(controller->getRegisterVal(Constants::REG_IN));
                break;
            case SUBI:
                value = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                alu->subtract(value);
                break;
            case DIV:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValDir(address);
                } else {
                    controller->loadRamValDir(controller->getRegisterVal(-address));
                }
                alu->divide(controller->getRegisterVal(Constants::REG_IN));
                break;
            case DIVI:
                value = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                alu->divide(value);
                break;
            case MUL:
                address = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                if(address > 0) {
                    controller->loadRamValDir(address);
                } else {
                    controller->loadRamValDir(controller->getRegisterVal(-address));
                }
                alu->multiply(controller->getRegisterVal(Constants::REG_IN));
                break;
            case MULI:
                value = controller->calcAddress(controller->getRegisterVal(Constants::REG_ARG),instruction_mode);
                alu->multiply(value);
                break;
        }
        controller->debugProcessor();
    }
}

RAM* Processor::getRam() {
    return controller->getRam();
}
