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
        sint cmpResult = 0;
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
                        instruction_mode == Constants::VAL_REG || instruction_mode == Constants::ADR_REG) controller->setRegisterVal(Constants::REG_IN,value);
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
            case DEC:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->decRegister(value);
                break;
            case INC:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->incRegister(value);
                break;
            case NEG:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->negRegister(value);
                break;
            case NOT:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->notRegister(value);
                break;
            case AND:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->And(value);
                break;
            case ANDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->And(value);
                break;
            case OR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->Or(value);
                break;
            case ORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->Or(value);
                break;
            case XOR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->Xor(value);
                break;
            case XORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->Xor(value);
                break;
            case ROL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->rotL(value);
                break;
            case ROLI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->rotL(value);
                break;
            case ROR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->rotR(value);
                break;
            case RORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->rotR(value);
                break;
            case SHL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->shiL(value);
                break;
            case SHLI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->shiL(value);
                break;
            case SHR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->shiR(value);
                break;
            case SHRI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->shiR(value);
                break;
            case CMP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                alu->comp(value);
                break;
            case CMPI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                alu->comp(value);
                break;
            case JG:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_GREATER) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JGE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_GREATER || cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JL:value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_LESS) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JLE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_LESS || cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JMP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                controller->setRegisterVal(Constants::REG_PC, value);
                break;
            case JO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_OVERFLOW) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_OVERFLOW) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_PARITY) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_PARITY) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JZ:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_ZERO) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNZ:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_ZERO) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JS:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_SIGN) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNS:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_SIGN) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case IN:
                break;
            case OUT:
                break;
        }
        controller->debugProcessor();
    }
}

RAM* Processor::getRam() {
    return controller->getRam();
}
