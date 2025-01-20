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

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB13 GPIO Pin which has a custom name of ACmonitorTrigger to High
 * @pre      The RB13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_SetHigh()          (_LATB13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB13 GPIO Pin which has a custom name of ACmonitorTrigger to Low
 * @pre      The RB13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_SetLow()           (_LATB13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB13 GPIO Pin which has a custom name of ACmonitorTrigger
 * @pre      The RB13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_Toggle()           (_LATB13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB13 GPIO Pin which has a custom name of ACmonitorTrigger
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_GetValue()         _RB13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB13 GPIO Pin which has a custom name of ACmonitorTrigger as Input
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_SetDigitalInput()  (_TRISB13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB13 GPIO Pin which has a custom name of ACmonitorTrigger as Output
 * @param    none
 * @return   none  
 */
#define ACmonitorTrigger_SetDigitalOutput() (_TRISB13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB14 GPIO Pin which has a custom name of TP129_RB14 to High
 * @pre      The RB14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP129_RB14_SetHigh()          (_LATB14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB14 GPIO Pin which has a custom name of TP129_RB14 to Low
 * @pre      The RB14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP129_RB14_SetLow()           (_LATB14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB14 GPIO Pin which has a custom name of TP129_RB14
 * @pre      The RB14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP129_RB14_Toggle()           (_LATB14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB14 GPIO Pin which has a custom name of TP129_RB14
 * @param    none
 * @return   none  
 */
#define TP129_RB14_GetValue()         _RB14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB14 GPIO Pin which has a custom name of TP129_RB14 as Input
 * @param    none
 * @return   none  
 */
#define TP129_RB14_SetDigitalInput()  (_TRISB14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB14 GPIO Pin which has a custom name of TP129_RB14 as Output
 * @param    none
 * @return   none  
 */
#define TP129_RB14_SetDigitalOutput() (_TRISB14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC9 GPIO Pin which has a custom name of TP107_RC9 to High
 * @pre      The RC9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP107_RC9_SetHigh()          (_LATC9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC9 GPIO Pin which has a custom name of TP107_RC9 to Low
 * @pre      The RC9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP107_RC9_SetLow()           (_LATC9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC9 GPIO Pin which has a custom name of TP107_RC9
 * @pre      The RC9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP107_RC9_Toggle()           (_LATC9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC9 GPIO Pin which has a custom name of TP107_RC9
 * @param    none
 * @return   none  
 */
#define TP107_RC9_GetValue()         _RC9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC9 GPIO Pin which has a custom name of TP107_RC9 as Input
 * @param    none
 * @return   none  
 */
#define TP107_RC9_SetDigitalInput()  (_TRISC9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC9 GPIO Pin which has a custom name of TP107_RC9 as Output
 * @param    none
 * @return   none  
 */
#define TP107_RC9_SetDigitalOutput() (_TRISC9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC12 GPIO Pin which has a custom name of TP108_RC12 to High
 * @pre      The RC12 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP108_RC12_SetHigh()          (_LATC12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC12 GPIO Pin which has a custom name of TP108_RC12 to Low
 * @pre      The RC12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP108_RC12_SetLow()           (_LATC12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC12 GPIO Pin which has a custom name of TP108_RC12
 * @pre      The RC12 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP108_RC12_Toggle()           (_LATC12 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC12 GPIO Pin which has a custom name of TP108_RC12
 * @param    none
 * @return   none  
 */
#define TP108_RC12_GetValue()         _RC12

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC12 GPIO Pin which has a custom name of TP108_RC12 as Input
 * @param    none
 * @return   none  
 */
#define TP108_RC12_SetDigitalInput()  (_TRISC12 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC12 GPIO Pin which has a custom name of TP108_RC12 as Output
 * @param    none
 * @return   none  
 */
#define TP108_RC12_SetDigitalOutput() (_TRISC12 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC13 GPIO Pin which has a custom name of TP126_RC13 to High
 * @pre      The RC13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP126_RC13_SetHigh()          (_LATC13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC13 GPIO Pin which has a custom name of TP126_RC13 to Low
 * @pre      The RC13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP126_RC13_SetLow()           (_LATC13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC13 GPIO Pin which has a custom name of TP126_RC13
 * @pre      The RC13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP126_RC13_Toggle()           (_LATC13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC13 GPIO Pin which has a custom name of TP126_RC13
 * @param    none
 * @return   none  
 */
#define TP126_RC13_GetValue()         _RC13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC13 GPIO Pin which has a custom name of TP126_RC13 as Input
 * @param    none
 * @return   none  
 */
#define TP126_RC13_SetDigitalInput()  (_TRISC13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC13 GPIO Pin which has a custom name of TP126_RC13 as Output
 * @param    none
 * @return   none  
 */
#define TP126_RC13_SetDigitalOutput() (_TRISC13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD0 GPIO Pin which has a custom name of PWM_L_N to High
 * @pre      The RD0 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define PWM_L_N_SetHigh()          (_LATD0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD0 GPIO Pin which has a custom name of PWM_L_N to Low
 * @pre      The RD0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PWM_L_N_SetLow()           (_LATD0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD0 GPIO Pin which has a custom name of PWM_L_N
 * @pre      The RD0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PWM_L_N_Toggle()           (_LATD0 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD0 GPIO Pin which has a custom name of PWM_L_N
 * @param    none
 * @return   none  
 */
#define PWM_L_N_GetValue()         _RD0

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD0 GPIO Pin which has a custom name of PWM_L_N as Input
 * @param    none
 * @return   none  
 */
#define PWM_L_N_SetDigitalInput()  (_TRISD0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD0 GPIO Pin which has a custom name of PWM_L_N as Output
 * @param    none
 * @return   none  
 */
#define PWM_L_N_SetDigitalOutput() (_TRISD0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD1 GPIO Pin which has a custom name of PWM_H_N to High
 * @pre      The RD1 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define PWM_H_N_SetHigh()          (_LATD1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD1 GPIO Pin which has a custom name of PWM_H_N to Low
 * @pre      The RD1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PWM_H_N_SetLow()           (_LATD1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD1 GPIO Pin which has a custom name of PWM_H_N
 * @pre      The RD1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define PWM_H_N_Toggle()           (_LATD1 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD1 GPIO Pin which has a custom name of PWM_H_N
 * @param    none
 * @return   none  
 */
#define PWM_H_N_GetValue()         _RD1

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD1 GPIO Pin which has a custom name of PWM_H_N as Input
 * @param    none
 * @return   none  
 */
#define PWM_H_N_SetDigitalInput()  (_TRISD1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD1 GPIO Pin which has a custom name of PWM_H_N as Output
 * @param    none
 * @return   none  
 */
#define PWM_H_N_SetDigitalOutput() (_TRISD1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD2 GPIO Pin which has a custom name of TP39_RD2 to High
 * @pre      The RD2 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP39_RD2_SetHigh()          (_LATD2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD2 GPIO Pin which has a custom name of TP39_RD2 to Low
 * @pre      The RD2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP39_RD2_SetLow()           (_LATD2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD2 GPIO Pin which has a custom name of TP39_RD2
 * @pre      The RD2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP39_RD2_Toggle()           (_LATD2 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD2 GPIO Pin which has a custom name of TP39_RD2
 * @param    none
 * @return   none  
 */
#define TP39_RD2_GetValue()         _RD2

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD2 GPIO Pin which has a custom name of TP39_RD2 as Input
 * @param    none
 * @return   none  
 */
#define TP39_RD2_SetDigitalInput()  (_TRISD2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD2 GPIO Pin which has a custom name of TP39_RD2 as Output
 * @param    none
 * @return   none  
 */
#define TP39_RD2_SetDigitalOutput() (_TRISD2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD5 GPIO Pin which has a custom name of GPIO_Y_L to High
 * @pre      The RD5 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_SetHigh()          (_LATD5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD5 GPIO Pin which has a custom name of GPIO_Y_L to Low
 * @pre      The RD5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_SetLow()           (_LATD5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD5 GPIO Pin which has a custom name of GPIO_Y_L
 * @pre      The RD5 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_Toggle()           (_LATD5 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD5 GPIO Pin which has a custom name of GPIO_Y_L
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_GetValue()         _RD5

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD5 GPIO Pin which has a custom name of GPIO_Y_L as Input
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_SetDigitalInput()  (_TRISD5 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD5 GPIO Pin which has a custom name of GPIO_Y_L as Output
 * @param    none
 * @return   none  
 */
#define GPIO_Y_L_SetDigitalOutput() (_TRISD5 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD6 GPIO Pin which has a custom name of GPIO_Y_H to High
 * @pre      The RD6 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_SetHigh()          (_LATD6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD6 GPIO Pin which has a custom name of GPIO_Y_H to Low
 * @pre      The RD6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_SetLow()           (_LATD6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD6 GPIO Pin which has a custom name of GPIO_Y_H
 * @pre      The RD6 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_Toggle()           (_LATD6 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD6 GPIO Pin which has a custom name of GPIO_Y_H
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_GetValue()         _RD6

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD6 GPIO Pin which has a custom name of GPIO_Y_H as Input
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_SetDigitalInput()  (_TRISD6 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD6 GPIO Pin which has a custom name of GPIO_Y_H as Output
 * @param    none
 * @return   none  
 */
#define GPIO_Y_H_SetDigitalOutput() (_TRISD6 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED1 to High
 * @pre      The RD8 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define LED1_SetHigh()          (_LATD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD8 GPIO Pin which has a custom name of LED1 to Low
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_SetLow()           (_LATD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD8 GPIO Pin which has a custom name of LED1
 * @pre      The RD8 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define LED1_Toggle()           (_LATD8 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD8 GPIO Pin which has a custom name of LED1
 * @param    none
 * @return   none  
 */
#define LED1_GetValue()         _RD8

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED1 as Input
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalInput()  (_TRISD8 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD8 GPIO Pin which has a custom name of LED1 as Output
 * @param    none
 * @return   none  
 */
#define LED1_SetDigitalOutput() (_TRISD8 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT to High
 * @pre      The RD9 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define FAULT_SetHigh()          (_LATD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD9 GPIO Pin which has a custom name of FAULT to Low
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_SetLow()           (_LATD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD9 GPIO Pin which has a custom name of FAULT
 * @pre      The RD9 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define FAULT_Toggle()           (_LATD9 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD9 GPIO Pin which has a custom name of FAULT
 * @param    none
 * @return   none  
 */
#define FAULT_GetValue()         _RD9

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT as Input
 * @param    none
 * @return   none  
 */
#define FAULT_SetDigitalInput()  (_TRISD9 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD9 GPIO Pin which has a custom name of FAULT as Output
 * @param    none
 * @return   none  
 */
#define FAULT_SetDigitalOutput() (_TRISD9 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13 to High
 * @pre      The RD13 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_SetHigh()          (_LATD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13 to Low
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_SetLow()           (_LATD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13
 * @pre      The RD13 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_Toggle()           (_LATD13 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_GetValue()         _RD13

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13 as Input
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_SetDigitalInput()  (_TRISD13 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD13 GPIO Pin which has a custom name of TP4_DPPIM_RD13 as Output
 * @param    none
 * @return   none  
 */
#define TP4_DPPIM_RD13_SetDigitalOutput() (_TRISD13 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD14 GPIO Pin which has a custom name of RELAY to High
 * @pre      The RD14 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define RELAY_SetHigh()          (_LATD14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RD14 GPIO Pin which has a custom name of RELAY to Low
 * @pre      The RD14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define RELAY_SetLow()           (_LATD14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RD14 GPIO Pin which has a custom name of RELAY
 * @pre      The RD14 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define RELAY_Toggle()           (_LATD14 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RD14 GPIO Pin which has a custom name of RELAY
 * @param    none
 * @return   none  
 */
#define RELAY_GetValue()         _RD14

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD14 GPIO Pin which has a custom name of RELAY as Input
 * @param    none
 * @return   none  
 */
#define RELAY_SetDigitalInput()  (_TRISD14 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RD14 GPIO Pin which has a custom name of RELAY as Output
 * @param    none
 * @return   none  
 */
#define RELAY_SetDigitalOutput() (_TRISD14 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif
