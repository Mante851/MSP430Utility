

#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include "utility.h"

/**
 * Stores infos used by the readDebounce function.
 */
typedef struct DebounceInfo
{
    // The IN register's pointer for the 
    // button.
    volatile unsigned char* buttonInRegister;
    
    // The button's port.
    unsigned char buttonPort;
    
    /*
     * Raised when the button is pressed.
     */
    void (*buttonReleased)(void);
    
    /*
     * Raised when the button is released.
     */
    void (*buttonPressed)(void);
    
    // Stores this port's readings.
    unsigned char _readings;
    
    // The state of this switch.
    State _state;
} DebounceInfo;


/**
 * Initializes a DebounceSettings struct.
 *
 * @param buttonInRegister
 *      The button's IN register (PxIN) address, 
 *      casted to a char pointer.        
 *
 * @param buttonPort 
 *      The button's port (BITx)
 * 
 * @param buttonReleased
 *      A pointer to a callback function for when 
 *      the button is released. Can be NULL.
 *
 * @param buttonPressed
 *      A pointer to a callback function for when 
 *      the button is pressed. Can be NULL.
 *
 * @param initialState
 *      The initial state of the button, either pressed
 *      or released.
 *
 * @param raiseOnCreate
 *      Determines if the event that's relative to the
 *      initial state should be raised or not.
 *
 * @returns
 *      A pointer to the newly allocated struct.
 */
DebounceInfo* createDebounceInfo(
                       volatile unsigned char* buttonInRegister,
                       unsigned char           buttonPort,
                       void (*buttonReleased)(void),
                       void (*buttonPressed) (void),
                       State initialState
                         );


/**
 * Reads the state of a button, represented by a 
 * DebounceInfo structure, by applying a debounce 
 * to it first.
 *
 * @returns 
 *      The state of the button, PRESSED (0x00), RELEASED (0xFF), 
 *      JUST_PRESSED (0x01) or JUST_RELEASED (0x02).
 */
State readDebounce(DebounceInfo* debounceInfo);

#endif