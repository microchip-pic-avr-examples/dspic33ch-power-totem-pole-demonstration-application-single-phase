/**
 * DMA Generated Driver Source File
 * 
 * @file      dma.c
 * 
 * @ingroup   dmadriver
 * 
 * @brief     This is the generated driver source file for DMA driver
 *
 * @skipline @version   Firmware Driver Version 1.1.4
 *
 * @skipline @version   PLIB Version 1.4.0
 *
 * @skipline  Device : dsPIC33CH512MP506
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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

#include <stddef.h> 
#include "../dma.h"

#include "driver/dma/drv_dma.h" //TODO: remove

// Section: File specific functions

static void (*DMA_ChannelHandler)(enum DMA_CHANNEL channel) = NULL;

// Section: Driver Interface

const struct DMA_INTERFACE DMA_SPI = {
    .Initialize                 = &DMA_Initialize,
    .Deinitialize               = &DMA_Deinitialize,
    .ChannelEnable              = &DMA_ChannelEnable,
    .ChannelDisable             = &DMA_ChannelDisable,
    .TransferCountSet           = &DMA_TransferCountSet,
    .TransferCountGet           = &DMA_TransferCountGet,
    .SoftwareTriggerEnable      = &DMA_SoftwareTriggerEnable,
    .SourceAddressSet           = &DMA_SourceAddressSet,
    .DestinationAddressSet      = &DMA_DestinationAddressSet,
    .IsSoftwareRequestPending   = &DMA_IsSoftwareRequestPending,
    .ChannelCallbackRegister = &DMA_ChannelCallbackRegister,
    .ChannelTasks = &DMA_ChannelTasks,
};
// Section: Driver Interface Function Definitions

void DMA_Initialize (void) 
{ 
  
    // DMAEN disabled; PRSSEL Fixed priority; 
    DMACON = (0x0 | 0x01) & 0x7FFF; //The module is disabled, till other settings are configured.
    // LADDR 6144; 
    DMAL= 0x1800;
    // HADDR 20479; 
    DMAH= 0x4FFF;

    // CHEN0 disabled; DAMODE0 Incremented; TRMODE0 One-Shot; CHREQ0 disabled; RELOAD0 enabled; SIZE0 16 bit; NULLW0 disabled; SAMODE0 Unchanged; 
    DMACH0 = (uint16_t)0x210 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; CHSEL0 Main SPI1 RX; HIGHIF disabled; 
    DMAINT0= 0x200;
    // SADDR0 696; 
    DMASRC0= 0x2B8;
    // DADDR0 0; 
    DMADST0= 0x0;
    // CNT0 4; 
    DMACNT0= 0x4;
    // Clearing Channel 0 Interrupt Flag;
    IFS0bits.DMA0IF = 0;
    // Enabling Channel 0 Interrupt
    IEC0bits.DMA0IE = 1;

    // CHEN1 disabled; SAMODE1 Unchanged; SIZE1 16 bit; DAMODE1 Unchanged; CHREQ1 disabled; RELOAD1 disabled; TRMODE1 One-Shot; NULLW1 disabled; 
    DMACH1 = (uint16_t)0x0 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; HIGHIF disabled; CHSEL1 Main INT0; 
    DMAINT1= 0x0;
    // SADDR1 0; 
    DMASRC1= 0x0;
    // DADDR1 0; 
    DMADST1= 0x0;
    // CNT1 0; 
    DMACNT1= 0x0;
    // Clearing Channel 1 Interrupt Flag;
    IFS0bits.DMA1IF = 0;

    // SAMODE2 Unchanged; CHEN2 disabled; SIZE2 16 bit; DAMODE2 Unchanged; CHREQ2 disabled; RELOAD2 disabled; NULLW2 disabled; TRMODE2 One-Shot; 
    DMACH2 = (uint16_t)0x0 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; HIGHIF disabled; CHSEL2 Main INT0; 
    DMAINT2= 0x0;
    // SADDR2 0; 
    DMASRC2= 0x0;
    // DADDR2 0; 
    DMADST2= 0x0;
    // CNT2 0; 
    DMACNT2= 0x0;
    // Clearing Channel 2 Interrupt Flag;
    IFS1bits.DMA2IF = 0;

    // SAMODE3 Unchanged; CHEN3 disabled; SIZE3 16 bit; DAMODE3 Unchanged; TRMODE3 One-Shot; NULLW3 disabled; CHREQ3 disabled; RELOAD3 disabled; 
    DMACH3 = (uint16_t)0x0 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; CHSEL3 Main INT0; HIGHIF disabled; 
    DMAINT3= 0x0;
    // SADDR3 0; 
    DMASRC3= 0x0;
    // DADDR3 0; 
    DMADST3= 0x0;
    // CNT3 0; 
    DMACNT3= 0x0;
    // Clearing Channel 3 Interrupt Flag;
    IFS1bits.DMA3IF = 0;

    // CHEN4 disabled; DAMODE4 Unchanged; TRMODE4 One-Shot; CHREQ4 disabled; RELOAD4 disabled; SIZE4 16 bit; NULLW4 disabled; SAMODE4 Unchanged; 
    DMACH4 = (uint16_t)0x0 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; HIGHIF disabled; CHSEL4 Main INT0; 
    DMAINT4= 0x0;
    // SADDR4 0; 
    DMASRC4= 0x0;
    // DADDR4 0; 
    DMADST4= 0x0;
    // CNT4 0; 
    DMACNT4= 0x0;
    // Clearing Channel 4 Interrupt Flag;
    IFS1bits.DMA4IF = 0;

    // SIZE5 16 bit; DAMODE5 Unchanged; CHEN5 disabled; SAMODE5 Unchanged; CHREQ5 disabled; RELOAD5 disabled; TRMODE5 One-Shot; NULLW5 disabled; 
    DMACH5 = (uint16_t)0x0 & (uint16_t)0xFFFE; //The channel is disabled, till other settings are configured.
    // CHSEL5 Main INT0; HALFIF disabled; LOWIF disabled; HALFEN disabled; DONEIF disabled; OVRUNIF disabled; HIGHIF disabled; 
    DMAINT5= 0x0;
    // SADDR5 0; 
    DMASRC5= 0x0;
    // DADDR5 0; 
    DMADST5= 0x0;
    // CNT5 0; 
    DMACNT5= 0x0;
    // Clearing Channel 5 Interrupt Flag;
    IFS2bits.DMA5IF = 0;
	
    DMA_ChannelCallbackRegister(&DMA_ChannelCallback);
    
    //Enable DMA
    DMACONbits.DMAEN = 1;  
}

void DMA_Deinitialize (void) 
{   
    DMACONbits.DMAEN = 0; 
    
    DMACH0bits.CHEN = 0;
    DMACH1bits.CHEN = 0;
    DMACH2bits.CHEN = 0;
    DMACH3bits.CHEN = 0;
    DMACH4bits.CHEN = 0;
    DMACH5bits.CHEN = 0;
    
    DMACON= 0x0;
    DMAL= 0x0;
    DMAH= 0x0;
    
    DMACH0 = 0x0; 
    DMAINT0 = 0x0; 
    DMASRC0 = 0x0; 
    DMADST0 = 0x0; 
    DMACNT0 = 0x1; 
    IFS0bits.DMA0IF = 0;
    IEC0bits.DMA0IE = 0;
    DMACH1 = 0x0; 
    DMAINT1 = 0x0; 
    DMASRC1 = 0x0; 
    DMADST1 = 0x0; 
    DMACNT1 = 0x1; 
    IFS0bits.DMA1IF = 0;
    DMACH2 = 0x0; 
    DMAINT2 = 0x0; 
    DMASRC2 = 0x0; 
    DMADST2 = 0x0; 
    DMACNT2 = 0x1; 
    IFS1bits.DMA2IF = 0;
    DMACH3 = 0x0; 
    DMAINT3 = 0x0; 
    DMASRC3 = 0x0; 
    DMADST3 = 0x0; 
    DMACNT3 = 0x1; 
    IFS1bits.DMA3IF = 0;
    DMACH4 = 0x0; 
    DMAINT4 = 0x0; 
    DMASRC4 = 0x0; 
    DMADST4 = 0x0; 
    DMACNT4 = 0x1; 
    IFS1bits.DMA4IF = 0;
    DMACH5 = 0x0; 
    DMAINT5 = 0x0; 
    DMASRC5 = 0x0; 
    DMADST5 = 0x0; 
    DMACNT5 = 0x1; 
    IFS2bits.DMA5IF = 0;
    
}

void DMA_ChannelCallbackRegister(void(*callback)(enum DMA_CHANNEL channel))
{
    if(NULL != callback)
    {
        DMA_ChannelHandler = callback;
    }
}

void __attribute__ ((weak)) DMA_ChannelCallback (enum DMA_CHANNEL channel)
{ 
    (void)channel;
} 

void __attribute__ ( ( interrupt, no_auto_psv ) ) _DMA0Interrupt( void )
{  
    if(NULL != DMA_ChannelHandler)
    {
        (*DMA_ChannelHandler)(DMA_CHANNEL_0);
    }

    IFS0bits.DMA0IF = 0;
}

void DMA_ChannelTasks( void )
{   
	if(IFS0bits.DMA1IF == 1)
	{
        if(NULL != DMA_ChannelHandler)
        {
		    (*DMA_ChannelHandler)(DMA_CHANNEL_1);
        }
		
		IFS0bits.DMA1IF = 0;
	}
	if(IFS1bits.DMA2IF == 1)
	{
        if(NULL != DMA_ChannelHandler)
        {
		    (*DMA_ChannelHandler)(DMA_CHANNEL_2);
        }
		
		IFS1bits.DMA2IF = 0;
	}
	if(IFS1bits.DMA3IF == 1)
	{
        if(NULL != DMA_ChannelHandler)
        {
		    (*DMA_ChannelHandler)(DMA_CHANNEL_3);
        }
		
		IFS1bits.DMA3IF = 0;
	}
	if(IFS1bits.DMA4IF == 1)
	{
        if(NULL != DMA_ChannelHandler)
        {
		    (*DMA_ChannelHandler)(DMA_CHANNEL_4);
        }
		
		IFS1bits.DMA4IF = 0;
	}
	if(IFS2bits.DMA5IF == 1)
	{
        if(NULL != DMA_ChannelHandler)
        {
		    (*DMA_ChannelHandler)(DMA_CHANNEL_5);
        }
		
		IFS2bits.DMA5IF = 0;
	}
}