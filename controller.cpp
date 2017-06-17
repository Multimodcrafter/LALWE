#include "controller.h"
#include "constants.h"
#include "logger.h"
#include <sstream>

Controller::Controller(QObject &appMgr)
{
    registers.append(new Register(appMgr,QString("pc"),0));
    registers.append(new Register(appMgr,QString("ind1"),1));
    registers.append(new Register(appMgr,QString("ind2"),2));
    registers.append(new Register(appMgr,QString("input"),3));
    registers.append(new Register(appMgr,QString("accu"),4));
    registers.append(new Register(appMgr,QString("opc"),5));
    registers.append(new Register(appMgr,QString("arg"),6));
    registers.append(new Register(appMgr,QString("dpt"),7));
    registers.append(new Register(appMgr,QString("spt"),8));
    registers.append(new Register(appMgr,QString("fpt"),9));
    registers.append(new Register(appMgr,QString("fla"),10));

    ram = new RAM(appMgr);
    doAnimations = true;
    continueAnim = false;
    idle = false;

    lastComparison = Constants::CMP_EQUAL;
    QObject::connect(this, SIGNAL(setGuiProperty(QString,QVariant)), &appMgr, SLOT(setGuiProperty(QString,QVariant)));
    QObject::connect(&appMgr, SIGNAL(stepAnimation()), this, SLOT(stepAnimation()));
}

sint Controller::getRegisterVal(sint reg) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) return registers.at(i)->getValue();
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
    return 0;
}

void Controller::setRegisterVal(sint reg, sint val) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) {
            registers.at(i)->setValue(val);
            return;
        }
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
}

void Controller::decRegister(sint reg) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) {
            sint val = registers.at(i)->getValue();
            registers.at(i)->setValue(val - 1);
            return;
        }
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
}

void Controller::incRegister(sint reg) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) {
            sint val = registers.at(i)->getValue();
            registers.at(i)->setValue(val + 1);
            return;
        }
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
}

void Controller::negRegister(sint reg) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) {
            sint val = registers.at(i)->getValue();
            registers.at(i)->setValue(-val);
            return;
        }
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
}

void Controller::notRegister(sint reg) {
    for(int i = 0; i <= 10 && reg != 0; ++i) {
        if((reg & (1 << i)) == reg) {
            sint val = registers.at(i)->getValue();
            registers.at(i)->setValue(~val);
            return;
        }
    }
    Logger::loggerInst->error("Inexistent register dereferenced!");
}

void Controller::setLastCmp(int result) {
    lastComparison = result;
}

int Controller::getLastCmp() {
    return lastComparison;
}

void Controller::loadRamValDir(sint addr) {
    this->setRegisterVal(Constants::REG_IN,ram->getValueAt(addr));
}

void Controller::loadRamValInd(sint addr) {
    this->setRegisterVal(Constants::REG_IND1,ram->getValueAt(addr));
    this->setRegisterVal(Constants::REG_IN,
    ram->getValueAt(this->getRegisterVal(Constants::REG_IND1)));
}

void Controller::storeRamVal(sint addr) {
    ram->setValueAt(addr,this->getRegisterVal(Constants::REG_ACC));
}

