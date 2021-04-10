

#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include "utility.h"


typedef enum ButtonState {
  PRESSED = 0x00,
  JUST_PRESSED ,
  JUST_RELEASED,
  RELEASED = 0xFF,
} ButtonState;


typedef struct Button
{
  Register* buttonIn;
  Port buttonPort;
  
  Action buttonReleased;
  Action buttonPressed;
  
  BitVector8b _readings;
  ButtonState _state;
} Button;


/**
 * Initializes a DebounceSettings struct.
 *
 * @param buttonIn
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
Button* createButton(
  Register* buttonInRegister,
  Port buttonPort,
  Action buttonReleased,
  Action buttonPressed,
  ButtonState initialState
);


/**
 * Reads the state of a button, represented by a 
 * Button structure, by applying a debounce 
 * to it first.
 *
 * @returns 
 *      The state of the button, PRESSED (0x00), RELEASED (0xFF), 
 *      JUST_PRESSED (0x01) or JUST_RELEASED (0x02).
 */
ButtonState readDebounce(Button* debounceInfo);

#endif