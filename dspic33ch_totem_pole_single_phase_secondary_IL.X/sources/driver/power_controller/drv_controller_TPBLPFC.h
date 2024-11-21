/**
 * Power Controller Typedef File
 * 
 * @file      drv_controller_TPBLPFC.h
 * 
 * @ingroup   p-controller-TPBLPFC
 * 
 * @brief     This is the driver header file for the power controller
*/

/**
  @Company
    Microchip Technology Inc.

  @File Name
    Controller_pwrctrl_TPBLPFC.h

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


#ifndef _CONTROLLER_PWRCTRL_TPBLPFC_H
#define	_CONTROLLER_PWRCTRL_TPBLPFC_H

#include <xc.h> 
#include "smps_control.h"   //interfaces to Switch Mode Power Supply compensator library.
#include "../dcdt/icomp1lv/dcdt_generated_code/icomp1lv_dcdt.h"
#include "../dcdt/icomp1hv/dcdt_generated_code/icomp1hv_dcdt.h"
#include "../dcdt/vcomp1ph/dcdt_generated_code/vcomp1ph_dcdt.h"

//======================================================================================================================
// @brief extern declarations
//======================================================================================================================

extern SMPS_2P2Z_T VMC_2p2z;
extern SMPS_2P2Z_T PHx_AVG_CM2p2z[3];

//======================================================================================================================
// @brief function prototypes
//======================================================================================================================

void CurrentController_PwrCtrl_TPBLPFC_Init(void);
void VoltageController_PwrCtrl_TPBLPFC_Init(void);

void VoutBalanceController_PwrCtrl_TPBLPFC_Init(void);

#endif	/* XC_HEADER_TEMPLATE_H */

