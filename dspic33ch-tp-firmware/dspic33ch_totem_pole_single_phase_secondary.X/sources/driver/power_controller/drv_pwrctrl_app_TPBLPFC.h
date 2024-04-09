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


#ifndef _DRV_PWRCTRL_APP_TPBLPFC_H
#define	_DRV_PWRCTRL_APP_TPBLPFC_H

#include <xc.h>

#define DCMIN 800//<<<FTX 800
#define DCMAX 19200

//Current clamp option
#define CLAMPCURRENT 21.2 //15A * SQRT2
#define CURRENTSENSOR 0.025 //V/A
//#define I_INCLAMP ((CLAMPCURRENT*CURRENTSENSOR)/3.3*4096)*16)
//#define I_INCLAMP 11936   // [(17A*SQRT2*CURRENTSENSOR)/3.3*4096] <<4
#define I_INCLAMP 10500   // [(1A*SQRT2*CURRENTSENSOR)/3.3*4096] <<4

#define TestModePh1
#define TestModePh2
#define TestModePh3

//==============================================================================
// @brief
//==============================================================================

void PWM_handler_PH123(void);

#endif	/* XC_HEADER_TEMPLATE_H */

