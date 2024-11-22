/**
 * CAN Generated Driver Types Header File
 * 
 * @file      can_types.h
 * 
 * @ingroup   candriver
 * 
 * @brief     This is the generated driver types header file for the CAN driver using CCL
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

#ifndef CAN_TYPES_H
#define CAN_TYPES_H

// Section: Included Files

#include <stdint.h>

/**
 @ingroup  candriver
 @struct   CAN_MSG_FIELD
 @brief    This data structure used to configure the message fields. 
           Some of the fields formatType and brs data should match with 
           the CAN FD configuration in Melody MCC user interface.
*/
struct CAN_MSG_FIELD 
{
    unsigned int idType:1;       /**< Defines the message ID type as Standard ID or Extended ID */
    unsigned int frameType:1;    /**< Defines the message frame type as Data Frame or RTR Frame */
    unsigned int dlc:4;          /**< Defines the size of the data bytes in a message format. The maximum date length is defined in MCC Melody */  
    unsigned int formatType:1;   /**< Defines the message type as CAN 2.0 Format or CAN_FD Format */
    unsigned int brs:1;          /**< Enables or Disables the Bit Rate Switch */
};


/**
 @ingroup  candriver
 @struct   CAN_MSG_OBJ
 @brief    This data structure used to configure the CAN FD message object
*/
struct CAN_MSG_OBJ
{
    uint32_t msgId;                 /**< Set the CAN Message ID */
    struct CAN_MSG_FIELD field;     /**< Defines the \ref CAN_MSG_FIELD CAN TX/RX Message Object Control */
    uint8_t *data;                  /**< Pointer to message data */
};   

/**
 @ingroup  candriver
 @enum     CAN_MSG_OBJ_BRS_MODE
 @brief    Defines the Bit rate with enabled or disabled the CAN message object
*/
enum CAN_MSG_OBJ_BRS_MODE 
{   
    CAN_NON_BRS_MODE    = 0,    /**< Disable BRS Mode (Supported only in CAN FD mode) */
    CAN_BRS_MODE        = 1     /**< Enable BRS Mode (Supported only in CAN FD mode) */
} ;

/**
 @ingroup  candriver
 @enum     CAN_MSG_OBJ_ID_TYPE
 @brief    Defines the Standard ID or Extended ID in CAN message object
*/
enum CAN_MSG_OBJ_ID_TYPE 
{   
    CAN_FRAME_STD       = 0,    /**< Standard ID CAN message object */
    CAN_FRAME_EXT       = 1,    /**< Extended ID CAN message object */
};

/**
 @ingroup  candriver
 @enum     CAN_MSG_OBJ_FRAME_TYPE
 @brief    Defines the Data frame or Remote transmit Request frame in CAN message object 
*/
enum CAN_MSG_OBJ_FRAME_TYPE
{   
    CAN_FRAME_DATA      = 0,    /**< Data Frame CAN message object */
    CAN_FRAME_RTR       = 1,    /**< Remote Transmit Request Frame CAN message object */
};

/**
 @ingroup  candriver
 @enum     CAN_MSG_OBJ_TYPE
 @brief    Defines the CAN FD format or CAN 2.0 format in CAN message object
*/
enum CAN_MSG_OBJ_TYPE
{   
    CAN_2_0_FORMAT      = 0,    /**< CAN 2.0 Message format */
    CAN_FD_FORMAT       = 1     /**< CAN FD Message format (Supported only in CAN FD mode) */
};

/**
 @ingroup  candriver
 @enum     CAN_TX_MSG_REQUEST_STATUS
 @brief    Defines the CAN transmit Api return status
*/
enum CAN_TX_MSG_REQUEST_STATUS
{ 
    CAN_TX_MSG_REQUEST_SUCCESS = 0,             /**< Transmit message object successfully placed into Transmit FIFO */
    CAN_TX_MSG_REQUEST_DLC_EXCEED_ERROR = 1,    /**< Transmit message object DLC size is more than Transmit FIFO configured DLC size */
    CAN_TX_MSG_REQUEST_BRS_ERROR = 2,           /**< Transmit FIFO is configured has Non BRS mode and CAN TX Message object has BRS enabled */
    CAN_TX_MSG_REQUEST_FIFO_FULL = 3,           /**< Transmit FIFO is Full */
};

