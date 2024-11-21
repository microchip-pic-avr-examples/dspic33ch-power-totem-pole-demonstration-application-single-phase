/**
 * CAN1 Generated Driver Source File
 * 
 * @file      can1.c
 *            
 * @ingroup   candriver
 *            
 * @brief     This is the generated driver source file for CAN1 driver using CCL
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

// Section: Included Files

#include <string.h>
#include "../can_types.h"    
#include "../can1.h"
#include "../../sources_common/Totempole_Application_Setup.h"

// CAN Bus FIFO Memory information
#define CAN1_FIFO_ALLOCATE_RAM_SIZE    144U // CAN FIFO allocated ram size based on (number of FIFO x FIFO message Payload size x Message object DLC size)

// CAN Bus receive FIFO Memory information
#define CAN1_NUM_OF_RX_FIFO            1U    // No of RX FIFO's configured
#define CAN1_RX_FIFO_MSG_DATA          64U   // CAN RX FIFO Message object data field size
#define CAN_RX_FIFO_WORD_0      0
#define CAN_RX_FIFO_WORD_1      1
#define CAN_RX_FIFO_WORD_2      2
#define CAN_RX_FIFO_WORD_4      4

// CAN Bus Transmit FIFO Memory information
#define CAN1_TX_MSG_SEND_REQ_BIT_POS   0x200U // CAN FIFO TX Message Send Request bit 
#define CAN1_TX_INC_FIFO_PTR_BIT_POS   0x100U // CAN FIFO Increment Head/Tail bit
#define CAN_TX_FIFO_WORD_0      0
#define CAN_TX_FIFO_WORD_1      1
#define CAN_TX_FIFO_WORD_2      2
#define CAN_TX_FIFO_WORD_4      4

// CAN Message object arbitration field information
#define CAN_MSG_OBJ_DLC_FIELD_SIZE          0xFU
#define CAN_MSG_OBJ_ID_TYPE_FIELD_POS       0x10U
#define CAN_MSG_OBJ_ID_TYPE_SHIFT_POS       0x4U
#define CAN_MSG_OBJ_RTR_SHIFT_POS           0x5U
#define CAN_MSG_OBJ_RTR_FIELD_POS           0x20U
#define CAN_MSG_OBJ_FRAME_TYPE_FIELD_POS    0x20U
#define CAN_MSG_OBJ_FRAME_TYPE_SHIFT_POS    0x5U
#define CAN_MSG_OBJ_BRS_FIELD_POS           0x40U
#define CAN_MSG_OBJ_BRS_SHIFT_POS           0x6U
#define CAN_MSG_OBJ_FORMAT_TYPE_FIELD_POS   0x80U
#define CAN_MSG_OBJ_FORMAT_TYPE_SHIFT_POS   0x7U
#define CAN_STD_MSG_ID_MAX_SIZE             0x7FFU
#define CAN_MSG_OBJ_SID_SHIFT_POS           0x12U
#define CAN_EXT_MSG_ID_HIGH_MAX_SIZE        0x1FFFU
#define CAN_EXT_MSG_ID_LOW_MAX_SIZE         0x1FU
#define CAN_MSG_OBJ_EID_LOW_SHIFT_POS       0xBU
#define CAN_MSG_OBJ_EID_HIGH_SHIFT_POS      0x5U

// Section: Driver Interface
const struct CAN_INTERFACE CAN_FD1 = {
    .Initialize = CAN1_Initialize,
    .Deinitialize = CAN1_Deinitialize,
    .OperationModeSet = CAN1_OperationModeSet,
    .OperationModeGet = CAN1_OperationModeGet,
    .IsBusOff = CAN1_IsBusOff,
    .SleepMode = CAN1_Sleep,
    .Transmit = CAN1_Transmit,
    .TransmitFIFOStatusGet = CAN1_TransmitFIFOStatusGet,
    .IsTxErrorActive = CAN1_IsTxErrorActive,
    .IsTxErrorPassive = CAN1_IsTxErrorPassive,
    .IsTxErrorWarning = CAN1_IsTxErrorWarning,
    .Receive = CAN1_Receive,
    .ReceiveMessageGet = CAN1_ReceiveMessageGet,
    .IsRxErrorPassive = CAN1_IsRxErrorPassive,
    .IsRxErrorWarning = CAN1_IsRxErrorWarning,
    .IsRxErrorActive = CAN1_IsRxErrorActive,
    .ReceivedMessageCountGet = CAN1_ReceivedMessageCountGet,
    .RX_FIFO_StatusGet = CAN1_RX_FIFO_StatusGet,
    .InvalidMessageCallbackRegister = &CAN1_InvalidMessageCallbackRegister,
    .BusWakeUpActivityCallbackRegister = &CAN1_BusWakeUpActivityCallbackRegister,
    .BusErrorCallbackRegister = &CAN1_BusErrorCallbackRegister,
    .ModeChangeCallbackRegister = &CAN1_ModeChangeCallbackRegister,
    .SystemErrorCallbackRegister = &CAN1_SystemErrorCallbackRegister,
    .TxAttemptCallbackRegister = &CAN1_TxAttemptCallbackRegister,
    .RxBufferOverFlowCallbackRegister = &CAN1_RxBufferOverFlowCallbackRegister,
    .Tasks = CAN1_Tasks
};

// Section: Private Variable Definitions
// Start CAN Message Memory Base Address
static uint8_t __attribute__((aligned(4)))can1FifoMsg[CAN1_FIFO_ALLOCATE_RAM_SIZE];

// CAN Default Callback Handler
static void (*CAN1_InvalidMessageHandler)(void) = NULL;
static void (*CAN1_BusWakeUpActivityHandler)(void) = NULL;
static void (*CAN1_BusErrorHandler)(void) = NULL;
static void (*CAN1_ModeChangeHandler)(void) = NULL;
static void (*CAN1_SystemErrorHandler)(void) = NULL;
static void (*CAN1_TxAttemptHandler)(void) = NULL;
static void (*CAN1_RxBufferOverFlowHandler)(void) = NULL;

// CAN Receive FIFO Message object data field 
static uint8_t rxMsgData[CAN1_RX_FIFO_MSG_DATA];

/**
 @ingroup  candriver
 @struct   CAN1 receive FIFO Message object head count information
           maintenance data structure
 @brief    This defines the object required for the maintenance of the receive FIFO message object
*/
struct CAN1_RX_FIFO_MSG
{
    const enum CAN1_RX_FIFO_CHANNELS channel;
    uint8_t headCount;
};

