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

#include "drv_controller_TPBLPFC.h"
#include "PFC_frameworkSetup.h"

SMPS_2P2Z_T VMC_2p2z;
SMPS_2P2Z_T PHx_AVG_CM2p2z;

//======================================================================================================================
//@brief SMPS_2P2Z_T AVG_CM2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t   PH1_AVG_CM2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t   PH1_AVG_CM2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PH1_AVG_CM2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PH1_AVG_CM2p2zBCoefficients[3] __attribute__((space(xmemory)));
uint16_t  PH1_AVG_CM2p2zReferenceSet;

//======================================================================================================================
//@brief SMPS_2P2Z_T VMC_2p2z //typedef from smps_control.h to var
//======================================================================================================================

int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
uint16_t  VMC_2p2zReferenceSet;

/*******************************************************************************
 * @fn        current loop compensator setup based on mode    
 * @brief      
 * @param     none
 * @return    none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/ 
void CurrentController_PwrCtrl_TPBLPFC_Init(void)
{
    
  PHx_AVG_CM2p2z.aCoefficients = &PH1_AVG_CM2p2zACoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z.bCoefficients = &PH1_AVG_CM2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z.controlHistory = &PH1_AVG_CM2p2zControlHistory[0]; // Set up pointer to controller history
  PHx_AVG_CM2p2z.errorHistory = &PH1_AVG_CM2p2zErrorHistory[0]; // Set up pointer to error history

#if HIGH_VOLTAGE==0
    PHx_AVG_CM2p2z.preShift = ICOMP1LV_COMP_2P2Z_PRESHIFT;
    PHx_AVG_CM2p2z.postScaler = ICOMP1LV_COMP_2P2Z_POSTSCALER;
    PHx_AVG_CM2p2z.postShift = ICOMP1LV_COMP_2P2Z_POSTSHIFT;
    PHx_AVG_CM2p2z.minOutput = ICOMP1LV_COMP_2P2Z_MIN_CLAMP;
    PHx_AVG_CM2p2z.maxOutput = ICOMP1LV_COMP_2P2Z_MAX_CLAMP; 

  //Set coefficients
  PH1_AVG_CM2p2zACoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_A1;
  PH1_AVG_CM2p2zACoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_A2;
  PH1_AVG_CM2p2zBCoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_B0;
  PH1_AVG_CM2p2zBCoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_B1;
  PH1_AVG_CM2p2zBCoefficients[2] = ICOMP1LV_COMP_2P2Z_COEFF_B2;
#endif 

#if HIGH_VOLTAGE==1

    PHx_AVG_CM2p2z.preShift = ICOMP1HV_COMP_2P2Z_PRESHIFT;
    PHx_AVG_CM2p2z.postScaler = ICOMP1HV_COMP_2P2Z_POSTSCALER;
    PHx_AVG_CM2p2z.postShift = ICOMP1HV_COMP_2P2Z_POSTSHIFT;
    PHx_AVG_CM2p2z.minOutput = ICOMP1HV_COMP_2P2Z_MIN_CLAMP;
    PHx_AVG_CM2p2z.maxOutput = ICOMP1HV_COMP_2P2Z_MAX_CLAMP; 

  //Set coefficients
  PH1_AVG_CM2p2zACoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_A1;
  PH1_AVG_CM2p2zACoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_A2;
  PH1_AVG_CM2p2zBCoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_B0;
  PH1_AVG_CM2p2zBCoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_B1;
  PH1_AVG_CM2p2zBCoefficients[2] = ICOMP1HV_COMP_2P2Z_COEFF_B2;
#endif 
  
  PH1_AVG_CM2p2zReferenceSet = 0;
  
  SMPS_Controller2P2ZInitialize(&PHx_AVG_CM2p2z); // Clear histories
}
/*******************************************************************************
 * @fn        voltage loop compensator setup based on mode    
 * @brief      
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/

void VoltageController_PwrCtrl_TPBLPFC_Init(void)
{
  VMC_2p2z.aCoefficients = &VMC_2p2zACoefficients[0]; // Set up pointer to derived coefficients
  VMC_2p2z.bCoefficients = &VMC_2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  VMC_2p2z.controlHistory = &VMC_2p2zControlHistory[0]; // Set up pointer to controller history
  VMC_2p2z.errorHistory = &VMC_2p2zErrorHistory[0]; // Set up pointer to error history

  VMC_2p2z.preShift = VCOMP1PH_COMP_2P2Z_PRESHIFT;
  VMC_2p2z.postScaler = VCOMP1PH_COMP_2P2Z_POSTSCALER;
  VMC_2p2z.postShift = VCOMP1PH_COMP_2P2Z_POSTSHIFT;
  VMC_2p2z.minOutput = VCOMP1PH_COMP_2P2Z_MIN_CLAMP;
  VMC_2p2z.maxOutput = VCOMP1PH_COMP_2P2Z_MAX_CLAMP;
  //Set coefficients
  VMC_2p2zACoefficients[0] = VCOMP1PH_COMP_2P2Z_COEFF_A1;
  VMC_2p2zACoefficients[1] = VCOMP1PH_COMP_2P2Z_COEFF_A2;
  VMC_2p2zBCoefficients[0] = VCOMP1PH_COMP_2P2Z_COEFF_B0;
  VMC_2p2zBCoefficients[1] = VCOMP1PH_COMP_2P2Z_COEFF_B1;
  VMC_2p2zBCoefficients[2] = VCOMP1PH_COMP_2P2Z_COEFF_B2;

  VMC_2p2zReferenceSet = 0;

  SMPS_Controller2P2ZInitialize(&VMC_2p2z); // Clear histories

}

//==============================================================================
//==============================================================================

