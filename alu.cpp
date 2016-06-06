#include "alu.h"
#include "constants.h"

ALU::ALU(Controller* cont) : controller(cont)
{

}

void ALU::add(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval + value == 0) flag += Constants::FLA_ZERO;
    if(accval + value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval + value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((accval > 0 && value > 0 && accval + value < 0) || (accval < 0 && value < 0 && accval + value > 0)) flag += Constants::FLA_OVERFLOW;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval + value);
}

void ALU::subtract(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval - value == 0) flag += Constants::FLA_ZERO;
    if(accval - value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval - value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((accval < 0 && value > 0 && accval - value > 0) || (accval > 0 && value < 0 && accval - value < 0)) flag += Constants::FLA_OVERFLOW;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval - value);
}

void ALU::multiply(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval * value == 0) flag += Constants::FLA_ZERO;
    if(accval * value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval * value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((((accval > 0 && value > 0) || (accval < 0 && value < 0)) && accval * value < 0) ||
            (((accval > 0 && value < 0) || (accval < 0 && value > 0)) && accval * value > 0)) flag += Constants::FLA_OVERFLOW;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval * value);
}

void ALU::divide(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if(accval / value == 0) flag += Constants::FLA_ZERO;
    if(accval / value < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval / value) % 2 == 1) flag += Constants::FLA_PARITY;
    if((((accval > 0 && value > 0) || (accval < 0 && value < 0)) && accval / value < 0) ||
            (((accval > 0 && value < 0) || (accval < 0 && value > 0)) && accval / value > 0)) flag += Constants::FLA_OVERFLOW;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval / value);
}

sint ALU::countBits(sint value) {
    sint count = 0;
    for(int i = 0; i < 32; ++i) {
        if((value & (1 << i)) != 0) ++count;
    }
    return count;
}