sint Controller::calcActualValue(sint addr, sint mode, bool indirect) {
    switch(mode) {
        case Constants::ADR_GLOBAL:
            if(indirect) {
                setAddressMode("Indirect global address");
                return ram->getValueAt(this->getRegisterVal(Constants::REG_DPT) + addr);
            } else {
                setAddressMode("Global address");
            }
            return this->getRegisterVal(Constants::REG_DPT) + addr;
        case Constants::ADR_LOCAL:
        {
            sint result = this->getRegisterVal(Constants::REG_FPT) - addr;
            if(this->getRegisterVal(Constants::REG_SPT) > result) {
                this->setRegisterVal(Constants::REG_SPT,result + 1);
            }
            if(indirect) {
                setAddressMode("Indirect local address");
                return ram->getValueAt(result);
            } else {
                setAddressMode("Local address");
            }
            return result;
        }
        case Constants::ADR_PARAMETER:
            if(indirect) {
                setAddressMode("Indirect parameter address");
                return ram->getValueAt(this->getRegisterVal(Constants::REG_FPT) + addr);
            } else {
                setAddressMode("Parameter address");
            }
            return this->getRegisterVal(Constants::REG_FPT) + addr;
        case Constants::ADR_REG:
            if(indirect) {
                setAddressMode("Indirect register address");
                return this->getRegisterVal(addr);
            } else {
                setAddressMode("Register address");
            }
            return addr;
        case Constants::ADR_ABSOLUTE:
            if(indirect) {
                setAddressMode("Indirect absolute address");
                this->setRegisterVal(Constants::REG_IND2, ram->getValueAt(addr));
                return ram->getValueAt(this->getRegisterVal(Constants::REG_IND2));
            } else {
                setAddressMode("Absolute address");
            }
            return ram->getValueAt(addr);
        case Constants::VAL_ABSOLUTE:
            if(indirect) {
                setAddressMode("Indirect absolute value");
                this->setRegisterVal(Constants::REG_IND2, addr);
                return ram->getValueAt(this->getRegisterVal(Constants::REG_IND2));
            } else {
                setAddressMode("Absolute value");
            }
            return addr;
        case Constants::VAL_GLOBAL:
            if(indirect) {
                setAddressMode("Indirect global value");
                this->setRegisterVal(Constants::REG_IND2, ram->getValueAt(addr + this->getRegisterVal(Constants::REG_DPT)));
                return ram->getValueAt(this->getRegisterVal(Constants::REG_IND2));
            } else {
                setAddressMode("Global value");
            }
            return ram->getValueAt(addr + this->getRegisterVal(Constants::REG_DPT));
        case Constants::VAL_LOCAL:
        {
            sint result = this->getRegisterVal(Constants::REG_FPT) - addr;
            if(this->getRegisterVal(Constants::REG_SPT) > result) {
                this->setRegisterVal(Constants::REG_SPT,result + 1);
            }
            if(indirect) {
                setAddressMode("Indirect local value");
                this->setRegisterVal(Constants::REG_IND2, ram->getValueAt(result));
                return ram->getValueAt(this->getRegisterVal(Constants::REG_IND2));
            } else {
                setAddressMode("Local value");
            }
            return ram->getValueAt(result);
        }
        case Constants::VAL_PARAMETER:
        {
            sint result = this->getRegisterVal(Constants::REG_FPT) + addr;
            if(indirect) {
                setAddressMode("Indirect parameter value");
                this->setRegisterVal(Constants::REG_IND2, ram->getValueAt(result));
                return ram->getValueAt(this->getRegisterVal(Constants::REG_IND2));
            } else {
                setAddressMode("Parameter value");
            }
            return ram->getValueAt(result);
        }
        case Constants::VAL_REG:
            if(indirect) {
                setAddressMode("Indirect register value");
                return ram->getValueAt(this->getRegisterVal(addr));
            } else {
                setAddressMode("Register value");
            }
            return this->getRegisterVal(addr);
        default:
            setAddressMode("N/A");
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
    sint pc = this->getRegisterVal(Constants::REG_PC) - 2 ;
    sint stp = this->getRegisterVal(Constants::REG_SPT);
    for(sint i = (param_count - 1); i >= 0; --i) {
        sint add_type = ram->getValueAt(pc + 2 + i*2);
        sint address = ram->getValueAt(pc + 3 + i*2);
        address = this->calcActualValue(address,add_type,false);
        emit setGuiProperty("effectiveAddress",address);
        ram->setValueAt(stp - param_count + i + 1, address);
    }
    ram->setValueAt(stp - param_count, pc + 3 + param_count*2);
    ram->setValueAt(stp - param_count - 1, stp);
    ram->setValueAt(stp - param_count - 2, this->getRegisterVal(Constants::REG_FPT));
    this->setRegisterVal(Constants::REG_FPT,stp - param_count);
    this->setRegisterVal(Constants::REG_SPT,stp - param_count - 3);
    this->setRegisterVal(Constants::REG_PC, ram->getValueAt(pc + param_count * 2 + 2));
}

void Controller::returnFunction() {
    this->setRegisterVal(Constants::REG_PC, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT)));
    this->setRegisterVal(Constants::REG_SPT, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT) - 1));
    this->setRegisterVal(Constants::REG_FPT, ram->getValueAt(this->getRegisterVal(Constants::REG_FPT) - 2));
}

void Controller::debugProcessor() {
    if(Logger::loggerInst->loglevel == 0) {
        ram->debugRam();
        std::stringstream ss;
        ss << "Registers:" << std::endl;
        ss << "PC: " << Constants::intToHex(registers[0]->getValue()) << "\tOPC: " << Constants::intToHex(registers[1]->getValue()) << "\tARG: " << registers[2]->getValue() << std::endl;
        ss << "ACC: " << registers[3]->getValue() << "\tFLA: " << registers[4]->getValue() << "\tIND1: " << Constants::intToHex(registers[5]->getValue()) << std::endl;
        ss << "IND2: " << Constants::intToHex(registers[6]->getValue()) << "\tIN: " << registers[7]->getValue() << "\tDPT: " << registers[8]->getValue() << std::endl;
        ss << "SPT: " << Constants::intToHex(registers[9]->getValue()) << "\tFPT: " << Constants::intToHex(registers[10]->getValue()) << std::endl;

        Logger::loggerInst->debug(ss.str());
    }
}

RAM* Controller::getRam() {
    return ram;
}

void Controller::toggleAnimations(bool newState) {
    ram->toggleAnimations(newState);
    this->doAnimations = newState;
    for(int i = 0; i < registers.size(); ++i) {
        registers.at(i)->toggleAnimations(newState);
    }
}

void Controller::reset() {
    ram->reset();
    for(int i = 0; i < registers.size(); ++i) {
        registers.at(i)->setValue(0);
    }
}

void Controller::setAddressMode(std::string mode) {
    emit setGuiProperty("addressMode", QVariant::fromValue(QString::fromStdString(mode)));
    sleep
}

void Controller::stepAnimation() {
    if(idle) continueAnim = true;
}
