/**
 * DMA Generated Driver Header File
 * 
 * @file      dma.h
 * 
 * @ingroup   dmadriver
 * 
 * @brief     This is the generated driver header file for the DMA driver
 *
 * @skipline @version   Firmware Driver Version 1.1.4
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

#ifndef DMA_H
#define DMA_H

// Section: Included Files

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include "dma_types.h"
#include "dma_interface.h"
       
// Section: Data Type Definitions

/**
 * @ingroup  dmadriver
 * @brief    Structure object of type DMA_INTERFACE with the custom name
 *           given by the user in the Melody Driver User interface. The default name 
 *           e.g. DMA can be changed by the user in the DMA user interface. 
 *           This allows defining a structure with application specific name using 
 *           the 'Custom Name' field. Application specific name allows the API Portability.
*/
extern const struct DMA_INTERFACE DMA_SPI;

/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_Initialize API
 */
#define DMA_SPI_Initialize DMA_Initialize
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_Deinitialize API
 */
#define DMA_SPI_Deinitialize DMA_Deinitialize
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_ChannelEnable API
 */
#define DMA_SPI_ChannelEnable DMA_ChannelEnable
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_TransferCountSet API
 */
#define DMA_SPI_TransferCountSet DMA_TransferCountSet
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_TransferCountGet API
 */
#define DMA_SPI_TransferCountGet DMA_TransferCountGet
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_SoftwareTriggerEnable API
 */
#define DMA_SPI_SoftwareTriggerEnable DMA_SoftwareTriggerEnable
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_SourceAddressSet API
 */
#define DMA_SPI_SourceAddressSet DMA_SourceAddressSet
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_DestinationAddressSet API
 */
#define DMA_SPI_DestinationAddressSet DMA_DestinationAddressSet
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_IsSoftwareRequestPending API
 */
#define DMA_SPI_IsSoftwareRequestPending DMA_IsSoftwareRequestPending
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_ChannelTasks API
 */
#define DMA_SPI_ChannelTasks DMA_ChannelTasks
/**
 * @ingroup  dmadriver
 * @brief    This macro defines the Custom Name for \ref DMA_ChannelCallbackRegister API
 */
#define DMA_SPI_ChannelCallbackRegister DMA_ChannelCallbackRegister

// Section: Driver Interface Functions

/**
 * @ingroup  dmadriver
 * @brief    Initializes the DMA module
 * @param    none
 * @return   none
 */
void DMA_Initialize (void);

/**
 * @ingroup  dmadriver
 * @brief    Deinitializes the DMA to POR values
 * @param    none
 * @return   none
 */
void DMA_Deinitialize(void);
    
/**
 * @ingroup    dmadriver
 * @brief      This inline function enables the DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel	- DMA channel number
 * @return     none
 */
inline static void DMA_ChannelEnable(enum DMA_CHANNEL channel)
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
                DMACH0bits.CHEN = 1;
                break; 
        case DMA_CHANNEL_1:
                DMACH1bits.CHEN = 1;
                break; 
        case DMA_CHANNEL_2:
                DMACH2bits.CHEN = 1;
                break; 
        case DMA_CHANNEL_3:
                DMACH3bits.CHEN = 1;
                break; 
        case DMA_CHANNEL_4:
                DMACH4bits.CHEN = 1;
                break; 
        case DMA_CHANNEL_5:
                DMACH5bits.CHEN = 1;
                break; 
        default: break;
    }
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function disables the DMA channel
 * @pre        DMA_Initialize must be called. 
 * @param[in]  channel	- DMA channel number
 * @return     none
 */
inline static void DMA_ChannelDisable(enum DMA_CHANNEL channel)
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
                DMACH0bits.CHEN = 0;
                break;    
        case DMA_CHANNEL_1:
                DMACH1bits.CHEN = 0;
                break;    
        case DMA_CHANNEL_2:
                DMACH2bits.CHEN = 0;
                break;    
        case DMA_CHANNEL_3:
                DMACH3bits.CHEN = 0;
                break;    
        case DMA_CHANNEL_4:
                DMACH4bits.CHEN = 0;
                break;    
        case DMA_CHANNEL_5:
                DMACH5bits.CHEN = 0;
                break;    
        default: break;
    }
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function sets the number of transfer counts to DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel	        - DMA channel number
 * @param[in]  transferCount	- Transfer count Value
 * @return     none
 */
inline static void DMA_TransferCountSet(enum DMA_CHANNEL channel, uint16_t transferCount)
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
				DMACNT0 = transferCount;
                
                break;
        case DMA_CHANNEL_1:
				DMACNT1 = transferCount;
                
                break;
        case DMA_CHANNEL_2:
				DMACNT2 = transferCount;
                
                break;
        case DMA_CHANNEL_3:
				DMACNT3 = transferCount;
                
                break;
        case DMA_CHANNEL_4:
				DMACNT4 = transferCount;
                
                break;
        case DMA_CHANNEL_5:
				DMACNT5 = transferCount;
                
                break;
        default: break;
    }
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function returns the number of transfer counts from DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel	- DMA channel number
 * @return     Returns the transfer count value
 */
