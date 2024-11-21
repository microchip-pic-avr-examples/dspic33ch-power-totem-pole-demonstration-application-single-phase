/**
 * CMP1 Generated Driver Header File 
 * 
 * @file      cmp1.h
 *            
 * @ingroup   cmpdriver
 *            
 * @brief     This is the generated driver header file for the CMP1 driver
 *            
 * @skipline @version   Firmware Driver Version 1.2.1
 *
 * @skipline @version   PLIB Version 1.4.0
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

#ifndef CMP1_H
#define CMP1_H

// Section: Included Files

#include <stddef.h>
#include <stdbool.h>
#include "cmp_interface.h"

// Section: Data Type Definitions

/**
 @ingroup  cmpdriver
 @brief    Structure object of type CMP_INTERFACE with the 
           custom name given by the user in the Melody Driver User interface. 
           The default name e.g. CMP_DAC1 can be changed by the 
           user in the CMP user interface. 
           This allows defining a structure with application specific name 
           using the 'Custom Name' field. Application specific name allows the 
           API Portability.
*/
extern const struct CMP_INTERFACE CMP_DAC1;

/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_Initialize API
 */
#define CMP_DAC1_Initialize CMP1_Initialize
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_Deinitialize API
 */
#define CMP_DAC1_Deinitialize CMP1_Deinitialize
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_StatusGet API
 */
#define CMP_DAC1_StatusGet CMP1_StatusGet
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_Enable API
 */
#define CMP_DAC1_Enable CMP1_Enable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_Disable API
 */
#define CMP_DAC1_Disable CMP1_Disable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_DACEnable API
 */
#define CMP_DAC1_DACEnable CMP1_DACEnable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_DACDisable API
 */
#define CMP_DAC1_DACDisable CMP1_DACDisable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_DACDataWrite API
 */
#define CMP_DAC1_DACDataWrite CMP1_DACDataWrite
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_DACDisable API
 */
#define CMP_DAC1_DACDisable CMP1_DACDisable
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_EventCallbackRegister API
 */
#define CMP_DAC1_EventCallbackRegister CMP1_EventCallbackRegister
/**
 * @ingroup  cmpdriver
 * @brief    This macro defines the Custom Name for \ref CMP1_Tasks API
 */
#define CMP_DAC1_Tasks CMP1_Tasks

// Section: CMP1 Module APIs

/**
 * @ingroup  cmpdriver
 * @brief    Initialize the CMP1 module
 * @param    none
 * @return   none  
 */

void CMP1_Initialize(void);

/**
 * @ingroup  cmpdriver
 * @brief    Deinitializes the CMP1 to POR values
 * @param    none
 * @return   none  
 */
void CMP1_Deinitialize(void);
        
/**
 * @ingroup  cmpdriver
 * @brief    Returns the comparator output status 
 * @param    none
 * @return   true   - Comparator output is high
 * @return   false  - Comparator output is low
 */
bool CMP1_StatusGet(void);

/**
 * @ingroup  cmpdriver
 * @brief    Enables the common DAC module
 * @param    none
 * @return   none 
 */
void CMP1_Enable(void);
    
/**
 * @ingroup  cmpdriver
 * @brief    Disables the common DAC module
 * @param    none
 * @return   none  
 */
void CMP1_Disable(void);

/**
 * @ingroup  cmpdriver
 * @brief    Enables the individual DAC module
 * @param    none
 * @return   none 
 */
void CMP1_DACEnable(void);
    
/**
 * @ingroup  cmpdriver
 * @brief    Disables the individual DAC module
 * @param    none
 * @return   none
 */
void CMP1_DACDisable(void);

/**
 * @ingroup    cmpdriver
 * @brief      CMP DAC Data write to register
 * @param[in]  value - DAC Data write value
 * @return     none 
 */
void CMP1_DACDataWrite(size_t value);

/**
 * @ingroup    cmpdriver
 * @brief      This function can be used to override default callback and to 
 *             define custom callback for CMP1 Event event
 * @param[in]  handler - Address of the callback function.  
 * @return     none  
 */
void CMP1_EventCallbackRegister(void (*handler)(void));

/**
 * @ingroup  cmpdriver
 * @brief    This is the default callback with weak attribute. 
 *           The user can override and implement the default callback without 
 *           weak attribute or can register a custom callback function using  
 *           CMP1_EventCallbackRegister.
 * @param    none
 * @return   none  
 */
void CMP1_EventCallback(void);

/**
 * @ingroup  cmpdriver
 * @brief    The Task function can be called in the main application using the High Speed
 *           Comparator, when interrupts are not used.  This would thus introduce the 
 *           polling mode feature of the Analog Comparator.
 * @param    none
 * @return   none  
 */
void CMP1_Tasks(void);

#endif //CMP1_H

/**
  End of File
*/

