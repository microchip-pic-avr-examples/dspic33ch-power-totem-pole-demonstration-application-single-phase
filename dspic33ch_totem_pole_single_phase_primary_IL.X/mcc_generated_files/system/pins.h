/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   Firmware Driver Version 1.0.2
 *
 * @skipline @version   PLIB Version 1.2.0
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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of PIM_TP4 to High
 * @pre      The RD13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define PIM_TP4_SetHigh()          (_LATD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of PIM_TP4 to Low
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PIM_TP4_SetLow()           (_LATD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD13 GPIO Pin which has a custom name of PIM_TP4
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PIM_TP4_Toggle()           (_LATD13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD13 GPIO Pin which has a custom name of PIM_TP4
 * @param    none
 * @return   none  
 */
#define PIM_TP4_GetValue()         _RD13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of PIM_TP4 as Input
 * @param    none
 * @return   none  
 */
#define PIM_TP4_SetDigitalInput()  (_TRISD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of PIM_TP4 as Output
 * @param    none
 * @return   none  
 */
#define PIM_TP4_SetDigitalOutput() (_TRISD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LD2_RD15 to High
 * @pre      The RD15 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LD2_RD15_SetHigh()          (_LATD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD15 GPIO Pin which has a custom name of LD2_RD15 to Low
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LD2_RD15_SetLow()           (_LATD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD15 GPIO Pin which has a custom name of LD2_RD15
 * @pre      The RD15 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LD2_RD15_Toggle()           (_LATD15 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD15 GPIO Pin which has a custom name of LD2_RD15
 * @param    none
 * @return   none  
 */
#define LD2_RD15_GetValue()         _RD15

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LD2_RD15 as Input
 * @param    none
 * @return   none  
 */
#define LD2_RD15_SetDigitalInput()  (_TRISD15 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD15 GPIO Pin which has a custom name of LD2_RD15 as Output
 * @param    none
 * @return   none  
 */
#define LD2_RD15_SetDigitalOutput() (_TRISD15 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
