
/**
 * SPI1 Generated Driver Source File
 * 
 * @file        spi1.c
 * 
 * @ingroup     spiclientdriver
 * 
 * @brief       This is the generated driver source file for SPI1 driver.
 *
 * @skipline @version     Firmware Driver Version 1.1.3
 *
 * @skipline @version     PLIB Version 1.2.0
 *
 * @skipline    Device : dsPIC33CH512MP506
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
#include <xc.h>
#include "../spi1.h"

// Section: File specific data type definitions

/** 
 @ingroup spidriver
  @brief Dummy data to be sent for half duplex communication.
*/
#define SPI1_DUMMY_DATA 0x0

//Defines an object for SPI_CLIENT_INTERFACE.

const struct SPI_CLIENT_INTERFACE SPI1_Client = {
    .Initialize         = &SPI1_Initialize,
    .Deinitialize       = &SPI1_Deinitialize,
    .Close              = &SPI1_Close,
    .Open               = &SPI1_Open,
    .ByteExchange       = &SPI1_ByteExchange,
    .ByteRead           = &SPI1_ByteRead,    
    .ByteWrite          = &SPI1_ByteWrite,
    .IsRxReady          = &SPI1_IsRxReady,
    .IsTxReady          = &SPI1_IsTxReady,
};
        

/**
 @ingroup spiclientdriver
 @struct SPI1_CONFIG 
 @brief Defines the SPI1 configuration.
*/
struct SPI1_CLIENT_CONFIG
{ 
    uint16_t controlRegister1; //SPI1CON1L
};

static const struct SPI1_CLIENT_CONFIG config[] = {  
                                        { 
                                            /*Configuration setting for CLIENT_CONFIG.
                                            SPI Mode : Mode 3*/
                                            0xc1,//SPI1CON1L
                                        },
                                    };

// Section: Driver Interface Function Definitions

void SPI1_Initialize (void)
{
  //TODO: replace below
//    // SPIBRGL 0; 
//    SPI1BRGL = 0x0;
//    // AUDEN disabled; FRMEN disabled; AUDMOD I2S; FRMSYPW One clock wide; AUDMONO stereo; FRMCNT 0x0; MSSEN disabled; FRMPOL disabled; IGNROV disabled; SPISGNEXT not sign-extended; FRMSYNC disabled; URDTEN disabled; IGNTUR disabled; 
//    SPI1CON1H = 0x0;
//    // WLENGTH 0; 
//    SPI1CON2L = 0x0;
//    // SPIROV disabled; FRMERR disabled; 
//    SPI1STATL = 0x0;
//    // SPIURDTL 0; 
//    SPI1URDTL = 0x0;
//    // SPIURDTH 0; 
//    SPI1URDTH = 0x0;
//    // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:Low, Active:High; SSEN enabled; MSTEN Client; MODE16 disabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF enabled; DISSDI disabled; 
//    SPI1CON1L = 0x81;   
  //TODO: replace above
  
//TODO: remove below
    SPI1CON1H = 0x00;
  // WLENGTH 0; 
  SPI1CON2L = 0x00;
  // SPIROV disabled; FRMERR disabled; 
  SPI1STATL = 0x00;
  // SPI1BRGL 0; 
  SPI1BRGL = 0x00;
  // SPITBFEN disabled; SPITUREN disabled; FRMERREN disabled; SRMTEN disabled; SPIRBEN disabled; BUSYEN disabled; SPITBEN disabled; SPIROVEN disabled; SPIRBFEN enabled; 
  SPI1IMSKL = 0x01;
  // RXMSK 4; TXWIEN disabled; TXMSK 0; RXWIEN disabled; 
  SPI1IMSKH = 0x400;
  // SPI1URDTL 0; 
  SPI1URDTL = 0x00;
  // SPI1URDTH 0; 
  SPI1URDTH = 0x00;
  // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:High, Active:Low; SSEN disabled; MSTEN Slave; MODE16 enabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF disabled; DISSDI disabled; 
  SPI1CON1L = 0x440;
//TODO: remove above
}

void SPI1_Deinitialize (void)
{
    SPI1_Close();
    
    SPI1BRGL = 0x0;
    SPI1CON1L = 0x0;
    SPI1CON1H = 0x0;
    SPI1CON2L = 0x0;
    SPI1STATL = 0x28;
    SPI1URDTL = 0x0;
    SPI1URDTH = 0x0;
}

void SPI1_Close(void)
{
    SPI1CON1Lbits.SPIEN = 0U;
}

bool SPI1_Open(uint8_t spiConfigIndex)
{
    bool status = false;
    if(!SPI1CON1Lbits.SPIEN)
    {
        SPI1CON1L = config[spiConfigIndex].controlRegister1;
        SPI1CON1Lbits.SPIEN = 1U;
        
        status = true;
    }
    return status;
}

uint8_t SPI1_ByteExchange(uint8_t byteData)
{
    while(1U == SPI1STATLbits.SPITBF)
    {

    }

    SPI1BUFL = byteData;

    while (1U == SPI1STATLbits.SPIRBE)
    {
    
    }

    return SPI1BUFL;
}

void SPI1_ByteWrite(uint8_t byteData)
{
    while(1U == SPI1STATLbits.SPITBF)
    {

    }
    
    SPI1BUFL = byteData;
}

uint8_t SPI1_ByteRead(void)
{
    while (1U == SPI1STATLbits.SPIRBE)
    {
    
    }
    
    return SPI1BUFL;
}

bool SPI1_IsRxReady(void)
{    
    return (!SPI1STATLbits.SPIRBE);
}

bool SPI1_IsTxReady(void)
{    
    return (!SPI1STATLbits.SPITBF);
}

