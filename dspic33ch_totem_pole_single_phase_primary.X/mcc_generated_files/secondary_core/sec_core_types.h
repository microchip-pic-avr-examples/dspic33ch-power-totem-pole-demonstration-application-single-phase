/**
 * SEC_CORE Generated Driver Types Header File
 * 
 * @file      sec_core_types.h
 *            
 * @ingroup   sec_coredriver
 *            
 * @brief     This is the generated driver types header file for the SEC_CORE driver
 *            
 * @skipline @version   Firmware Driver Version 1.0.0
 *
 * @skipline @version   PLIB Version 1.0.0
 *            
 * @skipline  Device : dsPIC33CH128MP508
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

#ifndef SEC_CORE_TYPES_H
#define SEC_CORE_TYPES_H

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_SYSTEM_STATUS
 @brief     Defines the status enumeration for sec_core.
*/
enum SEC_CORE_SYSTEM_STATUS
{
    SEC_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE = 0,   /**< sec_core status is in Low power mode */
    SEC_CORE_SYSTEM_STATUS_SLEEP_MODE            = 1,   /**< sec_core status is in Sleep mode */
    SEC_CORE_SYSTEM_STATUS_IDLE_MODE             = 2,   /**< sec_core status is in Idle mode */
    SEC_CORE_SYSTEM_STATUS_RUNNING_STATE         = 3,   /**< sec_core status is in Running state */
    SEC_CORE_SYSTEM_STATUS_IN_RESET_STATE        = 4    /**< sec_core status is in Reset state */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_RESET_CAUSE
 @brief     Defines the sec_core reset cause enumeration
*/
enum SEC_CORE_RESET_CAUSE
{
    SEC_CORE_RESET_CAUSE_WATCHDOG    = 0,            /**< sec_core reset caused from Watchdog Timer */
    SEC_CORE_RESET_CAUSE_UNSPECIFIED = 1             /**< sec_core reset cause is unspecified */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_PROTOCOLS
 @brief     Defines the list of Protocols configured for SEC_CORE_driver. 
 @note      These Protocols are required to create the buffer by the application
*/
enum SEC_CORE_PROTOCOLS
{
    MSI1_ProtocolA,      /**< Custom-name of ProtocolA for write operation only */
    MSI1_ProtocolB,      /**< Custom-name of ProtocolB for write operation only */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_PROTOCOL_SIZE
 @brief     Defines the size for Protocols.
 @note      Size not required for the driver but will be required for application to create the buffers
*/
enum SEC_CORE_PROTOCOL_SIZE
{
    MSI1_ProtocolA_SIZE = 5,     /**< Size of ProtocolA in 16-bit word*/
    MSI1_ProtocolB_SIZE = 10,     /**< Size of ProtocolB in 16-bit word*/
};

#endif    /* SEC_CORE1_TYPES_H */