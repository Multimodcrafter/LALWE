#include "register.h"
#include "constants.h"

Register::Register(QObject &appMgr, QString &name, int pos) :
    position(pos)
{
    value = 0;
    doAnimations = true;
    continueAnim = false;
    idle = false;
    this->name = name;
    QObject::connect(this,SIGNAL(setGuiProperty(QString,QVariant)),&appMgr,SLOT(setGuiProperty(QString,QVariant)));
    QObject::connect(&appMgr, SIGNAL(stepAnimation()), this, SLOT(stepAnimation()));
}

sint Register::getValue() {
    highlightReg(true);
    sleep
    highlightReg(false);
    return value;
}

void Register::setValue(sint newval) {
    highlightReg(true);
    sleep
    value = newval;
    emit setGuiProperty(name,QVariant::fromValue(value));
    sleep
    highlightReg(false);
}

void Register::toggleAnimations(bool newState) {
    this->doAnimations = newState;
}

void Register::highlightReg(bool active) {
    if(doAnimations) {
        if(active) {
            emit setGuiProperty("activeRegister",QVariant::fromValue(position));
        } else {
            emit setGuiProperty("activeRegister",QVariant::fromValue(-1));
        }
    }
}

void Register::stepAnimation() {
    if(idle) continueAnim = true;
}
