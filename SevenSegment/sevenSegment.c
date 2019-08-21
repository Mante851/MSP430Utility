#ifndef SEVEN_SEGMENT_C
#define SEVEN_SEGMENT_C

#include "sevenSegment.h"
#include <stdlib.h>

/**
 * @brief Allocates a SevenSegmentInfo structure
 * and returns a pointer to it.
 *
 * @param segmentsRegister 
 *      The display's port used to control
 *      the segments.
 *      gfedcba -> BIT6 to BIT0
 *
 * @param segmentsDirSegment
 *      The direction register used to control
 *      the display.
 */
SevenSegmentInfo* createSevenSegmentInfo(
             volatile unsigned char* segmentsRegister,
             volatile unsigned char* segmentsDirRegister)
{
    SevenSegmentInfo* result = 
        (SevenSegmentInfo*)malloc(sizeof(SevenSegmentInfo));
    
    result->segmentsRegister    = segmentsRegister;
    result->segmentsDirRegister = segmentsDirRegister;
    
    *segmentsRegister    &= ~SEG_DATA; // Segments off
    *segmentsDirRegister |=  SEG_DATA; // Segments dir on output
    
    return result;
}


/**
 * @brief Writes a digit to a seven segment display.
 */
inline void sevenSegWrite(SevenSegmentInfo* segInfo, unsigned char value)
{
    *(segInfo->segmentsRegister) = hexToSevenSeg(value);
}
    

/**
 * @brief Converts an hexadecimal value to the
 * representation used by a seven segment display.
 *
 * Only the last four bits are converted.
 */
unsigned char hexToSevenSeg(unsigned char value)
{
    static unsigned char segValues[]=
    { 
        0x40,0x79,0x24,0x30,
        0x19,0x12,0x02,0x78,
        0x00,0x10,0x08,0x03,
        0x46,0x21,0x06,0x0E
    };

    return ~segValues[value & 0x0F];
}


/**
 * @brief Converts a decimal value to it's 
 * hexadecimal representation.
 */
inline unsigned int decToHex(register unsigned int num)
{
    unsigned int mig, cent, dec, uni;
    // num = 1987
    mig  = num / 1000; //   1
    num  = num % 1000; // 987

    cent = num / 100; //  9
    num  = num % 100; // 87
    
    dec = num / 10;   // 8
    uni = num % 10;   // 7
    
    return 
        (mig << 12)|(cent << 8)|(dec << 4)| uni ; // 0x1987
}
    
    
#endif // !SEVEN_SEGMENT_C