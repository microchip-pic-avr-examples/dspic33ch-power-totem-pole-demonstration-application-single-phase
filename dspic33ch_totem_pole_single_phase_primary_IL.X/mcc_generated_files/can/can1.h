/**
 * CAN1 Generated Driver Header File 
 * 
 * @file      can1.h
 *            
 * @ingroup   candriver
 *            
 * @brief     This is the generated driver header file for the CAN1 driver using CCL
 *            
 * @skipline @version   Firmware Driver Version 1.3.1
 *
 * @skipline @version   PLIB Version 1.7.0
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

#ifndef CAN1_H
#define CAN1_H

// Section: Included Files

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "can_types.h"
#include "can_interface.h"

// Section: Data Type Definitions

/**
 @ingroup  candriver
 @brief    Structure object of type CAN_INTERFACE with the custom name given by 
           the user in the Melody Driver User interface. The default name 
           e.g. CAN can be changed by the user in the CAN FD user interface. 
           This allows defining a structure with application specific name using 
           the 'Custom Name' field. Application specific name allows the API Portability.
*/
extern const struct CAN_INTERFACE CAN_FD1;
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Initialize API
 */
#define CAN_FD1_Initialize CAN1_Initialize
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Deinitialize API
 */
#define CAN_FD1_Deinitialize CAN1_Deinitialize
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_OperationModeSet API
 */
#define CAN_FD1_OperationModeSet CAN1_OperationModeSet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_OperationModeGet API
 */
#define CAN_FD1_OperationModeGet CAN1_OperationModeGet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Receive API
 */
#define CAN_FD1_Receive CAN1_Receive
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_ReceiveMessageGet API
 */
#define CAN_FD1_ReceiveMessageGet CAN1_ReceiveMessageGet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_RX_FIFO_StatusGet API
 */
#define CAN_FD1_RX_FIFO_StatusGet CAN1_RX_FIFO_StatusGet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Transmit API
 */
#define CAN_FD1_Transmit CAN1_Transmit
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_TransmitFIFOStatusGet API
 */
#define CAN_FD1_TransmitFIFOStatusGet CAN1_TransmitFIFOStatusGet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsBusOff API
 */
#define CAN_FD1_IsBusOff CAN1_IsBusOff
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsTxErrorPassive API
 */
#define CAN_FD1_IsTxErrorPassive CAN1_IsTxErrorPassive
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsTxErrorWarning API
 */
#define CAN_FD1_IsTxErrorWarning CAN1_IsTxErrorWarning
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsTxErrorActive API
 */
#define CAN_FD1_IsTxErrorActive CAN1_IsTxErrorActive
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsRxErrorPassive API
 */
#define CAN_FD1_IsRxErrorPassive CAN1_IsRxErrorPassive
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsRxErrorWarning API
 */
#define CAN_FD1_IsRxErrorWarning CAN1_IsRxErrorWarning
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_IsRxErrorActive API
 */
#define CAN_FD1_IsRxErrorActive CAN1_IsRxErrorActive
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Sleep API
 */
#define CAN_FD1_Sleep CAN1_Sleep
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_ReceivedMessageCountGet API
 */
#define CAN_FD1_ReceivedMessageCountGet CAN1_ReceivedMessageCountGet
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_Tasks API
 */
#define CAN_FD1_Tasks CAN1_Tasks
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_InvalidMessageCallbackRegister API
 */
#define CAN_FD1_InvalidMessageCallbackRegister CAN1_InvalidMessageCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_BusWakeUpActivityCallbackRegister API
 */
#define CAN_FD1_BusWakeUpActivityCallbackRegister CAN1_BusWakeUpActivityCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_BusErrorCallbackRegister API
 */
#define CAN_FD1_BusErrorCallbackRegister CAN1_BusErrorCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_ModeChangeCallbackRegister API
 */
#define CAN_FD1_ModeChangeCallbackRegister CAN1_ModeChangeCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_SystemErrorCallbackRegister API
 */
#define CAN_FD1_SystemErrorCallbackRegister CAN1_SystemErrorCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_TxAttemptCallbackRegister API
 */
#define CAN_FD1_TxAttemptCallbackRegister CAN1_TxAttemptCallbackRegister
/**
 * @ingroup  candriver
 * @brief    This macro defines the Custom Name for \ref CAN1_RxBufferOverFlowCallbackRegister API
 */
#define CAN_FD1_RxBufferOverFlowCallbackRegister CAN1_RxBufferOverFlowCallbackRegister


/**
 @ingroup  candriver
 @brief    CAN1 Transmit FIFO TXQ Custom Name
*/
#define CAN1_TX_TXQ   CAN1_TXQ   /**< Defines the custom name of \ref CAN1_TXQ used for Transmit functionality */

