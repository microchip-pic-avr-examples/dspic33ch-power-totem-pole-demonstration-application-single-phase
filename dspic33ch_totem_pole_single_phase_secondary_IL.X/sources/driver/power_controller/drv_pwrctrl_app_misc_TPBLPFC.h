/**
 * Power Controller Typedef File
 * 
 * @file      drv_pwrctrl_app_misc_TPBLPFC.h
 * 
 * @ingroup   p-controller-misc-app-TPBLPFC
 * 
 * @brief     This is the driver header file for the power controller misc app
*/

/**
  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_app_misc_TPBLPFC.h

  @Summary
    This is the generated driver implementation file using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides all the function for the power controller to control and drive the power stage(s).
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  dsPIC33CH512MP506S1      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v6.00
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

#ifndef _DRV_PWRCTRL_APP_MISC_TPBLPFC_H_
#define	_DRV_PWRCTRL_APP_MISC_TPBLPFC_H_

#include <xc.h> // include processor files - each processor file is guarded.  

#include "../../../sources/devices/dev_TPBLPFC_typedef.h"

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

PWR_CTRL_STATE_e Softstart_reference(void);
void Drv_PwrCtrl_TPBLPFC_Init(void);
void Drv_PwrCtrl_TPBLPFC_Stop(void);

//------------------------------------------------------------------------------

#endif	

