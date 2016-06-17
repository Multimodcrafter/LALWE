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

void ALU::And(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval & value) == 0) flag += Constants::FLA_ZERO;
    if((accval & value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval & value) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval & value);
}

void ALU::Or(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval | value) == 0) flag += Constants::FLA_ZERO;
    if((accval | value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval | value) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval | value);
}

void ALU::Xor(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval ^ value) == 0) flag += Constants::FLA_ZERO;
    if((accval ^ value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval ^ value) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval ^ value);
}

void ALU::rotL(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint s =  value >= 0 ? value % 32 : -((-value)%32);
    sint overflow = (accval >> (32 - s)) & (~(-1 << s));
    sint result = (accval << s) | overflow;
    sint flag = 0;
    if((result) == 0) flag += Constants::FLA_ZERO;
    if((result) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(result) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, result);
}

void ALU::rotR(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint s =  value >= 0 ? value % 32 : -((-value)%32);
    sint overflow = (accval << (32 - s));
    sint result = ((accval >> s) & (~(-1 << (32 - s)))) | overflow;
    sint flag = 0;
    if((result) == 0) flag += Constants::FLA_ZERO;
    if((result) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(result) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, result);
}

void ALU::shiL(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = Constants::FLA_OVERFLOW;
    if((accval << value) == 0) flag += Constants::FLA_ZERO;
    if((accval << value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval << value) % 2 == 1) flag += Constants::FLA_PARITY;
    if(value < 33 && ((1 << (32 - value)) & accval) != 0) flag += Constants::FLA_CARRY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval << value);
}

void ALU::shiR(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    sint flag = 0;
    if((accval >> value) == 0) flag += Constants::FLA_ZERO;
    if((accval >> value) < 0) flag += Constants::FLA_SIGN;
    if(this->countBits(accval & value) % 2 == 1) flag += Constants::FLA_PARITY;
    controller->setRegisterVal(Constants::REG_FLA,flag);
    controller->setRegisterVal(Constants::REG_ACC, accval >> value);
}

void ALU::comp(sint value) {
    sint accval = controller->getRegisterVal(Constants::REG_ACC);
    if(accval > value) controller->setLastCmp(Constants::CMP_LESS);
    else if(accval < value) controller->setLastCmp(Constants::CMP_GREATER);
    else if(accval == value) controller->setLastCmp(Constants::CMP_EQUAL);
}

sint ALU::countBits(sint value) {
    sint count = 0;
    for(int i = 0; i < 32; ++i) {
        if((value & (1 << i)) != 0) ++count;
    }
    return count;
}
