#ifndef TIMER_C
#define TIMER_C


#include "timer.h"
#include "utility.h"
#include "linkedList.h"
#include <stdlib.h>


/**
 * @brief A list of TimerInfos managed by 
 * the interrupt. 
 */
Node* __timers = NULL;


/**
 * @brief Creates a timer.
 *
 * @param interval 
 *      The number of interrupt calls to wait before
 *      the callback is invoked.
 *
 * @param autoReset
 *      Tells whether the timer should automatically reset or not.
 *
 * @param elapsed 
 *      A pointer to a callback function.
 */
TimerInfo* createTimerInfo(unsigned int interruptCalls, bool autoReset, void (*elapsed)(void))
{
    TimerInfo* result = 
        (TimerInfo*)malloc(sizeof(TimerInfo));
    
    result->interruptCalls = interruptCalls;
    result->autoReset      = autoReset;
    result->elapsed        = elapsed;
    result->_calls         = 0;
    
    return result;
}


/**
 * @brief Initializes the timer 0.
 */ 
void initTimer0(unsigned int ccr0Delay)
{
    TA0CTL = 
        TASSEL_2 + // Select TAR Clock source = smclk
        MC_1     + // Count mode up
        TACLR;
  
    TA0CCR0 = ccr0Delay;    // Set timer period
          
    TA0CTL &= ~TAIFG;  // Clear TAIFG flag
    TA0CTL |=  TAIE;    // Enable TAIFG Interrupt
    
    // Initializing timers list
    if (__timers == NULL)
        __timers = createNode(NULL, NULL, NULL);
}


/**
 * @brief Add a TimerInfo struct to the managed
 * timers list.
 * @returns The new timer.
 */
inline void addManagedTimer(TimerInfo* timer)
{
    // Adding the timer to the
    // managed ones.
    llAdd(__timers, timer);
}


/**
 * @brief Clears the managed timers list.
 */
inline void clearManagedTimers(void)
{
    llClear(&__timers, true);
    
    // Re-initializing timers list
    __timers = createNode(NULL, NULL, NULL);
}


/**
 * @brief Removes a timer from the managed ones.
 */
inline void removeManagedTimer(TimerInfo* timer, bool freeContent)
{
    if (__timers == NULL) return;
  
    register Node* current;
    for (
         current = __timers->next;
         current != NULL;
         current = current->next)
    {
        if (current->content == timer)
        {
            llRemove(&__timers, current, freeContent);
            return;
        }
    }
}

/**
 * @brief Interrupt called at each tick of the
 * hardware timer (timer 0). Checks the timers in the
 * timers linked list, and manages their callbacks.
 */
__interrupt void __checkTimersCallback()
{
    // Re-enabling the interrupt
    TA0CTL &= ~TAIFG;
    
    register Node* current;
    for (
         current = __timers->next;
         current != NULL; 
         current = current->next)
    {
        // Getting the timer
        TimerInfo* timer = LL_GET_TIMER_INFO(current);
        
        // If the delay is reached
        if ((timer->_calls)++ == timer->interruptCalls)
        {
            // Calling the callback
            RAISE_EVENT(timer->elapsed);
            
            // Resetting the timer or deleting it
            if (timer->autoReset)
                timer->_calls = 0;
            
            else
                llRemove(&__timers, current, true);
        }   
    }
}

#endif // !TIMER_C