#include "serial.h"
#include <math.h>


inline void computeUCBR(
    float freq, 
    float baudRate, 
    byte *ucbr0, 
    byte *ucbr1, 
    byte *ucbrs,
    bool  useUCOS16Oversampling)
{
    byte div = 
        useUCOS16Oversampling ? 16 : 1;
        
    float val = 
        freq / baudRate / div;

    *ucbr0 = (byte)val % 256; 
    *ucbr1 = (byte)val / 256; 
    *ucbrs = (byte)round((val - (uint)val) * 8);
}


SerialPort Serial =
{
    16,
    &_begin,
    &_close,

    &_readCharAsync,
    &_readChar,
    &_read,

    &_writeCharAsync,
    &_writeChar,
    &_write,
    &_writeAsync,

    &_writeBuff,
    &_writeBuffAsync,
    &_readUntil
};


void _begin(BaudRate baudRate)
{
    // P3SEL |= BIT3(tx) + BIT4(rx);
    
    // P4.4,5 = USCI_A1 TXD/RXD
    P4SEL |= BIT4 + BIT5;   

    SERIAL_RESET
    (
        UCA1CTL1 |= UCSSEL_2; // SMCLK
        UCA1MCTL |= UCBRF_0;  // Modulation UCBRFx=0
        
        // Baud rate
        UCA1BR0   = baudRate >> 8;    
        UCA1BR1   = 0;    
        UCA1MCTL |= (byte)baudRate;
    );

    // Enable USCI_A0 RX interrupt
    UCA1IE |= UCRXIE;

    cbInit(&Serial._tx, Serial.buffSize);
    cbInit(&Serial._rx, Serial.buffSize);
}


inline void _close()
{
    cbFree(&Serial._tx);
    cbFree(&Serial._rx);
}


inline bool _readCharAsync(byte *data)
{
    return cbRead(&Serial._rx, data);
}


inline void _readChar(byte *data)
{
    while (!_readCharAsync(data))
        ;
}


inline void _read(byte *buffer, uint bytes)
{
    for (; bytes > 0; --bytes)
        _readChar(buffer++);
}


bool _writeCharAsync(const byte data)
{
    bool ret;
    SERIAL_LOCK
    (
        ret = 
            cbWrite(&Serial._tx, data);
    );
    return ret;
}


void _writeChar(const byte data)
{
    while (!_writeCharAsync(data))
        ;

    // Waiting for byte to be sent.
    while(!(UCA1IFG & UCTXIFG))
        ;
}


inline void _write(const byte *data)
{
    while(*data)
        _writeChar(*data++);
}


uint _writeAsync(const byte *data)
{
    int i;
    SERIAL_LOCK
    (
        for(i = 0; data[i]; ++i)
            if(!cbWrite(&Serial._tx, data[i]))
                break;
    );
    return i;
}


inline void _writeBuff(const byte *data, uint length)
{
    while(length > 0)
        _writeChar(*data++);
}


uint _writeBuffAsync(const byte *data, uint length)
{
    int i;
    SERIAL_LOCK
    (
        for(i = 0; i < length; ++i)
            if(!cbWrite(&Serial._tx, data[i]))
                break;
    );
    return i;
}


uint _readUntil(
    byte       *buffer, 
    uint        size, 
    const byte  terminator) 
{
    int i;
    for (i = 0; i < size; i++)
    {
        _readChar(buffer + i);
        if (buffer[i] == terminator)
            break;
    }

    return i;
}


#pragma vector = USCI_A1_VECTOR
__interrupt void __serial_interrupt(void)
{
    static byte data;
  
    // Reading
    if(UCA1IFG & UCRXIFG)                    
        cbWrite(&Serial._rx, UCA1RXBUF);

    // Writing
    if((UCA1IFG & UCTXIFG) && (UCA1IE & UCTXIE))
    {	
    	if(cbIsEmpty(&Serial._tx)) 
            return;

        // Data to send
    	cbRead(&Serial._tx, &data);
        UCA1TXBUF = data;
    }
}