/**
 @ingroup  candriver
 @brief    CAN1 Receive FIFO 1 Custom Name
*/
#define CAN1_RX_FIFO1   CAN1_FIFO_1   /**< Defines the custom name of \ref CAN1_FIFO_1 used for Receive functionality */

/**
 @ingroup  candriver
 @enum     CAN1_TX_FIFO_CHANNELS
 @brief    This enumeration defines the can Transmit FIFO Configured 
           in MCC Melody CAN user interface.
*/
enum CAN1_TX_FIFO_CHANNELS
{
    CAN1_TXQ = 0,   /**< CAN1 Transmit FIFO TXQ */
}; 

/**
 @ingroup  candriver
 @enum     CAN1_RX_FIFO_CHANNELS
 @brief    This enumeration defines the can receive FIFO.
*/
enum CAN1_RX_FIFO_CHANNELS
{
    CAN1_FIFO_1 = 1,  /**< CAN1 Receive FIFO 1 */
};

// Section: CAN1 Module APIs

/**
 * @ingroup  candriver
 * @brief    Initializes CAN1 module 
 * @param    none
 * @return   none  
 */
void CAN1_Initialize(void);

/**
 * @ingroup  candriver
 * @brief    Deinitializes CAN1 to POR values.
 * @param    none
 * @return   none 
 */
void CAN1_Deinitialize(void);

/**
 * @ingroup    candriver
 * @brief      Sets the CAN1 operation mode
 * @pre        \ref CAN1_Initialize() function should be called 
 *             before calling this function.
 * @param[in]  requestMode - CAN1 operation modes  
 * @return     CAN1 Operation mode request to set.
 */
enum CAN_OP_MODE_STATUS CAN1_OperationModeSet(const enum CAN_OP_MODES requestMode);

/**
 * @ingroup  candriver
 * @brief    Get the CAN1 operation mode
 * @pre      \ref CAN1_Initialize() function should be called
 *           before calling this function. 
 * @param    none
 * @return   Return the present CAN1 operation mode   
 */
enum CAN_OP_MODES CAN1_OperationModeGet(void);

/**
 * @ingroup     candriver
 * @brief       Reads the received single message object
 * @pre         \ref CAN1_Initialize() function should be called before
 *              calling this function. The \ref CAN1_ReceivedMessageCountGet()
 *              function should be checked to see if any CAN message is received.
 * @param[out]  rxCanMsg  - pointer to the message object  
 * @return      true   - CAN message read success
 * @return      false  - CAN message read fail
 */
bool CAN1_Receive(struct CAN_MSG_OBJ *rxCanMsg);

/**
 * @ingroup     candriver
 * @brief       Reads the received single message object from the CAN1 receive FIFO.
 * @pre         \ref CAN1_Initialize() function should be called before
 *              calling this function. The \ref CAN1_ReceivedMessageCountGet()
 *              function should be checked to see if the receiver is not empty before calling
 *              this function.
 * @param[out]  rxCanMsg - pointer to the message object  
 * @return      true   - CAN message read success
 * @return      false  - CAN message read fail  
 */
bool CAN1_ReceiveMessageGet(const enum CAN1_RX_FIFO_CHANNELS fifoChannel, struct CAN_MSG_OBJ *rxCanMsg);

/**
 * @ingroup    candriver
 * @brief      Writes the CAN message object to specified transmit FIFO channel
 * @pre        \ref CAN1_Initialize() function should be called before
 *             calling this function. \ref CAN1_TransmitFIFOStatusGet function
               should be called to check buffer availability in the Transmit FIFO. 
 * @param[in]  fifoChannel - FIFO channel where the message object to be written
 * @param[in]  txCanMsg    - pointer to the message object  
 * @return     Returns the transmit status of type \ref CAN_TX_MSG_REQUEST_STATUS
 */
enum CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(const enum CAN1_TX_FIFO_CHANNELS fifoChannel, struct CAN_MSG_OBJ *txCanMsg);

/**
 * @ingroup    candriver
 * @brief      Returns the CAN1 transmitter FIFO status. 
 * @pre        \ref CAN1_Initialize() function should be called before
 *             calling this function.
 * @param[in]  fifoChannel - CAN1 TX priority FIFO selection  
 * @return     Returns the transmit status of type \ref CAN_TX_FIFO_STATUS
 */
enum CAN_TX_FIFO_STATUS CAN1_TransmitFIFOStatusGet(const enum CAN1_TX_FIFO_CHANNELS fifoChannel);

/**
 * @ingroup  candriver
 * @brief    Returns the bus off status
 * @pre      \ref CAN1_Initialize() function should be called before calling this function.
 * @param    none
 * @return   true  - CAN node in Bus Off state
 * @return   false - CAN node not in Bus Off state
 */
bool CAN1_IsBusOff(void);

