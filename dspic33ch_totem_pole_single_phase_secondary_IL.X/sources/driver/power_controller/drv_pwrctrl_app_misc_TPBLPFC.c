/**
 * Power Controller Typedef File
 * 
 * @file      drv_pwrctrl_app_misc_TPBLPFC.c
 * 
 * @ingroup   p-controller-misc-app-TPBLPFC
 * 
 * @brief     This is the driver source file for the power controller misc app
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

#include <xc.h>
#include <stdint.h>

#include "../../../mcc_generated_files/system/pins.h"

#include "drv_controller_TPBLPFC.h"
#include "../../devices/dev_TPBLPFC_typedef.h"
#include "PFC_frameworkSetup.h"
#include "../../misc/MCC_add_on.h"

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_misc_TPBLPFC
 * @fn      Drv_PwrCtrl_TPBLPFC_Init
 * @param   void
 * @return  void
 * @brief   initialization
 * @details 
 *          initialization of voltage and current controller, PWM's 
 *          and synchronous rectifier port pins *             
 **********************************************************************************/

void Drv_PwrCtrl_TPBLPFC_Init(void)
{
  OpenLoopStartupDuration = OPEN_LOOP_STARTUP_PULSES;
  
  //Compensation out of DCDT: icomp1lv/hv_dcdt.h, vcomp1ph_dcdt.h
  CurrentController_PwrCtrl_TPBLPFC_Init();
  VoltageController_PwrCtrl_TPBLPFC_Init();
  
  //##############################################
  PG1TRIGC = PG1PER >> 1; //180� Phase Shift
//  PG1IOCONLbits.OSYNC = 0b01;
//  PG2IOCONLbits.OSYNC = 0b01;
  //##############################################

  PWM_DeadTimeHighSet(PH1_PWM, DEAD_TIME);
  PWM_DeadTimeLowSet(PH1_PWM, DEAD_TIME);
  PWM_GeneratorEnable(PH1_PWM);
  
  PWM_DeadTimeHighSet(PH2_PWM, DEAD_TIME);
  PWM_DeadTimeLowSet(PH2_PWM, DEAD_TIME);
  PWM_GeneratorEnable(PH2_PWM);
  
  PWM_H_N_SetLow();
  PWM_L_N_SetLow();

  Vout_Control.Reference.Reference_Internal = 0;
  Adaptive_Gain_Factor = (3276 << 1);  //factor 0.2    3276 is factor = 0.1
}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_misc_TPBLPFC
 * @fn      Drv_PwrCtrl_TPBLPFC_Stop
 * @param   void
 * @return  void
 * @brief   stop and reset power controller
 * @details 
 *          disable PWM's rectifier switches
 *          reset control flags
 **********************************************************************************/

void Drv_PwrCtrl_TPBLPFC_Stop(void)
{  
  //<<< only testing >>>
  PWM_OverrideHighEnable(PH1_PWM); //disable PWM output on Buck-H
  PWM_OverrideLowEnable(PH1_PWM); //disable PWM output on Buck-L  
  PWM_OverrideHighEnable(PH2_PWM); //disable PWM output on Buck-H
  PWM_OverrideLowEnable(PH2_PWM); //disable PWM output on Buck-L  

  PWM_H_N_SetLow();
  PWM_L_N_SetLow();
  Vout_Control.Reference.Reference_Internal = 0; //new 06.09.2021
  Vout_Control.Vout.Filtered = 0;

  //Burst mode flags must be reset on switch off to ensure proper switch on
  //afterwards when PFC was in Burst mode before switch off
  Phase_Values_PH1.Control_Status_Flags.bits.BurstModeLatched = 0;
  Phase_Values_PH1.Controller_Values.Control_Freeze = 0;
  Phase_Values_PH1.Controller_Values.BurstModeCycleCounter = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.BurstModeLatched = 0;
  Phase_Values_PH2.Controller_Values.Control_Freeze = 0;
  Phase_Values_PH2.Controller_Values.BurstModeCycleCounter = 0;
}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_misc_TPBLPFC
 * @fn      Softstart_reference
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   softstart ramp
 * @details 
 *          ramp up/down after startup and also when reference is changed.
 *          reference change ramps to new value either up or down.
 *          reference_set is the setpoint.
 *          reference_internal is the actual value.
 *          at the end of the ramp reference_internal = reference_set
 **********************************************************************************/

