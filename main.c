// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:         
// Authors:      
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "interrupt.h"
#include "switch.h"
#include "timer.h"
#include "config.h"

#define ON 0
#define OFF 1
#define PRESS 0
#define RELEASE 1
#define led1 LATGbits.LATG12
#define led2 LATGbits.LATG14

typedef enum stateTypeEnum{
    RUN, STOP, debouncePress, debounceRelease
} stateType;

volatile stateType state;
volatile stateType lastState;
volatile int temp;
/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    enableInterrupts();
    initLEDs();
    initSW2();
    state = RUN;
    
    while(1)
    {
        switch(state)
        {
            case RUN:
                led1 = ON;
                led2 = OFF;
                lastState = RUN;
                break;
            case STOP:
                led1 = OFF;
                led2 = ON;
                lastState = STOP;
                break;
            case debouncePress:
                delayMs(5);
                if(lastState==RUN)
                    state = STOP;
                else if(lastState==STOP)
                    state = RUN;
                else
                    state = RUN;
                break;
            case debounceRelease:
                delayMs(5);
                state = lastState;
                break;
        }
    }
    
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{
    //TODO: Implement the interrupt to capture the press of the button
    
    temp = PORTGbits.RG13;
    IFS1bits.CNGIF = 0;
    if(temp==PRESS)
        state = debouncePress;
    else if(temp==RELEASE)
        state = debounceRelease;
    else
        state = lastState;
    temp = -1;
    
    /*
    temp = PORTDbits.RD6;
    IFS1bits.CNDIF = 0;
     */
}
