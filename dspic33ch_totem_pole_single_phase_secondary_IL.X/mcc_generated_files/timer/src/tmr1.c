/**
 * TMR1 Generated Driver Source File 
 * 
 * @file      tmr1.c
 * 
 * @ingroup   timerdriver
 * 
 * @brief     This is the generated driver source file for TMR1 driver
 *
 * @skipline @version   Firmware Driver Version 1.5.1
 *
 * @skipline @version   PLIB Version 1.5.0
 *
 * @skipline  Device : dsPIC33CH512MP506S1
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

// Section: Included Files
#include "../tmr1.h"
#include "../timer_interface.h"

// Section: Data Type Definitions

#define MASK_32_BIT_LOW 0x0000FFFFU
#define MASK_32_BIT_HIGH 0xFFFF0000U

// Section: File specific functions

static void (*TMR1_TimeoutHandler)(void) = NULL;

// Section: Driver Interface

const struct TIMER_INTERFACE Timer1 = {
    .Initialize            = &TMR1_Initialize,
    .Deinitialize          = &TMR1_Deinitialize,
    .Start                 = &TMR1_Start,
    .Stop                  = &TMR1_Stop,
    #if TIMER_PERIODCOUNTSET_API_SUPPORT
    .PeriodCountSet        = &TMR1_PeriodCountSet,
    #endif
    .PeriodSet             = &TMR1_PeriodSet,
    .PeriodGet             = &TMR1_PeriodGet,
    .CounterGet            = &TMR1_CounterGet,
    .InterruptPrioritySet  = &TMR1_InterruptPrioritySet,
    .TimeoutCallbackRegister = &TMR1_TimeoutCallbackRegister,
    .Tasks          = &TMR1_Tasks
};

// Section: TMR1 Module APIs

void TMR1_Initialize (void)
{
    //TCS FOSC/2; TSYNC disabled; TCKPS 1:1; TGATE disabled; TECS FOSC/2; PRWIP Write complete; TMWIP Write complete; TMWDIS disabled; TSIDL disabled; TON disabled; 
    T1CON = 0x0;
    //TMR 0x0; 
    TMR1 = 0x0;
    //Period 0.1 ms; Frequency 100,000,000 Hz; PR 9999; 
    PR1 = 0x270F;
    
    TMR1_TimeoutCallbackRegister(&TMR1_TimeoutCallback);

}

void TMR1_Deinitialize (void)
{
    TMR1_Stop();
    
    T1CON = 0x0;
    TMR1 = 0x0;
    PR1 = 0x0;
}

void TMR1_Start( void )
{
    // Start the Timer 
    T1CONbits.TON = 1;
}

void TMR1_Stop( void )
{
    // Stop the Timer 
    T1CONbits.TON = 0;
}

void TMR1_PeriodSet(uint32_t count)
{
    PR1 = (uint16_t)(count & MASK_32_BIT_LOW);
}

void TMR1_InterruptPrioritySet(enum INTERRUPT_PRIORITY priority)
{
    IPC0bits.T1IP = priority;
}

void TMR1_TimeoutCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        TMR1_TimeoutHandler = handler;
    }
}

void __attribute__ ((weak)) TMR1_TimeoutCallback( void )
{ 

} 

void TMR1_Tasks( void )    
{
    if(IFS0bits.T1IF == 1)
    {
        (*TMR1_TimeoutHandler)();
        IFS0bits.T1IF = 0;
    }
}

void TMR1_PeriodCountSet(size_t count)
{
    PR1 = count & MASK_32_BIT_LOW;
}

/**
 End of File
*/
