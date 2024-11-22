/**
 * CAN Generated Driver Interface Header File
 * 
 * @file      can_interface.h
 *            
 * @defgroup  candriver CAN FD Driver
 *            
 * @brief     CAN FD Driver using dsPIC MCUs
 *            
 * @skipline @version   Firmware Driver Version 1.3.1
 *
 * @skipline @version   PLIB Version 1.7.0
 *            
 * @skipline  Device : dsPIC33CH512MP506
*/

/*
� [2024] Microchip Technology Inc. and its subsidiaries.

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

#ifndef CAN_INTERFACE_H
#define CAN_INTERFACE_H

// Section: Included Files

#include <stdbool.h>
#include <stdint.h>
#include "can_types.h"

// Section: Data Type Definitions     

/**
 @ingroup  candriver
 @struct   CAN_INTERFACE
 @brief    Structure containing the function pointers of CAN driver
*/
struct CAN_INTERFACE
{   
    void (*Initialize)(void);                                                                               
    ///< Pointer to CANx_Initialize e.g. \ref CAN1_Initialize
    
    void (*Deinitialize)(void);                                                                             
    ///< Pointer to CANx_Deinitialize e.g. \ref CAN1_Deinitialize
    
    enum CAN_OP_MODE_STATUS (*OperationModeSet)(const enum CAN_OP_MODES requestMode);                       
    ///< Pointer to CANx_OperationModeSet e.g. \ref CAN1_OperationModeSet
    
    enum CAN_OP_MODES (*OperationModeGet)(void);                                                            
    ///< Pointer to CANx_OperationModeGet e.g. \ref CAN1_OperationModeGet
    
    void (*SleepMode)(void);                                                                                
    ///< Pointer to CANx_Sleep e.g. \ref CAN1_Sleep
    
    bool (*IsBusOff)(void);                                                                                 
    ///< Pointer to CANx_IsBusOff e.g. \ref CAN1_IsBusOff
                             
    enum CAN_TX_MSG_REQUEST_STATUS (*Transmit)(const unsigned fifoChannel, struct CAN_MSG_OBJ *txCanMsg);   
    ///< Pointer to CANx_Transmit e.g. \ref CAN1_Transmit
    
    enum CAN_TX_FIFO_STATUS (*TransmitFIFOStatusGet)(const unsigned fifoChannel);                           
    ///< Pointer to  CANx_TransmitFIFOStatusGet e.g. \ref CAN1_TransmitFIFOStatusGet
    
    bool (*IsTxErrorPassive)(void);                                                                         
    ///< Pointer to CANx_IsTxErrorPassive e.g. \ref  CAN1_IsTxErrorPassive
    
    bool (*IsTxErrorWarning)(void);                                                                         
    ///< Pointer to CANx_IsTxErrorWarning e.g. \ref CAN1_IsTxErrorWarning
    
    bool (*IsTxErrorActive)(void);                                                                          
    ///< Pointer to CANx_IsTxErrorActive e.g. \ref CAN1_IsTxErrorActive
                                                                                                                                      
    bool (*Receive)(struct CAN_MSG_OBJ *rxCanMsg);                                                          
    ///< Pointer to CANx_Receive e.g. \ref CAN1_Receive
    
    bool (*ReceiveMessageGet)(const unsigned fifoChannel, struct CAN_MSG_OBJ *rxCanMsg);                    
    ///< Pointer to CANx_ReceiveMessageGet e.g. \ref CAN1_ReceiveMessageGet
    
    bool (*IsRxErrorPassive)(void);                                                                         
    ///< Pointer to CANx_IsRxErrorPassive e.g. \ref CAN1_IsRxErrorPassive
    
    bool (*IsRxErrorWarning)(void);                                                                         
    ///< Pointer to CANx_IsRxErrorWarning e.g. \ref CAN1_IsRxErrorWarning
    
    bool (*IsRxErrorActive)(void);                                                                          
    ///< Pointer to CANx_IsRxErrorActive e.g. \ref CAN1_IsRxErrorActive
    
    uint8_t (*ReceivedMessageCountGet)(void);                                                               
    ///< Pointer to CANx_ReceivedMessageCountGet e.g. \ref CAN1_ReceivedMessageCountGet
    
    uint8_t (*RX_FIFO_StatusGet)(const unsigned fifoNum);                                                               
    ///< Pointer to CANx_RX_FIFO_StatusGet e.g. \ref CAN1_RX_FIFO_StatusGet

    void (*InvalidMessageCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_InvalidMessageCallbackRegister e.g. \ref CAN1_InvalidMessageCallbackRegister

    void (*BusWakeUpActivityCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_BusWakeUpActivityCallbackRegister e.g. \ref CAN1_BusWakeUpActivityCallbackRegister

    void (*BusErrorCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_BusErrorCallbackRegister e.g. \ref CAN1_BusErrorCallbackRegister

    void (*ModeChangeCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_ModeChangeCallbackRegister e.g. \ref CAN1_ModeChangeCallbackRegister

    void (*SystemErrorCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_SystemErrorCallbackRegister e.g. \ref CAN1_SystemErrorCallbackRegister

    void (*TxAttemptCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_TxAttemptCallbackRegister e.g. \ref CAN1_TxAttemptCallbackRegister

    void (*RxBufferOverFlowCallbackRegister)(void (*CallbackHandler)(void));                               
    ///< Pointer to CANx_RxBufferOverFlowCallbackRegister e.g. \ref CAN1_RxBufferOverFlowCallbackRegister

    void (*Tasks)(void);                                                                                    
    ///< Pointer to CANx_Tasks e.g. \ref CAN1_Tasks (Supported only in polling mode)
};

#endif

