/**
 * Power Controller Typedef File
 * 
 * @file      drv_pwrctrl_app_TPBLPFC.h
 * 
 * @ingroup   p-controller-app-TPBLPFC
 * 
 * @brief     This is the driver header file for the power controller app
*/
/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_app_TPBLPFC.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides Controller settings for average current mode and voltage mode controller.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CK256MP506      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

#include "../sources_common/Totempole_Application_Setup.h"


#ifndef _DRV_PWRCTRL_APP_TPBLPFC_H
#define	_DRV_PWRCTRL_APP_TPBLPFC_H

#include <xc.h>

#define DCMIN (800<<1)
#define DCMAX (19200<<1)

#define FLOAT_TO_FRACT(val)  (val < 0.0) ? (32768 * (val) - 0.5) : (32767 * (val) + 0.5)

#define MIN_GAIN_FACTOR (uint16_t)(FLOAT_TO_FRACT(0.1))
#define MAX_GAIN_FACTOR (uint16_t)(FLOAT_TO_FRACT(0.8))

//==============================================================================
// @brief
//==============================================================================

void Application_handler_PH1(void);
void Application_handler_PH2(void);

#endif	/* XC_HEADER_TEMPLATE_H */

