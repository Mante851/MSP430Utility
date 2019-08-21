#include "io430f5529.h"
#include "serial.h"


int main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;
    
    P1DIR |= BIT0;
    
    __enable_interrupt();
    Serial.begin(BAUD_9600);
    
    byte data;
    while (true)
    {
        P1OUT ^= BIT0;
      
        Serial.readChar(&data);
        Serial.writeCharAsync(data);
    }
}