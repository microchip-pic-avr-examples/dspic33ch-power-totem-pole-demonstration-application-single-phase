/**
 * CLOCK Generated Driver Source File 
 * 
 * @file      clock.c
 *            
 * @ingroup   clockdriver
 *            
 * @brief     This is the generated source file for CLOCK driver
 *
 * @version   Firmware Driver Version 1.0.2
 *
 * @version   PLIB Version 1.4.1
 *
 * @skipline  Device : dsPIC33CH512MP506S1
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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

// Section: Includes
#include <xc.h>
#include <stdint.h>
#include "../clock.h"

void CLOCK_Initialize(void)
{
    /*  
       Input frequency                               :  8.00 MHz
       Clock source                                  :  FRC Oscillator with PLL
       System frequency (Fosc)                       :  200.00 MHz [(8.00 MHz / 1) * 50 / 1 / 2 = 200.00 MHz]
       PLL VCO frequency (Fvco)                      :  400.00 MHz [(8.00 MHz / 1) * 50 = 400.00 MHz]
       PLL output frequency (Fpllo)                  :  400.00 MHz [(8.00 MHz / 1) * 50 / 1 = 400.00 MHz]
       PLL VCO divider frequency (Fvcodiv)           :  100.00 MHz [400.00 MHz / 4 = 100.00 MHz]
       Clock switching enabled                       :  true
       Clock source when device boots                :  FRC Oscillator
       Auxiliary clock source                        :  FRC Oscillator with PLL
       Auxiliary clock input frequency               :  8.00 MHz
       Auxiliary PLL VCO frequency (AFvco)           :  1000.00 MHz  [(8.00 MHz / 1) * 125 = 1000.00 MHz]
       Auxiliary clock PLL output frequency (AFpllo) :  500.00 MHz [(8.00 MHz / 1) * 125 / 2 = 500.00 MHz]
       Auxiliary PLL VCO divider frequency (AFvcodiv):  500.00 MHz [1000.00 MHz / 2 = 500.00 MHz]
    */
    // RCDIV FRC/1; PLLPRE 1:1; DOZE 1:8; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3001;
    
    // PLLDIV 50; 
    PLLFBD = 0x32<<1; //<<< changed by hand
    // PLLPOST 1:1; VCODIV FVCO/4; POST2DIV 1:1; 
    PLLDIV = 0x21; //<<< changed by hand
    
    // ENAPLL enabled; FRCSEL FRC Oscillator; APLLPRE 1:1; 
    ACLKCON1 = 0x8101;
    // APLLFBDIV 125; 
    APLLFBD1 = 0x7D;
    // APSTSCLR 1:2; APOST2DIV 1:1; AVCODIV FVCO/2; 
    APLLDIV1 = 0x221;
    // ROEN disabled; DIVSWEN disabled; ROSLP disabled; ROSEL ; OE disabled; ROSIDL disabled; 
    REFOCONL = 0x0;
    // RODIV 0; 
    REFOCONH = 0x0;
    // ROTRIM 0; 
    REFOTRIMH = 0x0;
    // IOLOCK disabled; 
    RPCON = 0x0;
    // ADC1MD enabled; T1MD enabled; U1MD enabled; SPI1MD enabled; QEIMD enabled; PWMMD enabled; I2C1MD enabled; 
    PMD1 = 0x0;
    // CCP2MD enabled; CCP1MD enabled; CCP4MD enabled; CCP3MD enabled; 
    PMD2 = 0x0;
    // REFOMD enabled; 
    PMD4 = 0x0;
    // DMA1MD enabled; DMA0MD enabled; 
    PMD6 = 0x0;
    // CMP1MD enabled; CMP3MD enabled; PGA1MD enabled; CMP2MD enabled; 
    PMD7 = 0x0;
    // CLC3MD enabled; CLC4MD enabled; CLC1MD enabled; CLC2MD enabled; PGA2MD enabled; PGA3MD enabled; 
    PMD8 = 0x0;
    // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) (0x01));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}

bool CLOCK_AuxPllLockStatusGet(void)
{
    return ACLKCON1bits.APLLCK;
}

