#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H


#ifndef SEG_DATA
#define SEG_DATA 0xFF
#endif // !SEG_DATA


#ifndef SEG_DATA_OFF
/**
 * @brief Turns off a display's segments.
 */
#define SEG_DATA_OFF(segInfo)                   \
    (segInfo)->segmentsRegister &= ~SEG_DATA
#endif // !SEG_DATA_OFF


/**
 * @brief Holds informations about a 
 * sevent segment display.
 *
 * Utilizes all bits of a port:
 *  - BIT0 to BIT6 for segments
 *  - BIT7 for the dot
 */
typedef struct SevenSegmentInfo 
{
    /**
     * @brief The display's port used to control
     * the segments.
     * gfedcba -> BIT6 to BIT0
     */
    volatile unsigned char* segmentsRegister;
    
    /**
     * @brief The direction register used to control
     * the display.
     */
    volatile unsigned char* segmentsDirRegister;
} SevenSegmentInfo;


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
             volatile unsigned char* segmentsDirRegister);


/**
 * @brief Writes a number to a seven segment display.
 */
inline void sevenSegWrite(SevenSegmentInfo* segInfo, unsigned char value);


/**
 * @brief Turns the display's dot ON or OFF.
 */
inline void sevenSegDot(SevenSegmentInfo* segInfo, char state);
 
/**
 * @brief Converts an hexadecimal value to the
 * representation used by a seven segment display.
 *
 * Only the last four bits are converted.
 */
unsigned char hexToSevenSeg(unsigned char value);


/**
 * @brief Converts a decimal value to it's 
 * hexadecimal representation.
 */
unsigned int decToHex(register unsigned int value);

#endif // !SEVEN_SEGMENT_H