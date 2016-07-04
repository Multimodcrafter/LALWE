#include "stdint.h"
#define sint int32_t
#include "ram.h"
#include "constants.h"
#include "logger.h"
#include <sstream>
#include <QVariant>
#define arraysize (1 << 7)

RAM::RAM(QObject &appMgr) : memory(new sint [arraysize])
{
    for(int i = 0; i < arraysize; ++i) {
        memory[i] = 0;
    }
    addressView1 = 0;
    addressView2 = arraysize - Constants::RAM_VIEW_CELL_AMOUNT;
    formatHex = false;
    doAnimations = true;
    continueAnim = false;
    idle = false;
    followActive = true;
    QObject::connect(this, SIGNAL(setCtxtProperty(QString,QVariant)), &appMgr, SLOT(setCtxProperty(QString,QVariant)));
    QObject::connect(this, SIGNAL(setGuiProperty(QString,QVariant)), &appMgr, SLOT(setGuiProperty(QString,QVariant)));
    QObject::connect(&appMgr, SIGNAL(stepAnimation()), this, SLOT(stepAnimation()));
    updateList();
}

sint RAM::getValueAt(sint addr) {
    if(addr < arraysize) {
        highlightCell(addr);
        if(addressInSight(addr)) {
            sleep
        }
        highlightCell(-1);
        return memory[addr];
    } else {
        Logger::error("Address out of range.");
        return 0;
    }
}

void RAM::setValueAt(sint addr, sint value) {
    if(addr < arraysize) {
        highlightCell(addr);
        if(addressInSight(addr)) {
            sleep
        }
        memory[addr] = value;
        updateList();
        if(addressInSight(addr)) {
            sleep
        }
        highlightCell(-1);
    } else {
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

void RAM::toggleHexDisplay(bool newState) {
    this->formatHex = newState;
    updateList();
}

void RAM::toggleAnimations(bool newState) {
    this->doAnimations = newState;
}

void RAM::setRamViewAddress(int index, QString action) {
    if(action == "inc") {
        if(index == 1) {
            if(addressView1 + Constants::RAM_VIEW_CELL_AMOUNT <= arraysize - Constants::RAM_VIEW_CELL_AMOUNT) {
                addressView1 += Constants::RAM_VIEW_CELL_AMOUNT;
            } else {
                addressView1 = arraysize - Constants::RAM_VIEW_CELL_AMOUNT;
            }
        } else {
            if(addressView2 + Constants::RAM_VIEW_CELL_AMOUNT <= arraysize - Constants::RAM_VIEW_CELL_AMOUNT) {
                addressView2 += Constants::RAM_VIEW_CELL_AMOUNT;
            } else {
                addressView2 = arraysize - Constants::RAM_VIEW_CELL_AMOUNT;
            }
        }
    } else if(action == "dec") {
        if(index == 1) {
            if(addressView1 - Constants::RAM_VIEW_CELL_AMOUNT >= 0) {
                addressView1 -= Constants::RAM_VIEW_CELL_AMOUNT;
            } else {
                addressView1 = 0;
            }
        } else {
            if(addressView2 - Constants::RAM_VIEW_CELL_AMOUNT >= 0) {
                addressView2 -= Constants::RAM_VIEW_CELL_AMOUNT;
            } else {
                addressView2 = 0;
            }
        }
    } else {
        bool ok = false;
        sint result = action.toInt(&ok);
        if(ok && result >= 0 && result <= arraysize - Constants::RAM_VIEW_CELL_AMOUNT) {
            if(index == 1) {
                addressView1 = result;
            } else {
                addressView2 = result;
            }
        }
    }
    emit setGuiProperty("activeRamSlot" + QVariant::fromValue(index).toString(), QVariant::fromValue(0));
    emit setGuiProperty("activeRamSlot" + QVariant::fromValue(index).toString(), QVariant::fromValue(-1));
    updateList();
}

void RAM::updateList() {
    QStringList list;
    for(int i = addressView1; i < addressView1 + Constants::RAM_VIEW_CELL_AMOUNT && i < arraysize; ++i) {
        std::stringstream ss;
        if(formatHex) {
            ss << Constants::intToHex(i) << ":\t" << Constants::intToHex(memory[i]);
        } else {
            ss << i << ":\t" << memory[i];
        }
        std::string result = ss.str();
        list.append(QString::fromStdString(result));
    }
    emit setCtxtProperty("myModel1",QVariant::fromValue(list));
    list.clear();
    for(int i = addressView2; i < addressView2 + Constants::RAM_VIEW_CELL_AMOUNT && i < arraysize; ++i) {
        std::stringstream ss;
        if(formatHex) {
            ss << Constants::intToHex(i) << ":\t" << Constants::intToHex(memory[i]);
        } else {
            ss << i << ":\t" << memory[i];
        }
        std::string result = ss.str();
        list.append(QString::fromStdString(result));
    }
    emit setCtxtProperty("myModel2",QVariant::fromValue(list));
}

void RAM::highlightCell(sint addr) {
    if(doAnimations) {
        if(addr == -1) {
            emit setGuiProperty("activeRamSlot1",-1);
            emit setGuiProperty("activeRamSlot2",-1);
        }
        if(addr >= addressView1 && addr < addressView1 + Constants::RAM_VIEW_CELL_AMOUNT) {
            emit setGuiProperty("activeRamSlot1",QVariant::fromValue(addr - addressView1));
        } else if(addr >= 0 && addr < arraysize && followActive) {
            if(addr < addressView1) {
                setRamViewAddress(1,QVariant::fromValue(addr).toString());
                emit setGuiProperty("activeRamSlot1",QVariant::fromValue(0));
            } else {
                setRamViewAddress(1,QVariant::fromValue(addr - Constants::RAM_VIEW_CELL_AMOUNT + 1).toString());
                emit setGuiProperty("activeRamSlot1",QVariant::fromValue(Constants::RAM_VIEW_CELL_AMOUNT - 1));
            }
        }
        if(addr >= addressView2 && addr < addressView2 + Constants::RAM_VIEW_CELL_AMOUNT) {
            emit setGuiProperty("activeRamSlot2",QVariant::fromValue(addr - addressView2));
        }
    }
}

bool RAM::addressInSight(sint addr) {
    if(addr >= addressView1 && addr < addressView1 + Constants::RAM_VIEW_CELL_AMOUNT) {
        return true;
    }
    if(addr >= addressView2 && addr < addressView2 + Constants::RAM_VIEW_CELL_AMOUNT) {
        return true;
    }
    return false;
}

void RAM::stepAnimation() {
    if(idle) continueAnim = true;
}

void RAM::toggleFollowActive(const bool &newState) {
    followActive = newState;
}

void RAM::reset() {
    for(int i = 0; i < arraysize; ++i) {
        memory[i] = 0;
    }
    updateList();
}
