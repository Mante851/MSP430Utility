#ifndef TIMER_H
#define TIMER_H


#include "utility.h"
#include "linkedList.h"
#include "io430f5529.h"


#ifndef TIMER0_MS
/**
 * @brief The timer0 ccr0 value to 
 * wait for a millisecond.
 */
#define TIMER0_MS 0x400
#endif // !TIMER0_MS


/**
 * @brief Represents a timer.
 */
typedef struct TimerInfo
{
    /**
     * @brief The number of interrupt calls to wait before
     * the callback is invoked.
     */
    unsigned int interruptCalls;
    
    /**
     * Tells whether the timer should 
     * reset between each call.
     */
    bool autoReset;
     
    /**
     * A callback called every time the delay
     * is elapsed.
     */
    void (*elapsed)(void);
    
    /**
     * @brief The currently invoked calls.
     */
    unsigned int _calls;
    
} TimerInfo;


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
TimerInfo* createTimerInfo(
               unsigned int interruptCalls, 
               bool autoReset, 
               void (*elapsed)(void));


/**
 * @brief Add a TimerInfo struct to the managed
 * timers list.
 */
void addManagedTimer(TimerInfo* timer);


/**
 * @brief Clears the managed timers list.
 */
void clearManagedTimers(void);


/**
 * @brief Removes a timer from the managed ones.
 */
void removeManagedTimer(TimerInfo* timer, bool freeContent);


/**
 * @brief Returns a TimerInfo pointer 
 * from the content of a Node*
 */
#define LL_GET_TIMER_INFO(node) \
    (TimerInfo*)((node)->content)

      
/**
 * @brief Initializes the timer 0.
 */ 
void initTimer0(unsigned int ccr0Delay);


#endif // !TIMER_H