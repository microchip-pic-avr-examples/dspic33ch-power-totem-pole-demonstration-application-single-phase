/**
 * SEC_CORE Generated Driver Interface Header File
 * 
 * @file      sec_core_interface.h
 * 
 * @defgroup  sec_coredriver MSI-Secondary Driver
 * 
 * @skipline  @brief  This is the generated driver interface file for SEC_CORE driver. 
 *
 * @skipline @version   Driver Version 1.0.0
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

#ifndef SEC_CORE_INTERFACE_H
#define SEC_CORE_INTERFACE_H

// Section: Included Files
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "sec_core_types.h"

// Section: Data Type Definitions

/**
 @ingroup  sec_coredriver
 @struct   SEC_CORE_INTERFACE
 @brief    Structure containing the function pointers of SEC_CORE driver
*/
struct SEC_CORE_INTERFACE
{
    void (*Initialize)(void);
    ///< Pointer to SEC_COREx_Initialize e.g. SEC_CORE1_Initialize
    
    void (*Deinitialize)(void);
    ///< Pointer to SEC_COREx_Deinitialize e.g. SEC_CORE1_Deinitialize
    
    void (*Program)(void);
    ///< Pointer to SEC_COREx_Program e.g. SEC_CORE1_Program 
    
    void (*Start)(void);
    ///< Pointer to SEC_COREx_Start e.g. SEC_CORE1_Start

    void (*InterruptRequestGenerate)(void);
    ///< Pointer to SEC_COREx_InterruptRequestGenerate e.g. SEC_CORE1_InterruptRequestGenerate

    bool (*IsInterruptRequestAcknowledged)(void);
    ///< Pointer to SEC_COREx_IsInterruptRequestAcknowledged e.g. SEC_CORE1_IsInterruptRequestAcknowledged

    void (*InterruptRequestComplete)(void);
    ///< Pointer to SEC_COREx_InterruptRequestComplete e.g. SEC_CORE1_InterruptRequestComplete

    bool (*IsInterruptRequested)(void);
    ///< Pointer to SEC_COREx_IsInterruptRequested e.g. SEC_CORE1_IsInterruptRequested

    void (*InterruptRequestAcknowledge)(void);
    ///< Pointer to SEC_COREx_InterruptRequestAcknowledge e.g. SEC_CORE1_InterruptRequestAcknowledge

    void (*InterruptRequestAcknowledgeComplete)(void);
    ///< Pointer to SEC_COREx_InterruptRequestAcknowledgeComplete e.g. SEC_CORE1_InterruptRequestAcknowledgeComplete
    
    enum SEC_CORE_RESET_CAUSE (*GetResetCause)(void);
    ///< Pointer to SEC_COREx_GetResetCause e.g. SEC_CORE1_GetResetCause
    
    void (*ResetCauseClear)(enum SEC_CORE_RESET_CAUSE resetCause);
    ///< Pointer to SEC_COREx_ResetCauseClear e.g. SEC_CORE1_ResetCauseClear

    enum SEC_CORE_SYSTEM_STATUS (*SystemStatusGet)(void);
    ///< Pointer to SEC_COREx_SystemStatusGet e.g. SEC_CORE1_SystemStatusGet

    void (*WriteFIFOEnable)(void);
    ///< Pointer to SEC_COREx_WriteFIFOEnable e.g. SEC_CORE1_WriteFIFOEnable

    void (*WriteFIFODisable)(void);
    ///< Pointer to SEC_COREx_WriteFIFODisable e.g. SEC_CORE1_WriteFIFODisable

    void (*ReadFIFOEnable)(void);
    ///< Pointer to SEC_COREx_ReadFIFOEnable e.g. SEC_CORE1_ReadFIFOEnable

    void (*ReadFIFODisable)(void);
    ///< Pointer to SEC_COREx_ReadFIFODisable e.g. SEC_CORE1_ReadFIFODisable

    uint16_t (*FIFORead)(uint16_t *pData, uint16_t wordCount);
    ///< Pointer to SEC_COREx_FIFORead e.g. SEC_CORE1_FIFORead

    uint16_t (*FIFOWrite)(uint16_t *pData, uint16_t wordCount);
    ///< Pointer to SEC_COREx_FIFOWrite e.g. SEC_CORE1_FIFOWrite

    bool (*FIFOReadIsFull)(void);
    ///< Pointer to SEC_COREx_FIFOReadIsFull e.g. SEC_CORE1_FIFOReadIsFull
    
    bool (*FIFOReadIsEmpty)(void);
    ///< Pointer to SEC_COREx_FIFOReadIsEmpty e.g. SEC_CORE1_FIFOReadIsEmpty
    
    bool (*FIFOWriteIsFull)(void);
    ///< Pointer to SEC_COREx_FIFOWriteIsFull e.g. SEC_CORE1_FIFOWriteIsFull
    
    bool (*FIFOWriteIsEmpty)(void);
    ///< Pointer to SEC_COREx_FIFOWriteIsEmpty e.g. SEC_CORE1_FIFOWriteIsEmpty
    
    bool (*ProtocolRead)(enum SEC_CORE_PROTOCOLS protocolName, uint16_t *pData);
    ///< Pointer to SEC_COREx_ProtocolRead e.g. SEC_CORE1_ProtocolRead
    
    bool (*ProtocolWrite)(enum SEC_CORE_PROTOCOLS protocolName, uint16_t *pData);
    ///< Pointer to SEC_COREx_ProtocolRead e.g. SEC_CORE1_ProtocolRead

    bool (*ProtocolIsFull)(enum SEC_CORE_PROTOCOLS protocolName);
    ///< Pointer to SEC_COREx_ProtocolIsFull e.g. SEC_CORE1_ProtocolIsFull

    bool (*ProtocolIsEmpty)(enum SEC_CORE_PROTOCOLS protocolName);
    ///< Pointer to SEC_COREx_ProtocolIsEmpty e.g. SEC_CORE1_ProtocolIsEmpty
    
    void (*ProtocolCallbackRegister)(enum SEC_CORE_PROTOCOLS protocolName, void (*handler)(void));
    ///< Pointer to SEC_COREx_ProtocolCallbackRegister e.g. SEC_CORE1_ProtocolCallbackRegister

    void (*ReadFIFODataReadyCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_ReadFIFODataReadyCallbackRegister e.g. SEC_CORE1_ReadFIFODataReadyCallbackRegister

    void (*WriteFIFOEmptyCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_WriteFIFOEmptyCallbackRegister e.g. SEC_CORE1_WriteFIFOEmptyCallbackRegister

    void (*FIFOOverFLowUnderFlowCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_FIFOOverFLowUnderFlowCallbackRegister e.g. SEC_CORE1_FIFOOverFLowUnderFlowCallbackRegister

    void (*SecondaryInitiatedCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_SecondaryInitiatedCallbackRegister e.g. SEC_CORE1_SecondaryInitiatedCallbackRegister

    void (*SecondaryBreakCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_SecondaryBreakCallbackRegister e.g. SEC_CORE1_SecondaryBreakCallbackRegister

    void (*SecondaryResetCallbackRegister)(void (*handler)(void));
    ///< Pointer to SEC_COREx_SecondaryResetCallbackRegister e.g. SEC_CORE1_SecondaryResetCallbackRegister

};

#endif // SEC_CORE_INTERFACE_H

