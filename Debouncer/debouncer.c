#ifndef DEBOUNCER_C
#define DEBOUNCER_C

#include "debouncer.h"
#include "utility.h"
#include <stdlib.h>

/**
 * createDebounceInfo implementation
 */
DebounceInfo* createDebounceInfo(
                       volatile unsigned char* buttonInRegister,
                       unsigned char           buttonPort,
                       void (*buttonReleased)(void),
                       void (*buttonPressed) (void),
                       State initialState
                         )
{
    // Allocating the DebounceInfo struct
    DebounceInfo* result = 
        (DebounceInfo*)malloc(sizeof(DebounceInfo));
    
    result->buttonInRegister = buttonInRegister;
    result->buttonPort       = buttonPort;
    result->buttonPressed    = buttonPressed;
    result->buttonReleased   = buttonReleased;
    
    // Initial state
    result->_readings = initialState;
    result->_state    = initialState;
    
    return result;
}


/**
 * readDebounce implementation
 */
State readDebounce(DebounceInfo* dbInfo)
{
    // Left shift of the bits
    dbInfo->_readings <<= 1;
    
    // Reading the state of the 
    // button
    dbInfo->_readings |= 
        READ_PORT(
            *(dbInfo->buttonInRegister), 
              dbInfo->buttonPort);  

    // If the button is surely released
    if (dbInfo->_readings == RELEASED)
    {
        if (dbInfo->_state != RELEASED)
        {
            dbInfo->_state = RELEASED;
            RAISE_EVENT(dbInfo->buttonReleased);
            return JUST_RELEASED;
        }
    }
    
    // If the button is surely pressed
    else if (dbInfo->_readings == PRESSED)
    {
        if (dbInfo->_state != PRESSED)
        {
            dbInfo->_state = PRESSED;
            RAISE_EVENT(dbInfo->buttonPressed);
            return JUST_PRESSED;
        }
    }
    
    return dbInfo->_state;
}

#endif // !DEBOUNCER_C