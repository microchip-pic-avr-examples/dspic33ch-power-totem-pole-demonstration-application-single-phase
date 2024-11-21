/**
 * ADC Driver Header File
 * 
 * @file      drv_adc.c
 * 
 * @ingroup   sec-layer-ADC
 * 
 * @brief     This is the driver source file for adc builds upon mcc
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

/**
  Section: Included Files
 */
#include <p33CH512MP506S1.h>
#include <stdlib.h>

#include "drv_adc.h"
#include "../../../mcc_generated_files/system/pins.h"
#include "../../../mcc_generated_files/main_core/main_core.h"
#include "../power_controller/drv_pwrctrl_app_misc_TPBLPFC.h"
#include "../../devices/dev_TPBLPFC_numberconnection_typedef.h"
#include "../../devices/dev_TPBLPFC_typedef.h"
#include "../power_controller/drv_pwrctrl_app_TPBLPFC.h"
#include "../../../sources/driver/power_controller/drv_controller_TPBLPFC.h"
#include "../sources_common/PFC_frameworkSetup.h"

/***********************************************************************************
 * @ingroup drv_adc
 * @fn      _ADCAN0Interrupt
 * @param   void
 * @return  void
 * @brief   ISR triggered from PWM (100kHz), duty cycle half
 * @details 
 *          in PWM center aligned mode we have only one trigger for multi phases
 *          trigger is always in the middle of duty cycle
 *          dedicated core 0 -> I1
 *          dedicated core 1 -> I2
 *          shared core CH15 -> AC_N
 *          shared core CH10 -> Vout  
 **********************************************************************************/

void __attribute__((__interrupt__, auto_psv)) _ADCAN0Interrupt(void)
{
  TP4_DPPIM_RD13_SetHigh();

  ACmonitorTrigger_SetHigh();

  //prestart next shared core input which is AC_N/CH15
  //shared core Vout/CH10 input is ready because of latency from trigger to here
  //from min. 500ns 
  ADCON3Lbits.CNVCHSEL = 15; // change ADC channel
  ADCON3Lbits.CNVRTCH = 1; // trigger conversion

  //I1
  Phase_Values_PH1.Phase_Current.Raw = (int16_t) ADCBUF0;
  Phase_Values_PH1.Phase_Current.Rectified = abs((int16_t) (Phase_Values_PH1.Phase_Current.Raw - Phase_Values_PH1.Phase_Current.Offset)); //phase voltage - offset=VCC/2

  ACmonitorTrigger_SetLow();

  if ((Phase_Values_PH1.Phase_Current.Rectified > IIN_OC) && (pwr_ctrl_state > PCS_RELAYON)) //new 06.09.2021
  {
    PFC_Flags.bits.OC_PH1 = 1;
    PFC_Flags.bits.Run = 0;
    PFC_Flags.bits.Stop = 1;

    Drv_PwrCtrl_TPBLPFC_Stop();
  }
  
//  TP4_DPPIM_RD13_SetLow();
  
  //Vout triggered by PWM
  Vout_Control.Vout.Raw = ADCBUF10;

  if ((Vout_Control.Vout.Raw > VOUT_OV))
  {
    PFC_Flags.bits.OV_Vout = 1;
    PFC_Flags.bits.Run = 0;
    PFC_Flags.bits.Stop = 1;

    Drv_PwrCtrl_TPBLPFC_Stop();
  }
  
//  TP4_DPPIM_RD13_SetHigh();
  
  //shared core Vout input is ready because of latency from trigger to here
  //from min. 350ns (measured)), AD is 270ns
  AC_N.Phase_Voltage.Raw = ADCBUF15; //AC_N

  //<get AC data
  if (MAIN_CORE_ProtocolRead(MSI1_ProtocolB, &(primary_vac_values.Data[0])))
  {
    dev_connect_numbers();
    primary_vac_values.DataReceivedOK = 1;
  }

  ADCON3Lbits.CNVCHSEL = 10; // change ADC channel, triggered by TRIGA

  //<< PFC handler
  Application_handler_PH1();

  IFS5bits.ADCAN0IF = 0; //clear the channel_ANA0 interrupt flag

  TP4_DPPIM_RD13_SetLow();

}

/***********************************************************************************
 * @ingroup drv_adc
 * @fn      _ADCAN1Interrupt
 * @param   void
 * @return  void
 * @brief   ISR triggered from PWM (100kHz), duty cycle half
 * @details 
 *          in PWM center aligned mode we have only one trigger for multi phases
 *          trigger is always in the middle of duty cycle
 *          dedicated core 1 -> I2
 **********************************************************************************/

void __attribute__((__interrupt__, auto_psv)) _ADCAN1Interrupt(void)
{
  TP4_DPPIM_RD13_SetHigh();
    
  //I2
  Phase_Values_PH2.Phase_Current.Raw = ADCBUF1;
  Phase_Values_PH2.Phase_Current.Rectified = abs((int16_t) (Phase_Values_PH2.Phase_Current.Raw - Phase_Values_PH2.Phase_Current.Offset));

  if ((Phase_Values_PH2.Phase_Current.Rectified > IIN_OC) && (pwr_ctrl_state > PCS_RELAYON)) 
  {
    PFC_Flags.bits.OC_PH2 = 1;
    PFC_Flags.bits.Run = 0;
    PFC_Flags.bits.Stop = 1;

    Drv_PwrCtrl_TPBLPFC_Stop();
  }
  
  //<< PFC handler
  Application_handler_PH2();
  
  IFS5bits.ADCAN1IF = 0; //clear the channel_ANA0 interrupt flag
  
  TP4_DPPIM_RD13_SetLow();
  
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

