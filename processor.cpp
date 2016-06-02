#include "processor.h"
#include "constants.h"
#include "stdint.h"

#define NOP 0x00
#define MOV 0x10
#define HLT 0x20
#define CALL 0x30
#define RET 0x40

Processor::Processor()
{

}

void Processor::runProgram() {
    controller.init();
    while(controller.getRegisterVal(Constants::REG_PC) < controller.getRegisterVal(Constants::REG_DPT)) {
        sint instruction = controller.fetchInstruction(); //fetch the next instruction
        sint normalized_inst = instruction & 0xfffffff0; //remove the instruction mode
        sint instruction_mode = instruction - normalized_inst; //get the instruction mode (i.e. how to treat the address)
        switch(normalized_inst) {
            case MOV:
            {
                sint argument = controller.getRegisterVal(Constants::REG_ARG);
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
                controller.setRegisterVal(reg2,controller.getRegisterVal(reg1)); //move the value
            }
                break;
            case HLT:
                return; //program should terminate -> exit the execution procedure
            case CALL:
                controller.callFunction();
                break;
            case RET:
                controller.returnFunction();
                break;
        }
    }
}
