/**
 * MAIN_CORE Generated Driver Interface Header File
 * 
 * @file      main_core_interface.h
 * 
 * @defgroup  main_coredriver MSI-Main Driver
 * 
 * @skipline @brief  This is the generated driver interface file for the MAIN_CORE driver. 
 *
 * @skipline @version   Driver Version 1.0.0
 *
 * @skipline  Device : dsPIC33CH128MP508S1
*/

/*disclaimer*/

#ifndef MAIN_CORE_INTERFACE_H
#define MAIN_CORE_INTERFACE_H

// Section: Included Files
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "main_core_types.h"

// Section: Data Type Definitions

/**
 @ingroup  main_coredriver
 @struct   MAIN_CORE_INTERFACE
 @brief    Structure containing the function pointers of MAIN_CORE driver
*/
struct MAIN_CORE_INTERFACE
{
    void (*Initialize)(void);
    ///< Pointer to MAIN_CORE_Initialize
    
    void (*Deinitialize)(void);
    ///< Pointer to MAIN_CORE_Deinitialize

    void (*InterruptRequestGenerate)(void);
    ///< Pointer to MAIN_CORE_InterruptRequestGenerate

    bool (*IsInterruptRequestAcknowledged)(void);
    ///< Pointer to MAIN_CORE_IsInterruptRequestAcknowledged

    void (*InterruptRequestComplete)(void);
    ///< Pointer to MAIN_CORE_InterruptRequestComplete

    bool (*IsInterruptRequested)(void);
    ///< Pointer to MAIN_CORE_IsInterruptRequested

    void (*InterruptRequestAcknowledge)(void);
    ///< Pointer to MAIN_CORE_InterruptRequestAcknowledge

    void (*InterruptRequestAcknowledgeComplete)(void);
    ///< Pointer to MAIN_CORE_InterruptRequestAcknowledgeComplete
    
    enum MAIN_CORE_SYSTEM_STATUS (*SystemStatusGet)(void);
    ///< Pointer to MAIN_CORE_SystemStatusGet

    uint16_t (*FIFORead)(uint16_t *pData, uint16_t wordCount);
    ///< Pointer to MAIN_CORE_FIFORead

    uint16_t (*FIFOWrite)(uint16_t *pData, uint16_t wordCount);
    ///< Pointer to MAIN_CORE_FIFOWrite

    bool (*FIFOReadIsFull)(void);
    ///< Pointer to MAIN_CORE_FIFOReadIsFull
    
    bool (*FIFOReadIsEmpty)(void);
    ///< Pointer to MAIN_CORE_FIFOReadIsEmpty
    
    bool (*FIFOWriteIsFull)(void);
    ///< Pointer to MAIN_CORE_FIFOWriteIsFull
    
    bool (*FIFOWriteIsEmpty)(void);
    ///< Pointer to MAIN_CORE_FIFOWriteIsEmpty
    
    bool (*ProtocolRead)(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData);
    ///< Pointer to MAIN_CORE_ProtocolRead 
    
    bool (*ProtocolWrite)(enum MAIN_CORE_PROTOCOLS protocolName, uint16_t *pData);
    ///< Pointer to MAIN_CORE_ProtocolRead 

    bool (*ProtocolIsFull)(enum MAIN_CORE_PROTOCOLS protocolName);
    ///< Pointer to MAIN_CORE_ProtocolIsFull 

    bool (*ProtocolIsEmpty)(enum MAIN_CORE_PROTOCOLS protocolName);
    ///< Pointer to MAIN_CORE_ProtocolIsEmpty
    
    void (*ProtocolCallbackRegister)(enum MAIN_CORE_PROTOCOLS protocolName, void (*handler)(void));
    ///< Pointer to MAIN_CORE_ProtocolCallbackRegister

    void (*ReadFIFODataReadyCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_ReadFIFODataReadyCallbackRegister

    void (*WriteFIFOEmptyCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_WriteFIFOEmptyCallbackRegister

    void (*FIFOOverFLowUnderFlowCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_FIFOOverFLowUnderFlowCallbackRegister

    void (*MainInitiatedCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_MainInitiatedCallbackRegister

    void (*MainBreakCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_MainBreakCallbackRegister

    void (*MainResetCallbackRegister)(void (*handler)(void));
    ///< Pointer to MAIN_CORE_MainResetCallbackRegister

};

#endif // MAIN_CORE_INTERFACE_H