static volatile struct CAN1_RX_FIFO_MSG rxFIFOMsg[CAN1_NUM_OF_RX_FIFO] = 
{
    // Receive FIFO, FIFO head count
    {CAN1_FIFO_1, 0U},
}; 

/**
 @ingroup  candriver
 @struct   CAN1 FIFO parameters information data structure
 @brief    This Data structure is to implement a CAN FIFO parameters information
*/
struct CAN1_FIFO_INFO
{
    uint8_t payloadSize;
    uint8_t msgDeepSize;
    uint16_t *address;
};

// Section: Private Function Definitions

/**
 @ingroup  candriver
 @static   Get the DLC enum based decimal value
 @brief    This function get the DLC enum based decimal value
*/
static uint8_t CAN1_DlcToDataBytesGet(const enum CAN_DLC dlc)
{
    static const uint8_t dlcByteSize[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U};
    return dlcByteSize[dlc];
}

/**
 @ingroup  candriver
 @static   Get the FIFO user address, message depth and payload size information
 @brief    This function get the FIFO user address, message depth and payload size
           information.
*/

static void CAN1_FIFO_InfoGet(const uint8_t fifoNum, volatile struct CAN1_FIFO_INFO *fifoInfo)
{   
    switch (fifoNum) 
    {
        case CAN1_TXQ:
            fifoInfo->address = (uint16_t *) &C1TXQUAL;
            fifoInfo->payloadSize = 64U;
            fifoInfo->msgDeepSize = 1U;
            break;
     
        case CAN1_FIFO_1:
            fifoInfo->address = (uint16_t *) &C1FIFOUA1L;
            fifoInfo->payloadSize = 64U;
            fifoInfo->msgDeepSize = 1U;
            break;
     
        default:
            fifoInfo->address = NULL;
            fifoInfo->payloadSize = 0U;
            fifoInfo->msgDeepSize = 0U;
            break;
    }
}

/**
 @ingroup  candriver
 @static   Reset the CAN1 receive message head count
 @brief    This function reset the CAN1 receive message head count
*/
static void CAN1_RX_FIFO_ResetInfo(void)
{
    uint8_t count;
    
    for(count = 0; count < CAN1_NUM_OF_RX_FIFO; count++)
    {
        rxFIFOMsg[count].headCount = 0;
    }
}

/**
 @ingroup  candriver
 @static   Search the CAN1 receive FIFO channel index
 @brief    This function Search the CAN1 receive FIFO channel index
*/
static uint8_t CAN1_RxFifoSearch(uint8_t fifoNum)
{

    uint8_t rxFifoIndex;
    
    // If the last entry in the array holds the value
    if ((uint8_t)rxFIFOMsg[CAN1_NUM_OF_RX_FIFO - (uint8_t)1].channel == fifoNum) 
    {
        rxFifoIndex = CAN1_NUM_OF_RX_FIFO - (uint8_t)1;
    }
    else 
    {
        for (rxFifoIndex = 0; (uint8_t)rxFIFOMsg[rxFifoIndex].channel != fifoNum; rxFifoIndex++)
        {

        }
    }
    
    return rxFifoIndex;
}

/**
 @ingroup  candriver
 @static   Clear the CAN1 receive FIFO overflow status bit
 @brief    This function is used to clear the CAN1 receive FIFO overflow status bit
*/
static void CAN1_RX_FIFO_OverflowStatusFlagClear(const enum CAN1_RX_FIFO_CHANNELS fifoNum)
{   
    switch (fifoNum) 
    {
        case CAN1_FIFO_1:
            C1FIFOSTA1bits.RXOVIF = false;
            break;
            
        default:
            break;
    }
}

/**
 @ingroup  candriver
 @static   Update the receive FIFO message increment tail position
 @brief    This function Update the receive FIFO message increment tail position
*/
static void CAN1_RX_FIFO_IncrementMsgPtr(const uint8_t fifoNum) 
{
    switch (fifoNum) 
    {   
        case CAN1_FIFO_1:
            C1FIFOCON1Lbits.UINC = 1; // Update the CAN1_FIFO_1 message pointer.
            break;
        
        default:
            break;
    }    
}

