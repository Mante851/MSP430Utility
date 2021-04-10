#include "debouncer.h"
#include "utility.h"
#include <stdlib.h>



static void _updateReadings(Button *button);
static void _updateState(Button *button);
static void _raiseEvents(Button *button);


Button* createButton(
  Register* buttonIn,
  Port buttonPort,
  Action buttonReleased,
  Action buttonPressed,
  ButtonState initialState
) {
  Button* result = 
    (Button*)malloc(sizeof(Button));
  
  result->buttonIn = buttonIn;
  result->buttonPort = buttonPort;
  result->buttonPressed = buttonPressed;
  result->buttonReleased = buttonReleased;
  
  result->_readings = initialState;
  result->_state = initialState;
  
  return result;
}


ButtonState readDebounce(Button* button) {
  _updateReadings(button);
  _updateState(button);
  _raiseEvents(button);

  return button->_state;
}


static void _updateReadings(Button *button) {
  BitVector8b *readings = &(button->_readings);
  
  *readings <<= 1;
  *readings |= READ_PORT(
    button->buttonIn, 
    button->buttonPort
  );
}

static void _updateState(Button *button) {
  const BitVector8b readings = button->_readings;
  ButtonState *state = &(button->_state);

  if (allHigh(readings)) {
    *state = *state != RELEASED
      ? JUST_RELEASED
      : RELEASED;
  }
  else if (allLow(readings)) {
    *state = *state != PRESSED
      ? JUST_PRESSED
      : PRESSED;
  }
}

static void _raiseEvents(Button *button) {
  switch (button->_state) {
    case JUST_PRESSED:  RAISE_EVENT(button->buttonPressed);  break;
    case JUST_RELEASED: RAISE_EVENT(button->buttonReleased); break;
  }
}