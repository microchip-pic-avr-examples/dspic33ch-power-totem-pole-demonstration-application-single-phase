/**
 * SCCP2-TIMER Generated Driver Source File
 * 
 * @file      sccp2.c
 * 
 * @ingroup   timerdriver
 * 
 * @brief     This is the generated driver source file for SCCP2-TIMER driver
 *
 * @skipline @version   Firmware Driver Version 1.5.1
 *
 * @skipline @version   PLIB Version 1.6.0
 *
 * @skipline  Device : dsPIC33CH512MP506
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

#include <stddef.h> 
#include "../sccp2.h"
#include "../timer_interface.h"

// Section: Data Type Definitions

#define MASK_32_BIT_LOW 0x0000FFFFU
#define MASK_32_BIT_HIGH 0xFFFF0000U

// Section: File specific functions

static void (*SCCP2_TimeoutHandler)(void) = NULL;

// Section: Driver Interface

// Defines an object for TIMER_INTERFACE

const struct TIMER_INTERFACE SPI_Timeout_40us_Timer = {
    .Initialize     = &SCCP2_Timer_Initialize,
    .Deinitialize   = &SCCP2_Timer_Deinitialize,
    .Start          = &SCCP2_Timer_Start,
    .Stop           = &SCCP2_Timer_Stop,
    #if TIMER_PERIODCOUNTSET_API_SUPPORT
    .PeriodCountSet = &SCCP2_Timer_PeriodCountSet,
    #endif
    .PeriodSet      = &SCCP2_Timer_PeriodSet,
    .CounterGet     = &SCCP2_Timer_CounterGet,
    .PeriodGet	    = &SCCP2_Timer_PeriodGet,
    .InterruptPrioritySet = &SCCP2_Timer_InterruptPrioritySet,
    .TimeoutCallbackRegister = &SCCP2_Timer_TimeoutCallbackRegister,
    .Tasks          = &SCCP2_Timer_Tasks,
};

// Section: Driver Interface Function Definitions

void SCCP2_Timer_Initialize(void)
{
    // MOD ; CCSEL disabled; TMR32 16 Bit; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; CCPSLP disabled; CCPSIDL disabled; CCPON disabled; 
    CCP2CON1L = 0x0; //The module is disabled, till other settings are configured
    //SYNC None; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; IOPS Each Time Base Period Match; RTRGEN disabled; OPSRC Timer Interrupt Event; 
    CCP2CON1H = 0x0;
    //ASDG 0x0; SSDG disabled; ASDGM disabled; PWMRSEN disabled; 
    CCP2CON2L = 0x0;
    //ICSEL ; AUXOUT Disabled; ICGSM Level-Sensitive mode; OCAEN disabled; OENSYNC disabled; 
    CCP2CON2H = 0x0;
    //PSSACE Tri-state; POLACE disabled; OSCNT None; OETRIG disabled; 
    CCP2CON3H = 0x0;
    //ICOV disabled; ICDIS disabled; SCEVT disabled; ASEVT disabled; TRCLR disabled; TRSET disabled; ICGARM disabled; 
    CCP2STATL = 0x0;
    //TMRL 0x0000; 
    CCP2TMRL = 0x0;
    //TMRH 0x0000; 
    CCP2TMRH = 0x0;
    //PRL 3599; 
    CCP2PRL = 0xE0F;
    //PRH 0; 
    CCP2PRH = 0x0;
    //CMPA 0; 
    CCP2RA = 0x0;
    //CMPB 0; 
    CCP2RB = 0x0;
    //BUFL 0x0000; 
    CCP2BUFL = 0x0;
    //BUFH 0x0000; 
    CCP2BUFH = 0x0;
    
    SCCP2_Timer_TimeoutCallbackRegister(&SCCP2_TimeoutCallback);


}

void SCCP2_Timer_Deinitialize(void)
{
    CCP2CON1Lbits.CCPON = 0;
    
    CCP2CON1L = 0x0; 
    CCP2CON1H = 0x0; 
    CCP2CON2L = 0x0; 
    CCP2CON2H = 0x100; 
    CCP2CON3H = 0x0; 
    CCP2STATL = 0x0; 
    CCP2TMRL = 0x0; 
    CCP2TMRH = 0x0; 
    CCP2PRL = 0xFFFF; 
    CCP2PRH = 0xFFFF; 
    CCP2RA = 0x0; 
    CCP2RB = 0x0; 
    CCP2BUFL = 0x0; 
    CCP2BUFH = 0x0; 
}

void SCCP2_Timer_Start(void)
{
    CCP2CON1Lbits.CCPON = 1;
}

void SCCP2_Timer_Stop(void)
{
    CCP2CON1Lbits.CCPON = 0;
}

void SCCP2_Timer_PeriodSet(uint32_t count)
{
    if(count > 0xFFFFU)
    {
        CCP2PRL = (uint16_t)(count & MASK_32_BIT_LOW);
        CCP2PRH = (uint16_t)((count & MASK_32_BIT_HIGH) >> 16);
        CCP2CON1Lbits.T32 = 1;
    }
    else
    {
        CCP2PRL = (uint16_t)(count & MASK_32_BIT_LOW);
        CCP2CON1Lbits.T32 = 0;
    }
}

void SCCP2_Timer_InterruptPrioritySet(enum INTERRUPT_PRIORITY priority)
{
    IPC6bits.CCT2IP = priority;
}

void SCCP2_Timer_TimeoutCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        SCCP2_TimeoutHandler = handler;
    }
}

void SCCP2_TimeoutCallbackRegister(void* handler)
{
    if(NULL != handler)
    {
        SCCP2_TimeoutHandler = handler;
    }
}

void __attribute__ ((weak)) SCCP2_TimeoutCallback (void)
{ 

} 

void SCCP2_Timer_Tasks(void)
{
    if(IFS1bits.CCT2IF == 1)
    {
        if(NULL != SCCP2_TimeoutHandler)
        {
            (*SCCP2_TimeoutHandler)();
        }
        IFS1bits.CCT2IF = 0;
    }
}

void SCCP2_Timer_PeriodCountSet(size_t count)
{
    CCP2PRL = (uint16_t)(count & MASK_32_BIT_LOW);
    CCP2CON1Lbits.T32 = 0;
}

/**
 End of File
*/
