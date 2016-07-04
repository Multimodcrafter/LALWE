#include "processor.h"
#include "constants.h"
#include "stdint.h"
#include "logger.h"
#include <chrono>
#include <thread>

Processor::Processor(QObject &appMgr)
{
    controller = new Controller(appMgr);
    alu = new ALU(controller,appMgr);
    doAnimations = true;
    continueAnim = false;
    idle = false;
    waitForInput = false;
    QObject::connect(this, SIGNAL(setGuiProperty(QString,QVariant)), &appMgr, SLOT(setGuiProperty(QString,QVariant)));
    QObject::connect(&appMgr, SIGNAL(stepAnimation()), this, SLOT(stepAnimation()));
    QObject::connect(this, SIGNAL(printLine(QString)), &appMgr, SLOT(printLine(QString)));
}

void Processor::runProgram() {
    setCycleState(-1);
    controller->init();
    controller->debugProcessor();
    while(controller->getRegisterVal(Constants::REG_PC) < controller->getRegisterVal(Constants::REG_DPT)) {
        setCycleState(0);
        sint instruction = controller->fetchInstruction(); //fetch the next instruction
        setCycleState(1);
        sint normalized_inst = instruction & 0xfffffff0; //remove the instruction mode
        sint instruction_mode = instruction - normalized_inst; //get the instruction mode (i.e. how to treat the address)
        setDecodedOpc(QString::fromStdString(Constants::getMnemonic(normalized_inst)).toUpper());
        sint value = 0;
        sint cmpResult = 0;
        setCycleState(2);
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
                setEffectiveAddress("N/A");
                emit setGuiProperty("addressMode",QVariant::fromValue(QString("N/A")));
                setCycleState(-1);
                return; //program should terminate -> exit the execution procedure
            case CALL:
                setEffectiveAddress("N/A");
                emit setGuiProperty("addressMode",QVariant::fromValue(QString("N/A")));
                controller->callFunction();
                break;
            case RET:
                setEffectiveAddress("N/A");
                emit setGuiProperty("addressMode",QVariant::fromValue(QString("N/A")));
                controller->returnFunction();
                break;
            case LDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                if(instruction_mode == Constants::VAL_ABSOLUTE || instruction_mode == Constants::VAL_GLOBAL ||
                        instruction_mode == Constants::VAL_LOCAL || instruction_mode == Constants::VAL_PARAMETER ||
                        instruction_mode == Constants::VAL_REG) controller->loadRamValDir(value);
                else controller->loadRamValInd(value);
                break;
            case LD:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                if(instruction_mode == Constants::VAL_ABSOLUTE || instruction_mode == Constants::VAL_GLOBAL ||
                        instruction_mode == Constants::VAL_LOCAL || instruction_mode == Constants::VAL_PARAMETER ||
                        instruction_mode == Constants::VAL_REG || instruction_mode == Constants::ADR_REG) controller->setRegisterVal(Constants::REG_IN,value);
                else controller->loadRamValDir(value);
                break;
            case STO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->storeRamVal(value);
                break;
            case ADD:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->add(value);
                break;
            case ADDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->add(value);
                break;
            case SUB:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->subtract(value);
                break;
            case SUBI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->subtract(value);
                break;
            case DIV:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->divide(value);
                break;
            case DIVI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->divide(value);
                break;
            case MUL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->multiply(value);
                break;
            case MULI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->multiply(value);
                break;
            case DEC:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->decRegister(value);
                break;
            case INC:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->incRegister(value);
                break;
            case NEG:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->negRegister(value);
                break;
            case NOT:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->notRegister(value);
                break;
            case AND:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->And(value);
                break;
            case ANDI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->And(value);
                break;
            case OR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->Or(value);
                break;
            case ORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->Or(value);
                break;
            case XOR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->Xor(value);
                break;
            case XORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->Xor(value);
                break;
            case ROL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->rotL(value);
                break;
            case ROLI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->rotL(value);
                break;
            case ROR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->rotR(value);
                break;
            case RORI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->rotR(value);
                break;
            case SHL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->shiL(value);
                break;
            case SHLI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->shiL(value);
                break;
            case SHR:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->shiR(value);
                break;
            case SHRI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->shiR(value);
                break;
            case CMP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->comp(value);
                break;
            case CMPI:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,true);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                alu->comp(value);
                break;
            case JG:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_GREATER) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JGE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_GREATER || cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JL:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_LESS) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JLE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_LESS || cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JE:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getLastCmp();
                if(cmpResult == Constants::CMP_EQUAL) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JMP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                controller->setRegisterVal(Constants::REG_PC, value);
                break;
            case JO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_OVERFLOW) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNO:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_OVERFLOW) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_PARITY) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNP:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_PARITY) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JZ:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_ZERO) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNZ:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_ZERO) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JS:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_SIGN) != 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case JNS:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                cmpResult = controller->getRegisterVal(Constants::REG_FLA);
                if((cmpResult & Constants::FLA_SIGN) == 0) {
                    controller->setRegisterVal(Constants::REG_PC, value);
                }
                break;
            case IN:
                waitForInput = true;
                while(waitForInput) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                controller->setRegisterVal(Constants::REG_IN, inputValue);
                break;
            case OUT:
                value = controller->calcActualValue(controller->getRegisterVal(Constants::REG_ARG),instruction_mode,false);
                setEffectiveAddress(QVariant::fromValue(value).toString());
                emit printLine("Program output: " + QVariant::fromValue(value).toString());
                break;
        }
        controller->debugProcessor();
        emit setGuiProperty("decodedOpcode","N/A");
        emit setGuiProperty("effectiveAddress", "N/A");
        emit setGuiProperty("addressMode","N/A");
    }
    setCycleState(-1);
    emit setGuiProperty("status","Ready");
}

RAM* Processor::getRam() {
    return controller->getRam();
}

void Processor::toggleAnimations(bool newState) {
    alu->toggleAnimations(newState);
    controller->toggleAnimations(newState);
    doAnimations = newState;
}

void Processor::reset() {
    controller->reset();
}

void Processor::setCycleState(int state) {
    if(doAnimations) {
        emit setGuiProperty("cycleState",QVariant::fromValue(state));
    }
    sleep
}

void Processor::setDecodedOpc(QString opc) {
    emit setGuiProperty("decodedOpcode",QVariant::fromValue(opc));
    sleep
}

void Processor::setEffectiveAddress(QString addr) {
    emit setGuiProperty("effectiveAddress", QVariant::fromValue(addr));
    sleep
}

void Processor::stepAnimation() {
    if(idle) continueAnim = true;
}

void Processor::sendInput(sint value) {
    inputValue = value;
    waitForInput = false;
}
