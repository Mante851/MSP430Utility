#ifndef I2C_C
#define I2C_C


#include "io430f5529.h"


/**
 * Initializes I2C mode for the USCI.
 */
void initI2C(int slaveAddress)
{
    // USCI in reset mode
    UCB0CTL1 |= UCSWRST; 

    UCB0CTL0 |= 
        UCMST    +      // I2C master
        UCMODE_3 +      // USCI in I2C mode
        UCSYNC;         // Synchronous mode
    
    UCB0I2CSA = slaveAddress;
    
    // Disabling reset mode
    UCB0CTL1 &= ~UCSWRST; 
    
    UCB0CTL1 |= 
        ~UCTR   +       // Receiver mode
         0;       // Sending a START condition  
        
    // UCTXSTT is reset when the slave
    // acknowledges the START
    while (UCB0CTL1 & UCTXSTT)
        ;
}


#endif // !I2C_C