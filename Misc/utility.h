#ifndef UTILITY_H
#define UTILITY_H

#include <stdlib.h>
#include <stdbool.h>


#ifndef HIGH
#define HIGH 0x1
#endif // !HIGH


#ifndef LOW
#define LOW 0x0
#endif // !LOW


#ifndef BYTE_HIGH
#define BYTE_HIGH 0xFF
#endif // !BYTE_HIGH


typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef volatile unsigned char Register;
typedef unsigned char Port;
typedef unsigned char BitVector8b;

typedef void (*Action)();



/**
 * @brief Reads the state of a port from a register.
 * Returns HIGH if the port is 1, LOW otherwise.
 * 
 * @param reg:   The port's register
 * @param port:  The port's bit (BITx)
 */
#define READ_PORT(reg, bit) \
  (((*reg) & (bit)) ? HIGH : LOW)

      
/**
 * @brief Sets a port HIGH or LOW.
 * 
 * @param reg:   The port's register
 * @param port:  The port's bit (BITx)
 * @param value: The value to set, either HIGH or LOW
 */
#define SET_PORT(reg, bit, value) \
  ((value) ? (reg) |= (bit) : (reg) &= ~(bit))


/** 
 * @brief Switches the state of a port.
 * 
 * @param reg:   The port's register
 * @param port:  The port's bit (BITx)
 */
#define SWITCH_PORT(reg, bit)    \
  ((reg) ^= (bit))
      
/*
 * @brief Raises an event if the pointer to the
 * function is not NULL.
 */
#define RAISE_EVENT(functionPointer)    \
  { if ((functionPointer) != NULL)    \
    (functionPointer)(); }


inline void clamp(ulong *value, ulong min, ulong max) {
  if (*value < min) 
    *value = min;

  else if (*value > max) 
    *value = max;
}


inline bool allLow(BitVector8b vector) {
  return vector == 0;
}

inline bool allHigh(BitVector8b vector) {
  return vector == ~0;
}


#endif // !UTILITY_H