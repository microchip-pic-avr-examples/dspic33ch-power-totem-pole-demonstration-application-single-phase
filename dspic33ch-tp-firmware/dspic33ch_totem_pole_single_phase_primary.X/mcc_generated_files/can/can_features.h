/**
 * CAN Generated Feature Header File
 * 
 * @file      can_features.h
 *            
 * @ingroup   candriver
 *            
 * @brief     This is the generated module feature header file for CAN
 *            driver using CCL. This file provides module feature list available
 *            on the selected device. The macros defined in this file provides the flexibility
 *            to easily migrate the user application to other device which might have varied
 *            feature list.
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

#ifndef CAN_FEATURES
#define CAN_FEATURES
        
/*******************************************************************************
********************************************************************************
******************************************************************************** 
                            ********IMPORTANT********
This file defines the CAN module feature C-macros available 
on the selected device. The macros defined in this file provides the 
flexibility to easily migrate the user application to other device which might 
have varied feature list.

The file has to be manually included in main.c, if the user intends to migrate 
the application to another device which might have varied feature list.
  
  
 ***************The content in this file is strictly "read only" and should 
 not be altered. Failing to do so, the migration is not guaranteed.*************

********************************************************************************
********************************************************************************         
********************************************************************************/

/*******************************************************************************
            Macros defined for features supported in the device
*******************************************************************************/
/*!
  @ingroup  candriver
  @brief    This macro defines the CAN FD Message format data update request functionality.
 
  <b>APIs Supported:</b><br>
   CAN_TX_MSG_REQUEST_STATUS CANx_Transmit(const CANx_TX_FIFO_CHANNELS fifoChannel, CAN_MSG_OBJ *txCanMsg);
   bool CANx_Receive(CAN_MSG_OBJ *rxCanMsg);
   x denotes instance of CAN in CANx. 
   Refer to device specific datasheet to check number of CAN module instance.
   Refer driver header file for detailed description of the APIs.   
  
*/
#define CAN_FD_MESSAGE_FORMAT_FEATURE_AVAILABLE  1

/*! 
  @ingroup  candriver
  @brief    This macro defines the CAN FIFO based transmit priority data update request functionality.
 
  <b>APIs Supported:</b><br>
   CAN_TX_MSG_REQUEST_STATUS CANx_Transmit(const CANx_TX_FIFO_CHANNELS fifoChannel, CAN_MSG_OBJ *txCanMsg);
   CAN_TX_FIFO_STATUS CANx_TransmitFIFOStatusGet(const CANx_TX_FIFO_CHANNELS fifoChannel);
  
   x denotes instance of CAN in CANx. 
   Refer to device specific datasheet to check number of CAN module instance.
   Refer driver header file for detailed description of the APIs.
*/
#define CAN_FIFO_BASED_TRANSMIT_PRIORITY_FEATURE_AVAILABLE  1

/*******************************************************************************
            Macros defined for features not supported in the device
*******************************************************************************/
/*! 
  @ingroup  candriver
  @brief    This macro defines the CAN 2.0 DMA interface data update request functionality.
*/
#define CAN_DMA_INTERFACE_FEATURE_AVAILABLE  0

#endif //CAN_MODULE_FEATURES