/**
 @ingroup  candriver
 @static   Get the Receiver FIFO message index value
 @brief    This function get the Receiver FIFO message index value
*/
static uint16_t CAN1_RX_FIFO_MessageIndexGet(const enum CAN1_RX_FIFO_CHANNELS fifoNum) 
{
    uint16_t fifoMsgIndex;
    
    switch (fifoNum) 
    {            
        case CAN1_FIFO_1:
            fifoMsgIndex = C1FIFOSTA1bits.FIFOCI;
            break;

        default:
            fifoMsgIndex = 0;
            break;
    }
    
    return fifoMsgIndex;
}

/**
 @ingroup  candriver
 @static   Enable the transmit FIFO message send request bit
 @brief    This function enables The FIFO transmit message send request bit
*/
static void CAN1_TX_FIFO_MessageSendRequest(const enum CAN1_TX_FIFO_CHANNELS fifoChannel)
{   
    switch (fifoChannel) 
    {           
        case CAN1_TXQ:
            // Update the CAN1_TXQ message pointer; Set TXREQ bit
            C1TXQCONL = (C1TXQCONL | (CAN1_TX_MSG_SEND_REQ_BIT_POS | CAN1_TX_INC_FIFO_PTR_BIT_POS));
            break;

        default:
            break;
    }      
}

/**
 @ingroup  candriver
 @static   Read the message object from receive FIFO and update to the user message
           object pointer.
 @brief    This function read the message object from receive FIFO and update to the user  
           message object pointer.
*/
static void CAN1_MessageReadFromFifo(uint16_t *rxFifoObj, struct CAN_MSG_OBJ *rxCanMsg)
{   
    /*
    Receive FIFO Object format:
    
    Receive FIFO WORD_0:    |15:8|| EID<4:0>          | SID<10:8>              ||
                            | 7:0||           SID<7:0>                         ||
                             
    Receive FIFO WORD_1:    |15:8|| SID<11>           | EID<17:13>             ||
                            | 7:0|| EID<12:5>                                  ||
                             
    Receive FIFO WORD_2:    |15:8||FILHIT<4:0>  |   Reserved    |    ESI<1>    ||
                            | 7:0||FDF<1> | BRS<1> | RTR<1> | IDE<1> | DLC<3:0>||
                             
    Receive FIFO WORD_3:    |15:8||          Reserved                          ||
                            | 7:0||          Reserved                          ||
                          
    Receive FIFO WORD_4 to  |15:8||  Receive Data Byte 1,3,5.. n               || 
    Receive FIFO WORD_n:    | 7:0||  Receive Data Byte 0,2,4.. n-1             ||
    When timestamp is disabled
    */
    
    uint8_t dlcByteSize = 0;
        
    // SID <10:0> and EID <4:0>
    uint16_t rx0Data = rxFifoObj[CAN_RX_FIFO_WORD_0];

    // SID11 and EID <17:5>
    uint16_t rx1Data = rxFifoObj[CAN_RX_FIFO_WORD_1];

    // DLC <3:0>, IDE <1>, RTR <1>, BRS <1>, FDF <1> 
    rxCanMsg->field.dlc = (rxFifoObj[CAN_RX_FIFO_WORD_2] & CAN_MSG_OBJ_DLC_FIELD_SIZE);
    rxCanMsg->field.idType = ((rxFifoObj[CAN_RX_FIFO_WORD_2] & CAN_MSG_OBJ_ID_TYPE_FIELD_POS) >> CAN_MSG_OBJ_ID_TYPE_SHIFT_POS);
    rxCanMsg->field.frameType = ((rxFifoObj[CAN_RX_FIFO_WORD_2] & CAN_MSG_OBJ_FRAME_TYPE_FIELD_POS) >> CAN_MSG_OBJ_FRAME_TYPE_SHIFT_POS);
    rxCanMsg->field.brs = ((rxFifoObj[CAN_RX_FIFO_WORD_2] & CAN_MSG_OBJ_BRS_FIELD_POS) >> CAN_MSG_OBJ_BRS_SHIFT_POS);
    rxCanMsg->field.formatType = ((rxFifoObj[CAN_RX_FIFO_WORD_2] & CAN_MSG_OBJ_FORMAT_TYPE_FIELD_POS) >> CAN_MSG_OBJ_FORMAT_TYPE_SHIFT_POS);

    /* message is standard identifier */
    if(rxCanMsg->field.idType == (uint8_t) CAN_FRAME_STD) 
    {
        // SID <10:0>
        rxCanMsg->msgId = ((uint32_t)rx0Data & CAN_STD_MSG_ID_MAX_SIZE);   
    }
    else 
    {
        /* message is extended identifier */
        // EID <28:18>, EID <17:0>
        rxCanMsg->msgId = ((((uint32_t)rx0Data & CAN_STD_MSG_ID_MAX_SIZE) << CAN_MSG_OBJ_SID_SHIFT_POS) | 
                            (((uint32_t)rx1Data & CAN_EXT_MSG_ID_HIGH_MAX_SIZE) << CAN_MSG_OBJ_EID_HIGH_SHIFT_POS) | 
                            (((uint32_t)rx0Data >> CAN_MSG_OBJ_EID_LOW_SHIFT_POS) & CAN_EXT_MSG_ID_LOW_MAX_SIZE));
    }

    dlcByteSize = CAN1_DlcToDataBytesGet(rxCanMsg->field.dlc);

    // Coping receive FIFO data starting memory location
    (void)memset(rxMsgData, 0, CAN1_RX_FIFO_MSG_DATA);
    (void)memcpy((char *) rxMsgData, (char *) (&rxFifoObj[CAN_RX_FIFO_WORD_4]), dlcByteSize);
    rxCanMsg->data = rxMsgData;
}

