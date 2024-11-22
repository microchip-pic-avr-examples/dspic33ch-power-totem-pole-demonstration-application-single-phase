/**
 * Power Controller Typedef File
 * 
 * @file      drv_controller_TPBLPFC.c
 * 
 * @ingroup   p-controller-TPBLPFC
 * 
 * @brief     This is the driver source file for the power controller
*/
/*
� [2024] Microchip Technology Inc. and its subsidiaries.

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

#include "drv_controller_TPBLPFC.h"
#include "PFC_frameworkSetup.h"

SMPS_2P2Z_T VMC_2p2z;
SMPS_2P2Z_T PHx_AVG_CM2p2z[3];

/***********************************************************************************
 * @ingroup 
 * @fn      
 * @param   
 * @return  
 * @brief   connect current loop vars to dsp memory 
 * @details 
 *          dsp functionality needs vars to place in x nd y memory
 *          see datasheet for details
 *          typedef from smps_control.h to var
 **********************************************************************************/

int16_t   PH1_AVG_CM2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t   PH1_AVG_CM2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PH1_AVG_CM2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PH1_AVG_CM2p2zBCoefficients[3] __attribute__((space(xmemory)));
uint16_t  PH1_AVG_CM2p2zReferenceSet;

int16_t   PH2_AVG_CM2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t   PH2_AVG_CM2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   PH2_AVG_CM2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   PH2_AVG_CM2p2zBCoefficients[3] __attribute__((space(xmemory)));
uint16_t  PH2_AVG_CM2p2zReferenceSet;

/***********************************************************************************
 * @ingroup 
 * @fn      
 * @param   
 * @return  
 * @brief   connect voltage loop vars to dsp memory 
 * @details 
 *          dsp functionality needs vars to place in x nd y memory
 *          see datasheet for details
 *          typedef from smps_control.h to var
 **********************************************************************************/

int16_t   VMC_2p2zErrorHistory[3] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zACoefficients[2]__attribute__((space(xmemory)));
int16_t   VMC_2p2zControlHistory[2] __attribute__((space(ymemory), far));
int16_t   VMC_2p2zBCoefficients[3] __attribute__((space(xmemory)));
uint16_t  VMC_2p2zReferenceSet;

/***********************************************************************************
 * @ingroup drv_controller_TPBLPFC
 * @fn      CurrentController_PwrCtrl_TPBLPFC_Init
 * @param   void
 * @return  void
 * @brief   initialize current controller coefficient array(s) 
 * @details 
 *          load coefficient numbers into array
 *          initialize 2p2z current controller
 **********************************************************************************/

