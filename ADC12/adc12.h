#ifndef ADC12_H
#define ADC12_H


#include "io430f5529.h"
#include "utility.h"


/**
 * Waits for the ADC12's to finish 
 * the conversion.
 */
#define ADC12_WAIT_CONVERSION()         \
    while (ADC12CTL1 & ADC12BUSY)       \
        ;


/**
 * Tells whether the converter is converting 
 * or not.
 */
#define IS_ADC12_BUSY()        \
    (ADC12CTL1 & ADC12BUSY)

      
/**
 * Converts a raw value from the ADC12 converter
 * to a temperature.
 */
#define ADC12_GET_TEMPERATURE(rawValue) \
      (rawValue) * 0
        
        
/**
 * Starts the conversion with the 
 * ADC12 converter.
 */        
#define ADC12_START_CONVERSION()        \
        ADC12CTL0 |= ADC12ENC + ADC12SC;
        
        
/**
 * Initializes the ADC12 converter.
 */
void initADC12 (void);


/**
 * Returns the raw temperature converted
 * by the ADC12.
 */ 
int adc12GetRaw(void);


/**
 * Begins the conversion and returns 
 * true when the value is ready, false 
 * otherwise.
 *
 * @param result 
 *      A pointer to an integer that will hold 
 *      the conversion's result.
 */
bool adc12GetRawAsync(unsigned int* result);

#endif // !ADC12_H