/**
 @ingroup  candriver
 @static   Read the message object from user input and update to the CAN1 TX FIFO
 @brief    This function Read the message object from user input and update
           to the CAN1 TX FIFO.
*/
static void CAN1_MessageWriteToFifo(uint16_t *txFifoObj, struct CAN_MSG_OBJ *txCanMsg)
{
    /*
    Transmit FIFO Object format:
    
    Transmit FIFO WORD_0:    |15:8|| EID<4:0>          | SID<10:8>              ||
                             | 7:0||           SID<7:0>                         ||
                             
    Transmit FIFO WORD_1:    |15:8|| SID<11>           | EID<17:13>             ||
                             | 7:0|| EID<12:5>                                  ||
                             
    Transmit FIFO WORD_2:    |15:8||Sequence<6:0>(Not implemented) |  ESI<1>    ||
                             | 7:0||FDF<1> | BRS<1> | RTR<1> | IDE<1> | DLC<3:0>||
                             
    Transmit FIFO WORD_3:    |15:8||Sequence<22:15>(Not implemented)            ||
                             | 7:0||Sequence<14:7>(Not implemented)             ||
                          
    Transmit FIFO WORD_4 to  |15:8|| Transmit Data Byte 1,3,5.. n               ||
    Transmit FIFO WORD_n:    | 7:0|| Transmit Data Byte 0,2,4.. n-1             ||
    */
    
    uint8_t dlcByteSize = 0;

    /* message is standard identifier */
    if(txCanMsg->field.idType == (uint8_t) CAN_FRAME_STD) 
    {
        // SID <10:0>
        txFifoObj[CAN_TX_FIFO_WORD_0] = (txCanMsg->msgId & CAN_STD_MSG_ID_MAX_SIZE);
    } 
    else 
    {
        /* message is extended identifier */
        // SID <10:0> and EID <4:0>
        txFifoObj[CAN_TX_FIFO_WORD_0] = (((txCanMsg->msgId >> CAN_MSG_OBJ_SID_SHIFT_POS) & CAN_STD_MSG_ID_MAX_SIZE) | 
                        (txCanMsg->msgId & CAN_EXT_MSG_ID_LOW_MAX_SIZE) << CAN_MSG_OBJ_EID_LOW_SHIFT_POS);

        // EID <5:17>
        txFifoObj[CAN_TX_FIFO_WORD_1] = ((txCanMsg->msgId >>  CAN_MSG_OBJ_EID_HIGH_SHIFT_POS) & CAN_EXT_MSG_ID_HIGH_MAX_SIZE);
    }

    // DLC <3:0>, IDE <1>, RTR <1>, BRS <1>, FDF <1> 
    txFifoObj[CAN_TX_FIFO_WORD_2] = (txCanMsg->field.dlc & CAN_MSG_OBJ_DLC_FIELD_SIZE) | 
                    ((txCanMsg->field.idType << CAN_MSG_OBJ_ID_TYPE_SHIFT_POS) & CAN_MSG_OBJ_ID_TYPE_FIELD_POS) | 
                    ((txCanMsg->field.frameType << CAN_MSG_OBJ_FRAME_TYPE_SHIFT_POS) & CAN_MSG_OBJ_FRAME_TYPE_FIELD_POS) | 
                    ((txCanMsg->field.brs << CAN_MSG_OBJ_BRS_SHIFT_POS) & CAN_MSG_OBJ_BRS_FIELD_POS) | 
                    ((txCanMsg->field.formatType << CAN_MSG_OBJ_FORMAT_TYPE_SHIFT_POS) & CAN_MSG_OBJ_FORMAT_TYPE_FIELD_POS);
        
    // Data frame message
    if(txCanMsg->field.frameType == (uint8_t) CAN_FRAME_DATA)
    {
        dlcByteSize = CAN1_DlcToDataBytesGet(txCanMsg->field.dlc);

        // Coping TX message object to FIFO
        (void)memcpy((uint8_t*)(&txFifoObj[CAN_TX_FIFO_WORD_4]), txCanMsg->data, dlcByteSize);
    } 
    // RTR frame message
    else
    {
        txFifoObj[CAN_TX_FIFO_WORD_2] = txFifoObj[CAN_TX_FIFO_WORD_2] | (((bool) 1 << CAN_MSG_OBJ_RTR_SHIFT_POS) & CAN_MSG_OBJ_RTR_FIELD_POS);
    }
}

/**
 @ingroup  candriver
 @static   Configure the CAN1 transmit FIFO settings
 @brief    This function configure the CAN1 transmit FIFO settings
*/
static void CAN1_TX_FIFO_Configuration(void)
{
    // TXAT Unlimited attempts; PLSIZE 64; FSIZE 1; TXPRI 0; 
    C1TXQCONH = 0xE040;
    // TXQEIE disabled; TXREQ disabled; TXQNIE disabled; TXATIE disabled; UINC disabled; FRESET enabled; 
    C1TXQCONL = 0x480;
}

