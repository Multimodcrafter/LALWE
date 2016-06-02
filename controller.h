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
     * if the mode is local, the function checks wether the address is in the range of the stack pointer
     * if not, it is increased apropriately
     */
    sint calcAddress(sint,sint);

    /** fetches the value at the first ram slot and puts it into the PC register.
     * fetches the value at the second ram slot and puts it into the DataPointer register
     * sets the frame pointer and stackpointer to the end of the ram
     * This is the initialization needed for the program to run correctly
     */
    void init();

    /** fetches the next instruction and increases the PC*/
    sint fetchInstruction();

    /** performs the necessary steps to call a function. This includes:
     * Storing the parameters on the stack in reverse order
     * Storing the return address on the stack
     * Storing the previous stack and frame pointer values for returning
     * Setting the PC to the function location
     * Changing the frame- and stackpointer apropriately
     */
    void callFunction();

    /** performs the necessary steps to return from a function. This includes:
     * Setting the PC to the return address
     * Setting the stack and frame pointer to the previous value
     */
    void returnFunction();

private:
    Register *registers;

    RAM ram;

};

#endif // CONTROLLER_H
