/**
 * SPI Driver Header File
 * 
 * @file      drv_spi.h
 * 
 * @ingroup   pri-layer-SPI
 * 
 * @brief     This is the driver source file for spi builds upon mcc
*/

/*
    (c) 2022 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


#ifndef _DRV_SPI_H
#define	_DRV_SPI_H

#include <xc.h> 
#include <stdbool.h>

// SPI timeout in seconds. If SPI message not received within this time, trigger timeout
#define SPI_TIMEOUT                 (1.0e-3)    

// convert timeout to 100us ticks as the timeout is measured in the 100us scheduler
#define SPI_TIMEOUT_IN_100us_TICKS  ((uint16_t)(SPI_TIMEOUT/100.0e-6))

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================
void Drv_SPI_Reset_Timeout_Counter(uint16_t count);
void Drv_SPI_Timeout(void);
bool Drv_SPI_get_Connected_Flag(void);
bool Drv_SPI_get_ChecksumOK_Flag(void);

bool Drv_SPI_Checksum(void);    
uint16_t* Drv_SPI_get_Adr_Data_Obj(void);  //TODO: remove

//======================================================================================================================
// @brief static declarations
//======================================================================================================================

#endif	/* XC_HEADER_TEMPLATE_H */