/**
 @ingroup  candriver
 @static   Configure the CAN1 receive FIFO settings
 @brief    This function configure the CAN1 receive FIFO settings
*/

static void CAN1_RX_FIFO_Configuration(void)
{          
    // TXAT Disabled; PLSIZE 64; FSIZE 1; TXPRI 0; 
    C1FIFOCON1H = 0xE000;
    // TFHRFHIE disabled; TFERFFIE disabled; RXTSEN disabled; TXREQ disabled; RXOVIE disabled; RTREN disabled; TXEN disabled; TXATIE disabled; UINC disabled; FRESET enabled; TFNRFNIE disabled; 
    C1FIFOCON1L = 0x400;
}

/**
 @ingroup  candriver
 @static   Configure the CAN1 filter and mask settings
 @brief    This function configure the CAN1 filter and mask settings
*/
static void CAN1_RX_FIFO_FilterMaskConfiguration(void)
{
    /* Configure RX FIFO Filter control settings*/
    
    // message stored in FIFO1
    C1FLTCON0Lbits.F0BP = 1;
    // EID 0; SID 515; 
//    C1FLTOBJ0L = 0x203;       //TODO: manually removed
    C1FLTOBJ0L = CANReceiveID;  //TODO: manually added
    // EID 0; EXIDE disabled; SID11 disabled; 
    C1FLTOBJ0H = 0x0;
    // MSID 2047; MEID 0; 
    C1MASK0L = 0x7FF;
    // MEID 0; MSID11 disabled; MIDE enabled; 
    C1MASK0H = 0x4000;
    // Enable the filter 0
    C1FLTCON0Lbits.FLTEN0 = 1;
}

/**
 @ingroup  candriver
 @static   Configure the CAN1 bit rate settings
 @brief    This function configure the CAN1 bit rate settings
*/
static void CAN1_BitRateConfiguration(void)
{
    // BRP 0; TSEG1 30; 
    C1NBTCFGH = 0x1E;
    // SJW 7; TSEG2 7; 
    C1NBTCFGL = 0x707;
    // BRP 0; TSEG1 6; 
    C1DBTCFGH = 0x6;
    // SJW 1; TSEG2 1; 
    C1DBTCFGL = 0x101;
    // EDGFLTEN disabled; TDCMOD Auto; SID11EN disabled; 
    C1TDCH = 0x2;
    // TDCV 0x0; TDCO 7; 
    C1TDCL = 0x700;
}

/**
 @ingroup  candriver
 @static   Enable the CAN1 Error Notification interrupt
 @brief    This function enables the CAN1  error notification  interrupt
*/
static void CAN1_ErrorNotificationEnable(void)
{
    // CAN1 CAN1 Callback initialize
    CAN1_InvalidMessageCallbackRegister(&CAN1_InvalidMessageCallback);
    CAN1_BusWakeUpActivityCallbackRegister(&CAN1_BusWakeUpActivityCallback);
    CAN1_BusErrorCallbackRegister(&CAN1_BusErrorCallback);
    CAN1_ModeChangeCallbackRegister(&CAN1_ModeChangeCallback);
    CAN1_SystemErrorCallbackRegister(&CAN1_SystemErrorCallback);
    CAN1_TxAttemptCallbackRegister(&CAN1_TxAttemptCallback);
    CAN1_RxBufferOverFlowCallbackRegister(&CAN1_RxBufferOverFlowCallback);
}

// Section: Driver Interface Function Definitions
void CAN1_Initialize(void)
{
    /* Enable the CAN1 module */
    C1CONLbits.CON = 1;
    
    // RTXAT disabled; ESIGM disabled; TXBWS No delay; STEF disabled; SERRLOM disabled; ABAT disabled; REQOP Configuration mode; TXQEN enabled; 
    C1CONH = 0x490;  

    /* Place CAN1 module in configuration mode */
    if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
    {
        /* Initialize the C1FIFOBAL with the start address of the CAN1 FIFO message object area. */
        C1FIFOBAL = (uint16_t) &can1FifoMsg[0];
        
        // BRSDIS disabled; CON enabled; WAKFIL enabled; WFT T11 Filter; ISOCRCEN enabled; SIDL disabled; DNCNT 0x0; PXEDIS enabled; CLKSEL disabled; 
        C1CONL = 0x8760;
    
        // Disabled CAN1 Store in Transmit Event FIFO bit
        C1CONHbits.STEF = 0;
        // Enabled CAN1 Transmit Queue bit
        C1CONHbits.TXQEN = 1;
        
        /* configure CAN1 Bit rate settings */
        CAN1_BitRateConfiguration();        
        
        /* configure CAN1 FIFO settings */
        CAN1_TX_FIFO_Configuration();
        CAN1_RX_FIFO_Configuration();
        
        /* Configure Receive FIFO Filter and Mask settings */
        CAN1_RX_FIFO_FilterMaskConfiguration();    

        /* CAN Error Notification */
        CAN1_ErrorNotificationEnable();
        
        // Reset the CAN1 receive FIFO head count
        CAN1_RX_FIFO_ResetInfo();
        
        /* Place CAN1 module in Normal Operation mode */
        (void)CAN1_OperationModeSet(CAN_NORMAL_FD_MODE);    
    }
}

