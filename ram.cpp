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
    QObject::connect(this, SIGNAL(setCtxtProperty(QString,QVariant)), &appMgr, SLOT(setCtxProperty(QString,QVariant)));
    QObject::connect(this, SIGNAL(setGuiProperty(QString,QVariant)), &appMgr, SLOT(setGuiProperty(QString,QVariant)));
    updateList();
}

sint RAM::getValueAt(sint addr) {
    if(addr < arraysize) {
        highlightCell(addr);
        sleep
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
        sleep
        memory[addr] = value;
        updateList();
        sleep
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
        }
        if(addr >= addressView2 && addr < addressView2 + Constants::RAM_VIEW_CELL_AMOUNT) {
            emit setGuiProperty("activeRamSlot2",QVariant::fromValue(addr - addressView2));
        }
    }
}
