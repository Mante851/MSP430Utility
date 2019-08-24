#include "io430f5529.h"
#include "serial.h"


int main(void)
{   
    WDTCTL = WDTPW + WDTHOLD;
    
    __enable_interrupt();
    Serial.begin(BAUD_9600);
    
    byte data[16];
    while (true)
    {
        uint len = 
            Serial.readUntil(data, 16, '\n');

        Serial.writeBuff(data, len);
    }
    
    // Il metodo writeBuffAsync(...) non funziona,
    // stessa cosa per writeAsync(...)
    /* 
    while (true)
    {
        Serial.read(data, 4);
        Serial.writeBuffAsync(data, 4);
    }
    /**/
}