void CurrentController_PwrCtrl_TPBLPFC_Init(void)
{
   uint16_t i_;
    
  PHx_AVG_CM2p2z[1].aCoefficients = &PH1_AVG_CM2p2zACoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z[1].bCoefficients = &PH1_AVG_CM2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z[1].controlHistory = &PH1_AVG_CM2p2zControlHistory[0]; // Set up pointer to controller history
  PHx_AVG_CM2p2z[1].errorHistory = &PH1_AVG_CM2p2zErrorHistory[0]; // Set up pointer to error history
  
  PHx_AVG_CM2p2z[2].aCoefficients = &PH2_AVG_CM2p2zACoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z[2].bCoefficients = &PH2_AVG_CM2p2zBCoefficients[0]; // Set up pointer to derived coefficients
  PHx_AVG_CM2p2z[2].controlHistory = &PH2_AVG_CM2p2zControlHistory[0]; // Set up pointer to controller history
  PHx_AVG_CM2p2z[2].errorHistory = &PH2_AVG_CM2p2zErrorHistory[0]; // Set up pointer to error history

#if HIGH_VOLTAGE==0
  for(i_=0; i_<3; i_++)
  {
    PHx_AVG_CM2p2z[i_].preShift = ICOMP1LV_COMP_2P2Z_PRESHIFT;
    PHx_AVG_CM2p2z[i_].postScaler = ICOMP1LV_COMP_2P2Z_POSTSCALER;
    PHx_AVG_CM2p2z[i_].postShift = ICOMP1LV_COMP_2P2Z_POSTSHIFT;
    PHx_AVG_CM2p2z[i_].minOutput = ICOMP1LV_COMP_2P2Z_MIN_CLAMP;
    PHx_AVG_CM2p2z[i_].maxOutput = ICOMP1LV_COMP_2P2Z_MAX_CLAMP;
  }
  //Set coefficients
  PH1_AVG_CM2p2zACoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_A1;
  PH1_AVG_CM2p2zACoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_A2;
  PH1_AVG_CM2p2zBCoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_B0;
  PH1_AVG_CM2p2zBCoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_B1;
  PH1_AVG_CM2p2zBCoefficients[2] = ICOMP1LV_COMP_2P2Z_COEFF_B2;
  //Set coefficients
  PH2_AVG_CM2p2zACoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_A1;
  PH2_AVG_CM2p2zACoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_A2;
  PH2_AVG_CM2p2zBCoefficients[0] = ICOMP1LV_COMP_2P2Z_COEFF_B0;
  PH2_AVG_CM2p2zBCoefficients[1] = ICOMP1LV_COMP_2P2Z_COEFF_B1;
  PH2_AVG_CM2p2zBCoefficients[2] = ICOMP1LV_COMP_2P2Z_COEFF_B2;
#endif 

#if HIGH_VOLTAGE==1
  for(i_=0; i_<3; i_++)
  {
    PHx_AVG_CM2p2z[i_].preShift = ICOMP1HV_COMP_2P2Z_PRESHIFT;
    PHx_AVG_CM2p2z[i_].postScaler = ICOMP1HV_COMP_2P2Z_POSTSCALER;
    PHx_AVG_CM2p2z[i_].postShift = ICOMP1HV_COMP_2P2Z_POSTSHIFT;
    PHx_AVG_CM2p2z[i_].minOutput = ICOMP1HV_COMP_2P2Z_MIN_CLAMP;
    PHx_AVG_CM2p2z[i_].maxOutput = ICOMP1HV_COMP_2P2Z_MAX_CLAMP; 
  }
  //Set coefficients
  PH1_AVG_CM2p2zACoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_A1;
  PH1_AVG_CM2p2zACoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_A2;
  PH1_AVG_CM2p2zBCoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_B0;
  PH1_AVG_CM2p2zBCoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_B1;
  PH1_AVG_CM2p2zBCoefficients[2] = ICOMP1HV_COMP_2P2Z_COEFF_B2;
  //Set coefficients
  PH2_AVG_CM2p2zACoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_A1;
  PH2_AVG_CM2p2zACoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_A2;
  PH2_AVG_CM2p2zBCoefficients[0] = ICOMP1HV_COMP_2P2Z_COEFF_B0;
  PH2_AVG_CM2p2zBCoefficients[1] = ICOMP1HV_COMP_2P2Z_COEFF_B1;
  PH2_AVG_CM2p2zBCoefficients[2] = ICOMP1HV_COMP_2P2Z_COEFF_B2;
#endif 
  
  PH1_AVG_CM2p2zReferenceSet = 0;
  PH2_AVG_CM2p2zReferenceSet = 0;
  
  SMPS_Controller2P2ZInitialize(&PHx_AVG_CM2p2z[1]); // Clear histories
  SMPS_Controller2P2ZInitialize(&PHx_AVG_CM2p2z[2]); // Clear histories
}

/***********************************************************************************
 * @ingroup drv_controller_TPBLPFC
 * @fn      VoltageController_PwrCtrl_TPBLPFC_Init
 * @param   void
 * @return  void
 * @brief   initialize voltage controller coefficient array(s) 
 * @details 
 *          load coefficient numbers into array
 *          initialize 2p2z voltage controller
 **********************************************************************************/

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

