#include "io430f5529.h"
#include "serial.h"


int main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;
    
    __enable_interrupt();
    Serial.begin(BAUD_9600);
    
    byte data[4];
    while (true)
    {
        if(Serial.readCharAsync(data))
            Serial.writeCharAsync(*data);
    }
    
    while (true)
    {
        Serial.read(data, 4);
        Serial.writeBuffAsync(data, 4);
    }
}