void CAN1_Deinitialize(void)
{
    /* Place CAN1 module in configuration mode */
    if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
    {        
        C1CONL = 0x760;  
        C1CONH = 0x498;  
           
        /* Reset bit rate settings to POR*/
        C1NBTCFGH = 0x3E;
        C1NBTCFGL = 0xF0F;
        C1DBTCFGH = 0xE;
        C1DBTCFGL = 0x303;
        C1TDCH = 0x2;
        C1TDCL = 0x1000;
        
        /* configure CAN1 FIFO settings */
        /* Reset TX FIFO settings to POR*/
        C1TXQCONH = 0x60;
        C1TXQCONL = 0x480;
        
        /* Reset RX FIFO settings to POR*/
        C1FIFOCON1H = 0x60;
        C1FIFOCON1L = 0x400;
        
        /* Reset RX FIFO Filter control settings to POR*/
        C1FLTCON0Lbits.F0BP = 0x0;
        C1FLTOBJ0L = 0x0;
        C1FLTOBJ0H = 0x0;
        C1MASK0L = 0x0;
        C1MASK0H = 0x0;
        C1FLTCON0Lbits.FLTEN0 = 0x0;

    }
    
    /* Disable the CAN1 module */
    C1CONLbits.CON = 0;
}

enum CAN_OP_MODE_STATUS CAN1_OperationModeSet(const enum CAN_OP_MODES requestMode) 
{
    enum CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
    
    if((CAN_CONFIGURATION_MODE == CAN1_OperationModeGet()) || (requestMode == CAN_DISABLE_MODE)
            || (requestMode == CAN_CONFIGURATION_MODE))
    {
        C1CONHbits.REQOP = requestMode;

        while(C1CONHbits.OPMOD != requestMode) 
        {
            // This condition is avoiding the system error case endless loop
            if(C1INTLbits.SERRIF == 1)
            {
                status = CAN_OP_MODE_SYS_ERROR_OCCURED;
                break;
            }
        }
    }
    else
    {
        status = CAN_OP_MODE_REQUEST_FAIL;
    }
    
    return status;
}

enum CAN_OP_MODES CAN1_OperationModeGet(void) 
{
    return C1CONHbits.OPMOD;
}

bool CAN1_Receive(struct CAN_MSG_OBJ *rxCanMsg)
{
    uint8_t fifoChannel;
    uint8_t count;
    uint8_t rxMsgStatus;
    volatile struct CAN1_FIFO_INFO fifoInfo;
    bool status = false;
    
    // Iterate all receive FIFO's and read the message object
    for(count = 0; count < CAN1_NUM_OF_RX_FIFO; count++)
    {
        fifoChannel = rxFIFOMsg[count].channel;
        CAN1_FIFO_InfoGet(fifoChannel, &fifoInfo);
        rxMsgStatus = CAN1_RX_FIFO_StatusGet(fifoChannel);
        
        // If message object is available
        if((uint8_t)CAN_RX_MSG_AVAILABLE == (rxMsgStatus & (uint8_t)CAN_RX_MSG_AVAILABLE)) 
        {           
            if((uint16_t *)(*(fifoInfo.address)) != NULL)
            {
                CAN1_MessageReadFromFifo((uint16_t *) *fifoInfo.address, rxCanMsg);
                CAN1_RX_FIFO_IncrementMsgPtr(fifoChannel);
                
                // Update the RX FIFO Head count for CAN1_ReceivedMessageCountGet function
                rxFIFOMsg[count].headCount++; // Update the read one message
                if(rxFIFOMsg[count].headCount >= fifoInfo.msgDeepSize) 
                {
                    rxFIFOMsg[count].headCount = 0; // Reset the read message count
                }
                                
                // User have to clear manually RX Overflow status
                if((uint8_t)CAN_RX_MSG_OVERFLOW == (rxMsgStatus & (uint8_t)CAN_RX_MSG_OVERFLOW))
                {
                    CAN1_RX_FIFO_OverflowStatusFlagClear(fifoChannel);
                }
                
                status = true;
            }
            
            break;
        }
    }   
    
    return status;
}

bool CAN1_ReceiveMessageGet(const enum CAN1_RX_FIFO_CHANNELS fifoChannel, struct CAN_MSG_OBJ *rxCanMsg)
{
    uint8_t count;
    uint8_t rxMsgStatus;
    struct CAN1_FIFO_INFO fifoInfo;
    bool status = false;
    
    CAN1_FIFO_InfoGet(fifoChannel, &fifoInfo);
    rxMsgStatus = CAN1_RX_FIFO_StatusGet(fifoChannel);
    count = CAN1_RxFifoSearch(fifoChannel);

    // If message object is available
    if((uint8_t)CAN_RX_MSG_AVAILABLE == (rxMsgStatus & (uint8_t)CAN_RX_MSG_AVAILABLE)) 
    {           
        if((uint16_t *)(*(fifoInfo.address)) != NULL)
        {
            CAN1_MessageReadFromFifo((uint16_t *) *fifoInfo.address, rxCanMsg);
            CAN1_RX_FIFO_IncrementMsgPtr(fifoChannel);

            // Update the RX FIFO Head count for CAN1_ReceivedMessageCountGet function
            rxFIFOMsg[count].headCount++; // Update the read one message
            if(rxFIFOMsg[count].headCount >= fifoInfo.msgDeepSize) 
            {
                rxFIFOMsg[count].headCount = 0; // Reset the read message count
            }

            // User have to clear manually RX Overflow status
            if((uint8_t)CAN_RX_MSG_OVERFLOW == (rxMsgStatus & (uint8_t)CAN_RX_MSG_OVERFLOW))
            {
                CAN1_RX_FIFO_OverflowStatusFlagClear(fifoChannel);
            }

            status = true;
        }
    }
    
    return status;
}

