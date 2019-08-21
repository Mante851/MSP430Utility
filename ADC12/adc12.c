#ifndef ADC12_TEMPERATURE_C
#define ADC12_TEMPERATURE_C


#include "io430f5529.h"
#include "adc12.h"


/**
 * Initializes the ADC12 converter.
 */
void initADC12 (void)
{
    // Enable temperature sensor. REFMASTER = 1 (default), REFON = 1
    REFCTL0 |= REFON;
    
    /*
     * ADC12SHT0_x--12bit ADC Sample Hold Time, 
     * ADC12ON--12bit ADC ON/OFF--setting this bit to "1" 
     * (denoted by the label ADC12ON) turns on the ADC
     */
    ADC12CTL0 = 
        ADC12SHT0_8 + ADC12ON;              
    
    /* 
     * ADC12SHP-- the sampling signal (SAMPCON) is sourced from the sampling timer
     * ADC12CONSEQ_0-- Single-channel, single-conversion
     */
    ADC12CTL1 = 
        ADC12SHP + ADC12CONSEQ_0;           
    
    /* 
     * 4 bit section determines which of the possible input channels the ADC will
     * actually convert and store into the ADC12MEM0 register.
     * Default voltage references  (3.3 Volt): Vr- = AVss, Vr+ = AVcc
     */
    ADC12MCTL0 |= ADC12INCH_10;
    
    /*
     * ADC12ENC-- 12bit ADC Enable Conversion-- locks in the ADC settings 
     * and stabilizes it for conversion.
     * ADC12SC--12bit ADC Start Conversion-- setting this bit to one 
     * actually triggers the ADC's conversion cycle.
     */
    ADC12CTL0 |= 
        ADC12ENC + ADC12SC;
  
    ADC12_WAIT_CONVERSION();
}


/**
 * Returns the raw temperature converted
 * by the ADC12.
 */ 
int adc12GetRaw(void)
{
    // Start conversion
    ADC12_START_CONVERSION();
    ADC12_WAIT_CONVERSION();
    
    return ADC12MEM0;
}


/**
 * Begins the conversion and returns 
 * true when the value is ready, false 
 * otherwise.
 *
 * @param result 
 *      A pointer to an integer that will hold 
 *      the conversion's result.
 */
bool adc12GetRawAsync(unsigned int* result)
{
    static bool _isConverting   = false;
    
    if (!_isConverting)
    {
        ADC12_START_CONVERSION();
        _isConverting = true;
    }
      
    if (IS_ADC12_BUSY())
        return false;
    
    *result = ADC12MEM0;
    _isConverting = false;
    return true;
}


#endif // !ADC12_TEMPERATURE_C