#ifndef SERIAL_H_
#define SERIAL_H_

#include "io430f5529.h"
#include "utility.h"
#include "circularBuffer.h"


/**
 * @brief 
 * In order to read:
 * UCBR  = (BAUD_xxxx >> 8);
 * UCBRS = (byte)(BAUD_xxxx);
 */
typedef enum BaudRate 
{
    //               UCBR      UCBRS
    BAUD_9600   = (104 << 8) | UCBRS_1,
    BAUD_19200  = (52  << 8) | UCBRS_0,
    BAUD_38400  = (26  << 8) | UCBRS_0,
    BAUD_57600  = (17  << 8) | UCBRS_3,
    BAUD_115200 = (8   << 8) | UCBRS_6
} BaudRate;


#define SERIAL_DATA_RECEIVED() (UCA1IFG & UCRXIFG)
#define SERIAL_IS_AVAILABLE()  (UCA1IFG & UCTXIFG)
#define SERIAL_TX_ENABLED()    (UCA1IE  &  UCTXIE)
#define SERIAL_ENABLE_TX()     (UCA1IE |=  UCTXIE)
#define SERIAL_DISABLE_TX()    (UCA1IE &= ~UCTXIE)


/**
 * @brief 
 * Executes the give code while
 * the transmission is locked.
 */
#define SERIAL_LOCK_TX(expr)  \
    {                         \
        SERIAL_ENABLE_TX();   \
        expr                  \
        SERIAL_DISABLE_TX();  \
    }                


/**
 * @brief 
 * Execute the given code while
 * the USCI interface is in reset
 * mode.
 */
#define SERIAL_RESET(expr)      \
    {                           \
        UCA1CTL1 |= UCSWRST;    \
        expr                    \
        UCA1CTL1 &= ~UCSWRST;   \
    }


/**
 * @brief 
 * Computes the UCBRSx and UCBRx values
 * for the given frequency and baudRate
 * 
 * Useful links for baud rate calculation:
 * - https://forum.43oh.com/topic/2640-uart-configurator/
 * - http://processors.wiki.ti.com/index.php?title=USCI_UART_Baud_Rate_Gen_Mode_Selection&oldid=122242
 * - http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * - http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/51342?UART-setup-with-UCOS16-1-generates-framing-errors-on-MSP430F2618
 */
void computeUCBR(
    float freq, 
    float baudRate, 
    byte *ucbr0, 
    byte *ucbr1, 
    byte *ucbrs,
    bool  useUCOS16Oversampling);


typedef struct SerialPort
{ 
//\
Public:

    /**
     * @brief 
     * The size of the rx and tx buffers,
     * in bytes.
     */
    uint buffSize;

    /**
     * @brief 
     * Initializes the serial port.
     */
    void (*const begin)(BaudRate baudRate);

    /**
     * @brief 
     * Frees the underlying buffers.
     */
    void (*const close)(void);

    /**
     * @brief 
     * Reads a character from the rx buffer.
     * 
     * @param data: The character read.
     * @returns:
     *      True if the character was read,
     *      false if the rx buffer was empty.
     */
    bool (*const readCharAsync)(byte *data);

    /**
     * @brief 
     * Blocks until a character is received on 
     * the rx buffer.
     * 
     * @param data: The character read.
     */
    void (*const readChar)(byte *data);
    
    /**
     * @brief 
     * Reads the given number of bytes
     * from the input buffer.
     * 
     * @param buffer: The array that will contain the result.
     * @param bytes:  The number of bytes to read (buffer size or less).
     */
    void (*const read)(byte *buffer, uint bytes);
    

    /**
     * @brief 
     * Writes a character to the tx buffer.
     * 
     * @param data: The character to write.
     * @returns:
     *      True if the tx buffer is not full,
     *      false otherwise.
     */
    bool (*const writeCharAsync)(const byte data);

    /**
     * @brief 
     * Writes a character to the tx buffer and waits
     * for it to be sent.
     */
    void (*const writeChar)(const byte data);

    /**
     * @brief
     * Writes a given message to the output buffer in a string
     * format (NULL terminated char array)
     * 
     * The function blocks until all the characters
     * have been sent.
     * 
     * @param data: The message to send.
     */
    void (*const write)(const byte *data);

    /**
     * @brief
     * Writes a given message to the output buffer in a string
     * format (NULL terminated char array)
     * 
     * @param data: The message to send.
     * @returns:
     *      The bytes actually written to the tx buffer.
     */
    uint (*const writeAsync)(const byte *data);

    /**
     * @brief
     * Writes a given message to the output buffer.
     * Blocks until all the bytes have been sent.
     * 
     * @param data: 
     *      The bytes to write.
     *
     * @param length:  
     *      The number of bytes to write (message length or less)
     *
     * @returns
     *      The number of bytes actually written.
     */
    void (*const writeBuff)(const byte *data, uint length);

    /**
     * @brief
     * Writes a given message to the output buffer.
     * 
     * @param data: 
     *      The bytes to write.
     *
     * @param length:  
     *      The number of bytes to write (message length or less)
     *
     * @returns
     *      The number of bytes actually written.
     */
    uint (*const writeBuffAsync)(const byte *data, uint length);

    /**
     * @brief 
     * Reads the input buffer until it finds the 
     * given token.
     * 
     * @param buffer:     The buffer that will hold the message.
     * @param size:       The size of the given buffer.
     * @param terminator: The token used as line terminator.
     * @returns:
     *      The number of bytes actually read.
     */
    uint (*const readUntil)(
        byte       *buffer, 
        uint        size, 
        const byte  terminator);

//\
Private:
    CircularBuffer _tx;
    CircularBuffer _rx;

} SerialPort;


void _begin(BaudRate baudRate);
void _close(void);

bool _readCharAsync (      byte *data);
bool _writeCharAsync(const byte  data);

void _readChar (      byte *data);
void _writeChar(const byte  data);

void _read      (byte *buffer, uint bytes);
void _write     (const byte *data);
uint _writeAsync(const byte *data);

void _writeBuff     (const byte *data, uint length);
uint _writeBuffAsync(const byte *data, uint length);
uint _readUntil(
    byte       *buffer, 
    uint        size, 
    const byte  terminator);


extern SerialPort Serial;


#endif // !SERIAL_H_