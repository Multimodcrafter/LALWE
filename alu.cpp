#include "alu.h"
#include "constants.h"

ALU::ALU(Controller* cont,QObject &appMgr) : controller(cont)
{
    doAnimations = true;
    continueAnim = false;
    idle = false;
    QObject::connect(this,SIGNAL(setGuiProperty(QString,QVariant)), &appMgr, SLOT(setGuiProperty(QString,QVariant)));
    QObject::connect(&appMgr, SIGNAL(stepAnimation()), this, SLOT(stepAnimation()));
}

void ALU::add(sint value) {
    setOperator("+");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval + value == 0) flag += Constants::FLA_ZERO;
    if(accval + value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval + value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((accval > 0 && value > 0 && accval + value < 0) || (accval < 0 && value < 0 && accval + value > 0)) flag += Constants::FLA_OVERFLOW;
    setResult(accval + value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval + value);
}

void ALU::subtract(sint value) {
    setOperator("-");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval - value == 0) flag += Constants::FLA_ZERO;
    if(accval - value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval - value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((accval < 0 && value > 0 && accval - value > 0) || (accval > 0 && value < 0 && accval - value < 0)) flag += Constants::FLA_OVERFLOW;
    setResult(accval - value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval - value);
}

void ALU::multiply(sint value) {
    setOperator("*");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval * value == 0) flag += Constants::FLA_ZERO;
    if(accval * value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval * value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((((accval > 0 && value > 0) || (accval < 0 && value < 0)) && accval * value < 0) ||
            (((accval > 0 && value < 0) || (accval < 0 && value > 0)) && accval * value > 0)) flag += Constants::FLA_OVERFLOW;
    setResult(accval * value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval * value);
}

void ALU::divide(sint value) {
    setOperator("/");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval / value == 0) flag += Constants::FLA_ZERO;
    if(accval / value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval / value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((((accval > 0 && value > 0) || (accval < 0 && value < 0)) && accval / value < 0) ||
            (((accval > 0 && value < 0) || (accval < 0 && value > 0)) && accval / value > 0)) flag += Constants::FLA_OVERFLOW;
    setResult(accval/value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval / value);
}

void ALU::And(sint value) {
    setOperator("&");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval & value) == 0) flag += Constants::FLA_ZERO;
    if((accval & value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval & value) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(accval & value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval & value);
}

void ALU::Or(sint value) {
    setOperator("|");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval | value) == 0) flag += Constants::FLA_ZERO;
    if((accval | value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval | value) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(accval | value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval | value);
}

void ALU::Xor(sint value) {
    setOperator("^");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval ^ value) == 0) flag += Constants::FLA_ZERO;
    if((accval ^ value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval ^ value) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(accval ^ value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval ^ value);
}

void ALU::rotL(sint value) {
    setOperator("|<<|");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint s =  value >= 0 ? value % 32 : -((-value)%32);
    sint overflow = (accval >> (32 - s)) & (~(-1 << s));
    sint result = (accval << s) | overflow;
    sint flag = 0;
    if((result) == 0) flag += Constants::FLA_ZERO;
    if((result) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(result) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(result);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, result);
}

void ALU::rotR(sint value) {
    setOperator("|>>|");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint s =  value >= 0 ? value % 32 : -((-value)%32);
    sint overflow = (accval << (32 - s));
    sint result = ((accval >> s) & (~(-1 << (32 - s)))) | overflow;
    sint flag = 0;
    if((result) == 0) flag += Constants::FLA_ZERO;
    if((result) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(result) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(result);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, result);
}

void ALU::shiL(sint value) {
    setOperator("<<");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = Constants::FLA_OVERFLOW;
    if((accval << value) == 0) flag += Constants::FLA_ZERO;
    if((accval << value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval << value) % 2 == 1) flag += Constants::FLA_PARITY;
    if(value < 33 && ((1 << (32 - value)) & accval) != 0) flag += Constants::FLA_CARRY;
    setResult(accval << value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval << value);
}

void ALU::shiR(sint value) {
    setOperator(">>");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval >> value) == 0) flag += Constants::FLA_ZERO;
    if((accval >> value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval & value) % 2 == 1) flag += Constants::FLA_PARITY;
    setResult(accval >> value);
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval >> value);
}

void ALU::comp(sint value) {
    setOperator("?");
    setArg(value);
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    if(accval > value) {
        controller->setLastCmp(Constants::CMP_LESS);
        setResult(1);
    } else if(accval < value){
        controller->setLastCmp(Constants::CMP_GREATER);
        setResult(-1);
    } else if(accval == value) {
        controller->setLastCmp(Constants::CMP_EQUAL);
        setResult(0);
    }
}

void ALU::toggleAnimations(bool newState) {
    this->doAnimations = newState;
}

void ALU::setOperator(std::string op) {
    emit setGuiProperty("operation",QVariant::fromValue(QString::fromStdString(op)));
}

void ALU::setArg(sint arg) {
    if(doAnimations) emit setGuiProperty("busToAluActive",QVariant::fromValue(true));
    sleep
    emit setGuiProperty("operand",QVariant::fromValue(arg));
    sleep
    if(doAnimations) emit setGuiProperty("busToAluActive",QVariant::fromValue(false));
}

void ALU::setResult(sint res) {
    if(doAnimations) emit setGuiProperty("aluActive",QVariant::fromValue(true));
    sleep
    emit setGuiProperty("result",QVariant::fromValue(res));
    sleep
    if(doAnimations) emit setGuiProperty("aluActive",QVariant::fromValue(false));
    if(doAnimations) emit setGuiProperty("busFromAluActive",QVariant::fromValue(true));
    sleep
    if(doAnimations) emit setGuiProperty("busFromAluActive",QVariant::fromValue(false));
}

sint ALU::countBits(sint value) {
    sint count = 0;
    for(int i = 0; i < 32; ++i) {
        if((value & (1 << i)) != 0) ++count;
    }
    return count;
}

void ALU::stepAnimation() {
    if(idle) {
        continueAnim = true;
    }
}
