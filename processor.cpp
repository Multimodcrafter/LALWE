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
            case LDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                if(instruction_mode == Constants::VAL_ABSOLUTE || instruction_mode == Constants::VAL_GLOBAL ||
                        instruction_mode == Constants::VAL_LOCAL || instruction_mode == Constants::VAL_PARAMETER ||
                        instruction_mode == Constants::VAL_REG) controller->loadRamValDir(value);
                else controller->loadRamValInd(value);
                break;
            case LD:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                if(instruction_mode == Constants::VAL_ABSOLUTE || instruction_mode == Constants::VAL_GLOBAL ||
                        instruction_mode == Constants::VAL_LOCAL || instruction_mode == Constants::VAL_PARAMETER ||
                        instruction_mode == Constants::VAL_REG) controller->setRegisterVal(Constants::REG_IN,value);
                else controller->loadRamValDir(value);
                break;
            case STO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->storeRamVal(value);
                break;
            case ADD:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->add(value);
                break;
            case ADDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->add(value);
                break;
            case SUB:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->subtract(value);
                break;
            case SUBI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->subtract(value);
                break;
            case DIV:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->divide(value);
                break;
            case DIVI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->divide(value);
                break;
            case MUL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->multiply(value);
                break;
            case MULI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->multiply(value);
                break;
        }
        controller->debugProcessor();
    }
}

RAM* Processor::getRam() {
    return controller->getRam();
}
