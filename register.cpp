#include "register.h"

Register::Register()
{
    value = 0;
}

sint Register::getValue() {
    return value;
}

void Register::setValue(sint newval) {
    value = newval;
}