inline static uint16_t DMA_TransferCountGet(enum DMA_CHANNEL channel)
{
	uint16_t transferCount = 0U;
    switch(channel) 
	{
        case DMA_CHANNEL_0:
				transferCount = DMACNT0;
				break;
        case DMA_CHANNEL_1:
				transferCount = DMACNT1;
				break;
        case DMA_CHANNEL_2:
				transferCount = DMACNT2;
				break;
        case DMA_CHANNEL_3:
				transferCount = DMACNT3;
				break;
        case DMA_CHANNEL_4:
				transferCount = DMACNT4;
				break;
        case DMA_CHANNEL_5:
				transferCount = DMACNT5;
				break;
        default: 
				break;
    }
	return transferCount;
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function sets the software trigger of the DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel - DMA channel number
 * @return     none
 */
inline static void DMA_SoftwareTriggerEnable(enum DMA_CHANNEL channel)
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
				DMACH0bits.CHREQ = 1;
                break;
        case DMA_CHANNEL_1:
				DMACH1bits.CHREQ = 1;
                break;
        case DMA_CHANNEL_2:
				DMACH2bits.CHREQ = 1;
                break;
        case DMA_CHANNEL_3:
				DMACH3bits.CHREQ = 1;
                break;
        case DMA_CHANNEL_4:
				DMACH4bits.CHREQ = 1;
                break;
        case DMA_CHANNEL_5:
				DMACH5bits.CHREQ = 1;
                break;
        default: break;
    }
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function sets the source address to specified DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel - DMA channel number
 * @param[in]  address - Source address
 * @return     none
 */
inline static void DMA_SourceAddressSet(enum DMA_CHANNEL channel, uint16_t address) 
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
				DMASRC0 = address;
                break;
        case DMA_CHANNEL_1:
				DMASRC1 = address;
                break;
        case DMA_CHANNEL_2:
				DMASRC2 = address;
                break;
        case DMA_CHANNEL_3:
				DMASRC3 = address;
                break;
        case DMA_CHANNEL_4:
				DMASRC4 = address;
                break;
        case DMA_CHANNEL_5:
				DMASRC5 = address;
                break;
        default: break;
    }    
}

/**
 * @ingroup    dmadriver
 * @brief      This inline function sets the destination address to specified DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel - DMA channel number
 * @param[in]  address - Destination address
 * @return     none
 */
inline static void DMA_DestinationAddressSet(enum DMA_CHANNEL channel, uint16_t address) 
{
    switch(channel) 
	{
        case DMA_CHANNEL_0:
				DMADST0 = address;
                break;
        case DMA_CHANNEL_1:
				DMADST1 = address;
                break;
        case DMA_CHANNEL_2:
				DMADST2 = address;
                break;
        case DMA_CHANNEL_3:
				DMADST3 = address;
                break;
        case DMA_CHANNEL_4:
				DMADST4 = address;
                break;
        case DMA_CHANNEL_5:
				DMADST5 = address;
                break;
        default: break;
    }    
}


/**
 * @ingroup    dmadriver
 * @brief      This inline function returns the status of the software request of the DMA channel
 * @pre        DMA_Initialize must be called
 * @param[in]  channel  - DMA channel number
 * @return     true		- DMA request is pending
 * @return     false	- DMA request is not pending
 */
inline static bool DMA_IsSoftwareRequestPending(enum DMA_CHANNEL channel)
{
	bool status = 0;
    switch(channel) 
	{
        case DMA_CHANNEL_0:
                status = DMACH0bits.CHREQ;
                break;				
        case DMA_CHANNEL_1:
                status = DMACH1bits.CHREQ;
                break;				
        case DMA_CHANNEL_2:
                status = DMACH2bits.CHREQ;
                break;				
        case DMA_CHANNEL_3:
                status = DMACH3bits.CHREQ;
                break;				
        case DMA_CHANNEL_4:
                status = DMACH4bits.CHREQ;
                break;				
        case DMA_CHANNEL_5:
                status = DMACH5bits.CHREQ;
                break;				
        default: 
				break;
    }
	return status;
}

/**
 * @ingroup  dmadriver
 * @brief    This function is used to implement the tasks for polled implementations of DMA Channel
 * @pre      DMA_Initialize must be called
 * @param    none
 * @return   none
 */
void DMA_ChannelTasks(void);


/**
 * @ingroup    dmadriver
 * @brief      This function can be used to override default callback and to define 
 *             custom callback for DMA Channel event.
 * @param[in]  callback - Address of the callback function
 * @return     none 
 */
void DMA_ChannelCallbackRegister(void(*callback)(enum DMA_CHANNEL channel));

/**
 * @ingroup    dmadriver
 * @brief      This is the default callback with weak attribute. The user can 
 *             override and implement the default callback without weak attribute 
 *             or can register a custom callback function using \ref DMA_ChannelCallbackRegister.
 * @param[in]  channel - Selected channel for the Callback handling
 * @return     none  
 */
void DMA_ChannelCallback(enum DMA_CHANNEL channel);
    
#endif  // DMA_H