enum CAN_TX_MSG_REQUEST_STATUS CAN1_Transmit(const enum CAN1_TX_FIFO_CHANNELS fifoChannel, struct CAN_MSG_OBJ *txCanMsg)
{
    volatile struct CAN1_FIFO_INFO fifoInfo;
    CAN1_FIFO_InfoGet(fifoChannel, &fifoInfo);
    enum CAN_TX_MSG_REQUEST_STATUS txMsgStatus = CAN_TX_MSG_REQUEST_SUCCESS;
    enum CAN_OP_MODES canOpModeStatus = CAN1_OperationModeGet();
    
    // If CAN module is configured in Non-BRS mode and TX message object has BRS set
    if((txCanMsg->field.brs == (bool) 1) && (C1CONLbits.BRSDIS == (bool) 1 || (CAN_NORMAL_2_0_MODE == canOpModeStatus)))
    {
        txMsgStatus |= CAN_TX_MSG_REQUEST_BRS_ERROR;
    }
    
    // If CAN 2.0 mode, Tx Message object has more than 8 bytes of DLC Size
    if((CAN_NORMAL_2_0_MODE == canOpModeStatus) || (txCanMsg->field.formatType == (uint8_t) CAN_2_0_FORMAT))
    { 
        // CAN 2.0 mode DLC supports upto 8 byte 
        if(txCanMsg->field.dlc > (uint8_t) DLC_8) 
        {
           txMsgStatus |= CAN_TX_MSG_REQUEST_DLC_EXCEED_ERROR;
        }
    }
    
    // If any CAN TX message object has DLC size more than CAN TX FIFO Payload size
    if(CAN1_DlcToDataBytesGet(txCanMsg->field.dlc) > fifoInfo.payloadSize) 
    {
        txMsgStatus |= CAN_TX_MSG_REQUEST_DLC_EXCEED_ERROR;
    }
    
    if(CAN_TX_MSG_REQUEST_SUCCESS == txMsgStatus)
    {
        if(CAN_TX_FIFO_AVAILABLE == CAN1_TransmitFIFOStatusGet(fifoChannel))
        {
            if((uint16_t *)(*(fifoInfo.address)) != NULL) 
            {
                CAN1_MessageWriteToFifo((uint16_t *) *fifoInfo.address, txCanMsg);
                CAN1_TX_FIFO_MessageSendRequest(fifoChannel);
            }         
        }
        else
        {
            txMsgStatus |= CAN_TX_MSG_REQUEST_FIFO_FULL;
        }
    }

    return txMsgStatus;
}

enum CAN_TX_FIFO_STATUS CAN1_TransmitFIFOStatusGet(const enum CAN1_TX_FIFO_CHANNELS fifoChannel)
{
    enum CAN_TX_FIFO_STATUS fifoStatus;
    
    switch (fifoChannel) 
    {
        case CAN1_TXQ:
            fifoStatus = ((C1TXQSTA & 0x1) ? CAN_TX_FIFO_AVAILABLE:CAN_TX_FIFO_FULL);
            break;
            
        default:
            fifoStatus = CAN_TX_FIFO_FULL;
            break;
    }
    
    return fifoStatus;
}

uint8_t CAN1_ReceivedMessageCountGet(void)
{
    uint8_t fifoChannel = 0;
    uint8_t count = 0;
    uint8_t numOfMsg = 0;
    uint8_t totalMsgObj = 0;
    uint8_t rxMsgStatus;
    struct CAN1_FIFO_INFO fifoInfo;
    uint16_t rxfifoMsgTail;
    
    // Iterate all receive FIFO's and get the message object count
    for(count = 0; count < CAN1_NUM_OF_RX_FIFO; count++)
    {
        fifoChannel = rxFIFOMsg[count].channel;
        CAN1_FIFO_InfoGet(fifoChannel, &fifoInfo);
        rxMsgStatus = CAN1_RX_FIFO_StatusGet(fifoChannel);
        
        // If message object is available
        if((uint8_t)CAN_RX_MSG_AVAILABLE == (rxMsgStatus & (uint8_t)CAN_RX_MSG_AVAILABLE)) 
        {
            // If receive FIFO overflow has occurred, FIFO is full 
            if((uint8_t)CAN_RX_MSG_OVERFLOW == (rxMsgStatus & (uint8_t)CAN_RX_MSG_OVERFLOW))
            {
                numOfMsg = fifoInfo.msgDeepSize;
            } 
            else
            {
                rxfifoMsgTail = CAN1_RX_FIFO_MessageIndexGet(fifoChannel);
                
                if(rxfifoMsgTail < rxFIFOMsg[count].headCount)
                {
                    numOfMsg = ((rxfifoMsgTail + fifoInfo.msgDeepSize) - rxFIFOMsg[count].headCount);
                }
                else if(rxfifoMsgTail > rxFIFOMsg[count].headCount)
                {
                    numOfMsg = rxfifoMsgTail - rxFIFOMsg[count].headCount;
                }
                else
                {
                    numOfMsg = fifoInfo.msgDeepSize;
                }
            }
            
            totalMsgObj += numOfMsg;
        }
    }
    
    return totalMsgObj;
}

