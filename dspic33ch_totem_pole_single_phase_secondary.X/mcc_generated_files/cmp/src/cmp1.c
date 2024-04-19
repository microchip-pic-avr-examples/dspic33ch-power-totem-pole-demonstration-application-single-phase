/**
 * CMP1 Generated Driver Source File
 * 
 * @file      cmp1.c
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver source file for CMP1 driver
 *            
 * @skipline @version   Firmware Driver Version 1.2.1
 *
 * @skipline @version   PLIB Version 1.4.0
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

#include <xc.h>
#include "../cmp1.h"

// Section: File specific functions

static void (*CMP1_EventHandler)(void) = NULL;

// Section: Driver Interface
const struct DAC_DC_INTERFACE dac1_dc_interface = {
    .Enable = &CMP1_DACEnable,
    .Disable = &CMP1_DACDisable,
    .DataWrite = &CMP1_DACDataWrite,
};

const struct CMP_INTERFACE CMP_DAC1 = {
    .Initialize = &CMP1_Initialize,
    .Deinitialize = &CMP1_Deinitialize,
    .Enable = &CMP1_Enable,
    .Disable = &CMP1_Disable,
    .StatusGet = &CMP1_StatusGet,
    
    .EventCallbackRegister = &CMP1_EventCallbackRegister,
    .Tasks = &CMP1_Tasks,
    .cmp_dac_dc_interface = &dac1_dc_interface
};

// Section: CMP1 Module APIs

void CMP1_Initialize(void)
{           
    // Comparator Register settings
    DACCTRL1L = 0x40; //FCLKDIV 1:1; CLKDIV 1:1; CLKSEL FVCO/2; DACSIDL disabled; DACON disabled; 
    DACCTRL2H = 0x8A; //SSTIME 138; 
    DACCTRL2L = 0x55; //TMODTIME 85; 
    DAC1CONH = 0x0; //TMCB 0; 
    DAC1CONL = 0x8200; //HYSSEL None; HYSPOL Rising Edge; INSEL S1CMP1A; CMPPOL Non Inverted; FLTREN disabled; DACOEN enabled; CBE disabled; IRQM Interrupts are disabled; DACEN enabled; 

    //Slope Settings
    DAC1DATH = 0xCD; //DACDATH 205; 
    DAC1DATL = 0xCD; //DACDATL 205; 
    SLP1CONH = 0x0; //PSE Negative; TWME disabled; HME disabled; SLOPEN disabled; 
    SLP1CONL = 0x0; //SLPSTRT None; SLPSTOPB None; SLPSTOPA None; HCFSEL None; 
    SLP1DAT = 0x0; //SLPDAT 0; 
    
    CMP1_EventCallbackRegister(&CMP1_EventCallback);
    
    
    DACCTRL1Lbits.DACON = 1;
}

void CMP1_Deinitialize(void)
{ 
    DACCTRL1Lbits.DACON = 0;
    
    
    // Comparator Register settings
    DACCTRL1L = 0x0;
    DACCTRL2H = 0x8A;
    DACCTRL2L = 0x55;
    DAC1CONH = 0x0;
    DAC1CONL = 0x0;

    //Slope Settings
    DAC1DATH = 0x0;
    DAC1DATL = 0x0;
    SLP1CONH = 0x0;
    SLP1CONL = 0x0;
    SLP1DAT = 0x0;
}

bool CMP1_StatusGet(void)
{
    return (DAC1CONLbits.CMPSTAT);
}

void CMP1_Enable(void)
{
    DACCTRL1Lbits.DACON = 1;
}

void CMP1_Disable(void)
{
    DACCTRL1Lbits.DACON = 0;
}

void CMP1_DACEnable(void)
{
    DAC1CONLbits.DACEN = 1;
}

void CMP1_DACDisable(void)
{
    DAC1CONLbits.DACEN = 0;
}

void CMP1_DACDataWrite(size_t value)
{
    DAC1DATHbits.DACDATH = value;
}

void CMP1_EventCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CMP1_EventHandler = handler;
    }
}

void __attribute__ ((weak)) CMP1_EventCallback(void)
{ 
   
} 

void CMP1_Tasks(void)
{
    if(IFS4bits.CMP1IF == 1)
    {
        // CMP1 callback function 
        if(NULL != CMP1_EventHandler)
        {
            (*CMP1_EventHandler)();
        }
    
        // clear the CMP1 interrupt flag
        IFS4bits.CMP1IF = 0;
    }
}

/**
 End of File
*/