/**
 @ingroup  candriver
 @enum     CAN_OP_MODES
 @brief    Defines the CAN operation modes are available for the module to use.
*/
enum CAN_OP_MODES
{
    CAN_NORMAL_FD_MODE = 0x0,           /**< CAN FD Normal Operation Mode (Supported only in CAN FD mode) */
    CAN_DISABLE_MODE = 0x1,             /**< CAN Disable Operation Mode */               
    CAN_INTERNAL_LOOPBACK_MODE = 0x2,   /**< CAN Internal Loopback Operation Mode */
    CAN_LISTEN_ONLY_MODE = 0x3,         /**< CAN Listen only Operation Mode */
    CAN_CONFIGURATION_MODE = 0x4,       /**< CAN Configuration Operation Mode */
    CAN_EXTERNAL_LOOPBACK_MODE = 0x5,   /**< CAN External loopback Operation Mode */
    CAN_NORMAL_2_0_MODE = 0x6,          /**< CAN 2.0 Operation Mode */
    CAN_RESTRICTED_OPERATION_MODE =0x7, /**< CAN Restricted Operation Mode */
};   

/**
 @ingroup  candriver
 @enum     CAN_OP_MODE_STATUS
 @brief    Defines the CAN operation set Api return status
*/
enum CAN_OP_MODE_STATUS
{
    CAN_OP_MODE_REQUEST_SUCCESS,     /**< Defines the requested operation mode set successfully */
    CAN_OP_MODE_REQUEST_FAIL,        /**< Defines the requested operation mode set failure. Set configuration mode before setting CAN normal or debug operation mode. */
    CAN_OP_MODE_SYS_ERROR_OCCURED    /**< Defines the system error occurred while setting operation mode. */
};

/**
 @ingroup  candriver
 @enum     CAN_TX_FIFO_STATUS
 @brief    Defines the CAN transmit status get Api return status
*/
enum CAN_TX_FIFO_STATUS
{
    CAN_TX_FIFO_FULL,       /**< Defines the Transmit FIFO is full */
    CAN_TX_FIFO_AVAILABLE,  /**< Defines the Transmit FIFO is available */
};

/**
 @ingroup  candriver
 @enum     CAN_DLC
 @brief    Defines the CAN message payload size that are available for the mode to use.
*/
enum CAN_DLC
{
    // DLC_0 to DLC_8 for CAN 2.0 and CAN FD
    DLC_0,  /**< Data length count 0 */
    DLC_1,  /**< Data length count 1 */  
    DLC_2,  /**< Data length count 2 */  
    DLC_3,  /**< Data length count 3 */  
    DLC_4,  /**< Data length count 4 */  
    DLC_5,  /**< Data length count 5 */  
    DLC_6,  /**< Data length count 6 */  
    DLC_7,  /**< Data length count 7 */  
    DLC_8,  /**< Data length count 8 */  

    // <Supported only in CAN FD mode
    // DLC_12 to DLC_64 for CAN FD
    DLC_12, /**< Data length count 12 */  
    DLC_16, /**< Data length count 16 */  
    DLC_20, /**< Data length count 20 */  
    DLC_24, /**< Data length count 24 */  
    DLC_32, /**< Data length count 32 */  
    DLC_48, /**< Data length count 48 */  
    DLC_64, /**< Data length count 64 */  
};

/**
 @ingroup  candriver
 @enum     CAN FD receive FIFO Status Enumeration
 @brief    This enumeration defines the CAN FD receive FIFO status
*/
enum CAN_RX_FIFO_STATUS 
{
    CAN_RX_MSG_NOT_AVAILABLE = 0x0,
    CAN_RX_MSG_AVAILABLE = 0x1,
    CAN_RX_MSG_OVERFLOW = 0x8
};

#endif  //CAN_TYPES_H
/**
 End of File
*/