/**
 * @ingroup  candriver
 * @brief    Returns the transmit error passive state
 * @pre      \ref CAN1_Initialize() function should be called
 *           before calling this function.
 * @param    none
 * @return   true    - CAN node Error Passive state
 * @return   false   - CAN node not Error Passive state   
 */
bool CAN1_IsTxErrorPassive(void);

/**
 * @ingroup  candriver
 * @brief    Returns the transmit error warning state. 
 *           If Transmitter error counter is above 95 to below
 *           128, then transmitter error warning state is set.
 * @pre      \ref CAN1_Initialize() function should be called before
 *           calling this function.
 * @param    none
 * @return   true    - CAN node Error warning state
 * @return   false   - CAN node not Error warning state   
 */
bool CAN1_IsTxErrorWarning(void);

/**
 * @ingroup  candriver
 * @brief    Returns the transmit error active state 
 * @pre      \ref CAN1_Initialize() function should be called before 
 *           calling this function.
 * @param    none
 * @return   true    - CAN node Error active state
 * @return   false   - CAN node not Error active state  
 */
bool CAN1_IsTxErrorActive(void);

/**
 * @ingroup  candriver
 * @brief    Returns the receive error passive state
 * @pre      \ref CAN1_Initialize() function should be called before
 *           calling this function.
 * @param    none
 * @return   true    - CAN node Error Passive state
 * @return   false   - CAN node not Error Passive state  
 */
bool CAN1_IsRxErrorPassive(void);

/**
 * @ingroup  candriver
 * @brief    Returns the receive error warning state.
 *           If Receiver error counter is above 95 to below 128, then
 *           receiver error warning state is set.
 * @pre      \ref CAN1_Initialize() function should be called before
 *           calling this function.
 * @param    none
 * @return   true    - CAN node Error warning state
 * @return   false   - CAN node not Error warning state  
 */
bool CAN1_IsRxErrorWarning(void);

/**
 * @ingroup  candriver
 * @brief    Returns the receive error active state
 * @pre      \ref CAN1_Initialize() function should be called before
 *           calling this function.
 * @param    none
 * @return   true    - CAN node Error active state
 * @return   false   - CAN node not Error active state  
 */
bool CAN1_IsRxErrorActive(void);

/**
 * @ingroup  candriver
 * @brief    Sets the CAN node in sleep mode
 * @pre      \ref CAN1_Initialize() function should be called before
 *           calling this function.
 * @param    none
 * @return   none  
 */
void CAN1_Sleep(void);

/**
 * @ingroup candriver
 * @brief   Returns the number of CAN messages received in all the FIFO
 * @pre     \ref CAN1_Initialize() function should be called before
 *          calling this function.
 * @param   none
 * @return  Number of messages received. 
 */
uint8_t CAN1_ReceivedMessageCountGet(void);

/**
 * @ingroup     candriver
 * @brief       This returns the CAN1 receive FIFO status
 * @pre         \ref CAN1_Initialize() function should be called before
 *              calling this function
 * @param[in]   fifoNum - FIFO channel of which status is required
 * @return      Combined status of the FIFO
 * @note        Use \ref CAN_RX_FIFO_STATUS to mask individual status.
*/
uint8_t CAN1_RX_FIFO_StatusGet(const enum CAN1_RX_FIFO_CHANNELS fifoNum);

/**
 * @ingroup  candriver
 * @brief    This routine is used to implement the tasks for polled implementations
 * @pre      CAN1_Initialize() function should have been 
 *           called before calling this function. 
 * @param    none
 * @return   none  
 */
void CAN1_Tasks(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_InvalidMessageCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_InvalidMessageCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_InvalidMessageCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_InvalidMessageCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_BusWakeUpActivityCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_BusWakeUpActivityCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_BusWakeUpActivityCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_BusWakeUpActivityCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_BusErrorCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_BusErrorCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_BusErrorCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_BusErrorCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_ModeChangeCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_ModeChangeCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_ModeChangeCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_ModeChangeCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_SystemErrorCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_SystemErrorCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_SystemErrorCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_SystemErrorCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_TxAttemptCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none    
 */
void CAN1_TxAttemptCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_TxAttemptCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_TxAttemptCallback(void);

/**
 * @ingroup    candriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for \ref CAN1_RxBufferOverFlowCallback.
 * @param[in]  handler - Address of the callback routine
 * @return     none  
 */
void CAN1_RxBufferOverFlowCallbackRegister(void (*handler)(void));

/**
 * @ingroup  candriver
 * @brief    This is the default callback with weak attribute. The user can 
 *           override and implement the default callback without weak attribute 
 *           or can register a custom callback function using \ref CAN1_RxBufferOverFlowCallbackRegister.
 * @param    none
 * @return   none  
 */
void CAN1_RxBufferOverFlowCallback(void);
#endif  //_CAN1_H
/**
 End of File
*/


