/**
 * Power Controller Typedef File
 * 
 * @file      dev_TPBLPFC_typedef.c
 * 
 * @ingroup   p-controller-type-TPBLPFC
 * 
 * @brief     This is the typedef file for power controller
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
#include "dev_TPBLPFC_typedef.h"
#include "../../sources_common/PFC_frameworkSetup.h"

//------------------------------------------------------------------------------
uint16_t OpenLoopStartupDuration;
uint16_t Adaptive_Gain_Factor;
PHASE_VALUES_t Phase_Values_PH1, Phase_Values_PH2, AC_N;
VMC_VALUES_t Vout_Control;
PFC_FLAGS_t PFC_Flags;
PWR_CTRL_STATE_e pwr_ctrl_state = 1; //1 = PCS_INIT
Primary_Data_Handling primary_vac_values;

//------------------------------------------------------------------------------
//<< 
//------------------------------------------------------------------------------

void Init_pwr_ctrl (void)
{
  //Vout -----------------------------------------------------------------------
  Vout_Control.Vout.FilterCounter     = 0;
  Vout_Control.Vout.Filtered          = 0;
  Vout_Control.Vout.Raw               = 0;
  Vout_Control.Vout.Rectified         = 0;
  Vout_Control.Vout.Rectified_Shift   = 0;
  Vout_Control.Vout.Vin_div_Averaged2 = 0;
  Vout_Control.Vout.PreviousValue     = 0;
  Vout_Control.Reference.Voltage_Loop_Output = 0;
  
  //Control reference ----------------------------------------------------------
  Vout_Control.Reference.Reference_Set      = VOUT_REF; //preset reference to configured value
  Vout_Control.Reference.Reference_Internal = 0;
  
  //PH1 voltages ---------------------------------------------------------------
  Phase_Values_PH1.Phase_Voltage.FilterCounter      = 0;
  Phase_Values_PH1.Phase_Voltage.Filtered           = 0;
  Phase_Values_PH1.Phase_Voltage.Raw                = 0;
  Phase_Values_PH1.Phase_Voltage.Rectified          = 0;
  Phase_Values_PH1.Phase_Voltage.Rectified_Shift    = 0;
  Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2  = 0;
  Phase_Values_PH1.Phase_Voltage.Max_Peak  = 0;
  
  //PH1 currents ---------------------------------------------------------------
  Phase_Values_PH1.Phase_Current.FilterCounter      = 0;
  Phase_Values_PH1.Phase_Current.Filtered           = 0;
  Phase_Values_PH1.Phase_Current.Raw                = 0;
  Phase_Values_PH1.Phase_Current.Rectified          = 0;
  Phase_Values_PH1.Phase_Current.Rectified_Shift    = 0;
  Phase_Values_PH1.Phase_Current.Vin_div_Averaged2  = 0;
  
  //PH1 flags ------------------------------------------------------------------
  Phase_Values_PH1.Control_Status_Flags.bits.Soft_Start_Zero_Cross     = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_last         = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.Relay                     = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.Stop                      = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.Fault                     = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed      = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.CCM                       = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.BurstModeDetected         = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.BurstModeLatched          = 0;
  Phase_Values_PH1.Control_Status_Flags.bits.Reference_Changed         = 0;
  
  //PH1 controller numbers -----------------------------------------------------
  Phase_Values_PH1.Controller_Values.PWM_Counter                    = 0;
  Phase_Values_PH1.Controller_Values.Duty_Cycle_Set_Value           = 0;
  Phase_Values_PH1.Controller_Values.CCM_Counter                    = 0;
  Phase_Values_PH1.Controller_Values.DFF_Value                      = 0;
  Phase_Values_PH1.Controller_Values.IAC_Reference_l                = 0;
  Phase_Values_PH1.Controller_Values.IAC_Reference                  = 0;
  Phase_Values_PH1.Controller_Values.BurstModeCycleCounter          = 0; 
  Phase_Values_PH1.Controller_Values.Control_Freeze                 = 0;
  //----------------------------------------------------------------------------
  
  //PH2 voltages ---------------------------------------------------------------
  Phase_Values_PH2.Phase_Voltage.FilterCounter      = 0;
  Phase_Values_PH2.Phase_Voltage.Filtered           = 0;
  Phase_Values_PH2.Phase_Voltage.Raw                = 0;
  Phase_Values_PH2.Phase_Voltage.Rectified          = 0;
  Phase_Values_PH2.Phase_Voltage.Rectified_Shift    = 0;
  Phase_Values_PH2.Phase_Voltage.Vin_div_Averaged2  = 0;
  Phase_Values_PH2.Phase_Voltage.Max_Peak  = 0;
  
  //PH2 currents ---------------------------------------------------------------
  Phase_Values_PH2.Phase_Current.FilterCounter      = 0;
  Phase_Values_PH2.Phase_Current.Filtered           = 0;
  Phase_Values_PH2.Phase_Current.Raw                = 0;
  Phase_Values_PH2.Phase_Current.Rectified          = 0;
  Phase_Values_PH2.Phase_Current.Rectified_Shift    = 0;
  Phase_Values_PH2.Phase_Current.Vin_div_Averaged2  = 0;
  
  //PH2 flags ------------------------------------------------------------------
  Phase_Values_PH2.Control_Status_Flags.bits.Soft_Start_Zero_Cross     = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_last         = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.Relay                     = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.Stop                      = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.Fault                     = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed      = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed_last = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.CCM                       = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.BurstModeDetected         = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.BurstModeLatched          = 0;
  Phase_Values_PH2.Control_Status_Flags.bits.Reference_Changed         = 0;
  
  //PH2 controller numbers -----------------------------------------------------
  Phase_Values_PH2.Controller_Values.PWM_Counter                    = 0;
  Phase_Values_PH2.Controller_Values.Duty_Cycle_Set_Value           = 0;
  Phase_Values_PH2.Controller_Values.CCM_Counter                    = 0;
  Phase_Values_PH2.Controller_Values.DFF_Value                      = 0;
  Phase_Values_PH2.Controller_Values.IAC_Reference_l                = 0;
  Phase_Values_PH2.Controller_Values.IAC_Reference                  = 0;
  Phase_Values_PH2.Controller_Values.BurstModeCycleCounter          = 0;
  Phase_Values_PH2.Controller_Values.Control_Freeze                 = 0;
  //----------------------------------------------------------------------------

  //PFC common Flags
  PFC_Flags.value = 0x00000000;
  PFC_Flags.bits.AGOn = 1;
  PFC_Flags.bits.Stop = 1;  
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