// ramp speed was 1 all the time
#define SOFT_START_RAMP_SPEED 4 //LV step is 26.9mV, HV step is 25.2mV
#define STEP_DELAY 30           //30*100us=3ms delay between one step of SOFT_START_RAMP_SPEED          

PWR_CTRL_STATE_e Softstart_reference(void)
{
  static uint16_t SoftstartCounter = 0;
  static PWR_CTRL_STATE_e returnvalue = 1;

#ifdef VOLTAGE_LOOP
  if (++SoftstartCounter > STEP_DELAY)
#else
  if (++SoftstartCounter > 20) //fixed value
#endif    
  {
    if (Vout_Control.Reference.Reference_Internal <= Vout_Control.Reference.Reference_Set)
    {
      if (((Vout_Control.Reference.Reference_Internal + SOFT_START_RAMP_SPEED) < Vout_Control.Reference.Reference_Set)
        && ((Vout_Control.Reference.Reference_Internal + SOFT_START_RAMP_SPEED) >= SOFT_START_RAMP_SPEED))
      {
        Vout_Control.Reference.Reference_Internal += SOFT_START_RAMP_SPEED;
        returnvalue = pwr_ctrl_state;
      }
      else
      {
        returnvalue = PCS_UP_AND_RUNNING;
        Vout_Control.Reference.VOUT_dV_dt_Hystresis_High = Vout_Control.Reference.Reference_Internal + (Vout_Control.Reference.Reference_Internal >> 3);
        Vout_Control.Reference.VOUT_dV_dt_Hystresis_Low = Vout_Control.Reference.Reference_Internal - (Vout_Control.Reference.Reference_Internal >> 4);
        Vout_Control.Reference.VOUT_Hystresis_High = Vout_Control.Reference.Reference_Internal + (Vout_Control.Reference.Reference_Internal >> 5);
        Vout_Control.Reference.VOUT_Hystresis_Low = Vout_Control.Reference.Reference_Internal - (Vout_Control.Reference.Reference_Internal >> 5);
        Phase_Values_PH1.Control_Status_Flags.bits.Reference_Changed = 0;
      }
    }
    else
    {
      if (((Vout_Control.Reference.Reference_Internal + SOFT_START_RAMP_SPEED) > Vout_Control.Reference.Reference_Set)
        && ((Vout_Control.Reference.Reference_Internal + SOFT_START_RAMP_SPEED) >= SOFT_START_RAMP_SPEED))
      {
        Vout_Control.Reference.Reference_Internal -= SOFT_START_RAMP_SPEED;
        returnvalue = pwr_ctrl_state;
      }
      else
      {
        returnvalue = PCS_UP_AND_RUNNING;
        Vout_Control.Reference.VOUT_dV_dt_Hystresis_High = Vout_Control.Reference.Reference_Internal + (Vout_Control.Reference.Reference_Internal >> 3);
        Vout_Control.Reference.VOUT_dV_dt_Hystresis_Low = Vout_Control.Reference.Reference_Internal - (Vout_Control.Reference.Reference_Internal >> 4);
        Vout_Control.Reference.VOUT_Hystresis_High = Vout_Control.Reference.Reference_Internal + (Vout_Control.Reference.Reference_Internal >> 5);
        Vout_Control.Reference.VOUT_Hystresis_Low = Vout_Control.Reference.Reference_Internal - (Vout_Control.Reference.Reference_Internal >> 5);
        Phase_Values_PH1.Control_Status_Flags.bits.Reference_Changed = 0;
      }
    }

    SoftstartCounter = 0;
  }
  else
    returnvalue = pwr_ctrl_state;

  return returnvalue;
}

//==============================================================================
//==============================================================================