uint8_t CAN1_RX_FIFO_StatusGet(const enum CAN1_RX_FIFO_CHANNELS fifoNum)
{
    uint8_t rxFifoStatus;
    
    switch (fifoNum) 
    {
        case CAN1_FIFO_1:
            rxFifoStatus = (uint8_t)(C1FIFOSTA1 & (CAN_RX_MSG_AVAILABLE | CAN_RX_MSG_OVERFLOW));
            break;

        default:
            rxFifoStatus = (uint8_t)CAN_RX_MSG_NOT_AVAILABLE;
            break;
    }
    
    return rxFifoStatus;
}

bool CAN1_IsBusOff(void)
{
    return C1TRECHbits.TXBO;
}

bool CAN1_IsRxErrorPassive(void)
{
    return C1TRECHbits.RXBP;
}

bool CAN1_IsRxErrorWarning(void)
{
    return C1TRECHbits.RXWARN;
}

bool CAN1_IsRxErrorActive(void)
{
    bool errorState = false;
    if((0 < C1TRECLbits.RERRCNT) && (C1TRECLbits.RERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

bool CAN1_IsTxErrorPassive(void) 
{
    return C1TRECHbits.TXBP;
}

bool CAN1_IsTxErrorWarning(void) 
{
    return C1TRECHbits.TXWARN;
}

bool CAN1_IsTxErrorActive(void)
{
    bool errorState = false;
    if((0 < C1TRECLbits.TERRCNT) && (C1TRECLbits.TERRCNT < 128)) 
    {
        errorState = true;
    }
    
    return errorState;
}

void CAN1_Sleep(void)
{
    C1INTLbits.WAKIF = 0;
    C1INTHbits.WAKIE = 1;
    
    // CAN Info Interrupt Enable bit
    IEC1bits.C1IE = 1;  
    
    /* put the module in disable mode */
   (void)CAN1_OperationModeSet(CAN_DISABLE_MODE);
}

void CAN1_InvalidMessageCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_InvalidMessageHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_InvalidMessageCallback ( void )
{ 

} 

void CAN1_BusWakeUpActivityCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_BusWakeUpActivityHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_BusWakeUpActivityCallback ( void )
{ 

} 

void CAN1_BusErrorCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_BusErrorHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_BusErrorCallback ( void )
{ 

} 

void CAN1_ModeChangeCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_ModeChangeHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_ModeChangeCallback ( void )
{ 

} 

void CAN1_SystemErrorCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_SystemErrorHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_SystemErrorCallback ( void )
{ 

} 

void CAN1_TxAttemptCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_TxAttemptHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_TxAttemptCallback(void)
{ 

}

void CAN1_RxBufferOverFlowCallbackRegister(void (*handler)(void))
{
    if(NULL != handler)
    {
        CAN1_RxBufferOverFlowHandler = handler;
    }
}

void __attribute__ ((weak)) CAN1_RxBufferOverFlowCallback(void)
{ 

} 

void __attribute__((__interrupt__, no_auto_psv)) _C1Interrupt(void)
{
    // Bus Wake-up Activity Interrupt 
    if(1 == C1INTLbits.WAKIF)
    {
        if(CAN1_BusWakeUpActivityHandler != NULL)
        {
            CAN1_BusWakeUpActivityHandler();
        }
        
        C1INTLbits.WAKIF = 0;
    }
    
    IFS1bits.C1IF = 0;
}

void CAN1_Tasks(void)
{
    if(1 == C1INTLbits.IVMIF)
    {
        if(CAN1_InvalidMessageHandler != NULL)
        {
            CAN1_InvalidMessageHandler();
        }
       
        C1INTLbits.IVMIF = 0;
    }
    
    if(1 == C1INTLbits.CERRIF)
    {
        if(CAN1_BusErrorHandler != NULL)
        {
            CAN1_BusErrorHandler();
        }
       
        C1INTLbits.CERRIF = 0;
    }
    
    if(1 == C1INTLbits.MODIF)
    {
        if(CAN1_ModeChangeHandler != NULL)
        {
            CAN1_ModeChangeHandler();
        }
       
        C1INTLbits.MODIF = 0;
    }
    
    if(1 == C1INTLbits.SERRIF)
    {
        if(CAN1_SystemErrorHandler != NULL)
        {
            CAN1_SystemErrorHandler();
        }
       
        C1INTLbits.SERRIF = 0;
    }
    
    if(1 == C1INTLbits.TXATIF)
    {
        if(CAN1_TxAttemptHandler != NULL)
        {
            CAN1_TxAttemptHandler();
        }
    }
    
    if(1 == C1FIFOSTA1bits.RXOVIF)
    {
        if(CAN1_RxBufferOverFlowHandler != NULL)
        {
            CAN1_RxBufferOverFlowHandler();
        }
        
        C1FIFOSTA1bits.RXOVIF = 0;
    }
    
}

/**
 End of File
*/


