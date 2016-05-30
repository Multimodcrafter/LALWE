#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "stdint.h"
#include "register.h"
#include "ram.h"
#define sint int32_t

class Controller
{
public:
    Controller();

    /** returns the currently stored value in the specified register*/
    sint getRegisterVal(sint);

    /** stores the given value in the specified register*/
    void setRegisterVal(sint,sint);

    /** loads a value directly from ram into the IN register*/
    void loadRamValDir(sint);

    /** loads a value indirectly from ram into the IN register using the IND1 or IND2 register*/
    void loadRamValInd(sint,sint);

    /** stores a value in ram at the specified location*/
    void storeRamVal(sint);

    /** calculates the effective address out of the given relative address,
     * using the specified mode (Global, Local, Parameter)
     */
    sint calcAddress(sint,sint);

    /** fetches the value at the first ram slot and puts it into the PC register.
     * This is the initialization needed for the program to run correctly
     */
    void loadEntryPoint();

    /** fetches the next instruction and increases the PC*/
    sint fetchInstruction();

    /** performs the necessary steps to call a function. This includes:
     * Storing the parameters on the stack in reverse order
     * Storing the return address on the stack
     * Setting the PC to the function location
     * Changing the frame- and stackpointer apropriately
     */
    void callFunction();

private:
    Register *registers;

    RAM ram;

};

#endif // CONTROLLER_H
