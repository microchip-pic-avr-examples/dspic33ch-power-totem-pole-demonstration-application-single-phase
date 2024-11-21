/**
 * Power Controller Typedef File
 * 
 * @file      drv_pwrctrl_app_TPBLPFC.c
 * 
 * @ingroup   p-controller-app-TPBLPFC
 * 
 * @brief     This is the driver source file for the power controller app
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

#include "../../../mcc_generated_files/system/pins.h"
#include "../../../mcc_generated_files/cmp/cmp1.h"
//#include "../../../mcc_generated_files/pwm_hs/pwm.h"
#include "drv_controller_TPBLPFC.h"
#include "../../devices/dev_TPBLPFC_typedef.h"
#include "PFC_frameworkSetup.h"
#include "../../misc/MCC_add_on.h"
#include "drv_pwrctrl_app_TPBLPFC.h"

/*******************************************************************************
 * @details static functions in drv_pwrctrl_app_TPBLPFC.c    
 *          functions are performance optimized inlined        
 *******************************************************************************/

static void VOUTaveraging(void);
static void VOUT_HystereticCheck(void);
static void ZeroCross_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void SoftstartAfterZC_PHx_GTIMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void SoftstartAfterZC_PHx_PFCMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void CCM_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void DutyCycleFeedForward_PHx(struct PHASE_VALUES_s* PhaseX);
static void BurstMode_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void Adaptive_Currentcontroller_Gain(uint16_t PWMnr);

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      Application_handler_PH123
 * @param   void
 * @return  void
 * @brief   ISR drive application handler
 * @details 
 *          switch control PH1, PH2 time optimized for
 *          1/2 PH different modes based on drv_usersetup_TPBLPFC.h settings 
 **********************************************************************************/

void Application_handler_PH1(void)
{

  _LATC12 = Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range;
  //    _LATC12 = Phase_Values_PH1.Control_Status_Flags.bits.Soft_Start_Zero_Cross;
  DAC1DATH = (Phase_Values_PH1.Controller_Values.IAC_Reference >> 1);

  VOUTaveraging();

  //Vout limits and dV/dt check
#ifdef ADAPTIVE_STEPRESPONSE_ENABLED
  if ((pwr_ctrl_state == PCS_UP_AND_RUNNING))
  {
    VOUT_HystereticCheck();
  }
#endif
  //PHASE #1  
  Handler_PHx(&Phase_Values_PH1, 1);

}

void Application_handler_PH2(void)
{

#if defined MODE_INTERLEAVED
  //PHASE #2  
  Handler_PHx(&Phase_Values_PH2, 2);
#endif

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      Handler_PHx
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   Phase handler
 * @details 
 *          phase specific functionality to drive and control totempole
 *          bridgeless topology:
 *          zero-cross, burst-mode, softstart after zero cross, CCM/DCM,
 *          outer voltage control loop, inner average current control loop,
 *          adaptive current controller gain, duty cycle feedforward, 
 *          PWM duty cycle write back 
 **********************************************************************************/

static void __inline__ Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_OK)
  {
    //<ZC range detection and handling, polarity change check
    ZeroCross_PHx(PhaseX, PWMnr);

#ifdef VOUT_PRECHARGE_PCS_ENABLE
    if (pwr_ctrl_state > PCS_VACOK)
#else
    if (PhaseX->Control_Status_Flags.bits.Relay)
#endif
    {
#ifdef AC_CYCLE_SKIPPING_ENABLED 
      // Burst mode checked after startup ramp or if ref changed during run mode
      if ((pwr_ctrl_state == PCS_UP_AND_RUNNING) || PhaseX->Control_Status_Flags.bits.Reference_Changed)
      {
        BurstMode_PHx(PhaseX, PWMnr);
      }
#endif
#ifdef MODE_GRID_TIE_INVERTER
      SoftstartAfterZC_PHx_GTIMode(PhaseX, PWMnr); //GTI special switch handling 
#else
      //#if defined MODE_PFC || defined MODE_INTERLEAVED
      SoftstartAfterZC_PHx_PFCMode(PhaseX, PWMnr); //PFC mode standard switch handling
#endif
      //synch switch handling based on polarity 
      if ((!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
        (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range) &&
        ((pwr_ctrl_state == PCS_UP_AND_RUNNING) || PhaseX->Control_Status_Flags.bits.Reference_Changed))
      {
#ifdef VOLTAGE_LOOP
        if (Vout_Control.Reference.Voltage_Loop_Output > BURST_MODE_VMC_REF_FREEZE)
#endif
          CCM_PHx(PhaseX, PWMnr);
      }
    }

#ifdef VOLTAGE_LOOP
    if ((Vout_Control.Vout.FilterCounter < 1) && (PWMnr == 1))
    {
      //outer voltage loop
      if (PhaseX->Controller_Values.Control_Freeze)
      {
        Vout_Control.Reference.Voltage_Loop_Output = BURST_MODE_VMC_REF_FREEZE;
      }
      else
      {
        SMPS_Controller2P2ZUpdate(&VMC_2p2z, &Vout_Control.Vout.Filtered, Vout_Control.Reference.Reference_Internal,
          &Vout_Control.Reference.Voltage_Loop_Output);
      }
    }
#endif

    //Current loop
    if (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range)
    {
#ifdef VOLTAGE_LOOP
      PhaseX->Controller_Values.IAC_Reference_l = (__builtin_mulss((Vout_Control.Reference.Voltage_Loop_Output), (PhaseX->Phase_Voltage.Vin_div_Averaged2)));
#else
      PhaseX->Controller_Values.IAC_Reference_l = (__builtin_mulss((Vout_Control.Reference.Reference_Internal << 3), PhaseX->Phase_Voltage.Vin_div_Averaged2));
#endif

      PhaseX->Controller_Values.IAC_Reference = (PhaseX->Controller_Values.IAC_Reference_l >> 15); //shift only 15 because of 2 signed bits in mulss

#ifndef VOLTAGE_LOOP
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VDC_Input_Voltage)
      {
        PhaseX->Controller_Values.IAC_Reference = (Vout_Control.Reference.Reference_Internal); //IAC reference is 31 digits per Ampere left shifted by 4
      }
#endif     

      PhaseX->Phase_Current.Rectified_Shift = (uint16_t) (PhaseX->Phase_Current.Rectified << 4); //<<< 4
      //inner current loop
      if (!PhaseX->Controller_Values.Control_Freeze) //Freezed because of cycle skipping
      {
        Adaptive_Currentcontroller_Gain(PWMnr);
        PHx_AVG_CM2p2z[PWMnr].KfactorCoeffsB = Adaptive_Gain_Factor;

#ifdef MODE_INTERLEAVED
        //////removed 12.06.24 because we did not get max current, limit 32000 !!!
        //////PhaseX->Controller_Values.IAC_Reference >>= 1; //in interleaved mode current demand is half, split to 2 phases
#endif

        XFT_SMPS_Controller2P2ZUpdate(&PHx_AVG_CM2p2z[PWMnr], &PhaseX->Phase_Current.Rectified_Shift, PhaseX->Controller_Values.IAC_Reference,
          &(PhaseX->Controller_Values.Duty_Cycle_Set_Value));
      }

#ifdef DUTY_RATIO_FEEDFORWARD_ENABLED     
      //DC ff / switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))
      {
        DutyCycleFeedForward_PHx(PhaseX);
      }
#endif

      //Vout precharge option with openloop fixed duty cycle 50% at LV and 25% at HV
      if (pwr_ctrl_state == PCS_VOUT_PRELOAD)
      {
#if HIGH_VOLTAGE        
#ifdef MODE_INTERLEAVED
        PhaseX->Controller_Values.Duty_Cycle_Set_Value = MPER >> 2;
#else
        PhaseX->Controller_Values.Duty_Cycle_Set_Value = MPER >> 2; //open loop precharge with fix 25% duty cycle
#endif
#else
        PhaseX->Controller_Values.Duty_Cycle_Set_Value = MPER >> 1;
#endif
      }

      //check min/max value
      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value < DCMIN) PhaseX->Controller_Values.Duty_Cycle_Set_Value = 0;
      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value > DCMAX) PhaseX->Controller_Values.Duty_Cycle_Set_Value = DCMAX;


      //      PWM_TriggerCompareValueSet(PWMnr,PhaseX->Controller_Values.Duty_Cycle_Set_Value >> 1);
      if (PWMnr == 1) PG1TRIGA = (Phase_Values_PH1.Controller_Values.Duty_Cycle_Set_Value >> 1);
      if (PWMnr == 2) PG2TRIGA = (Phase_Values_PH2.Controller_Values.Duty_Cycle_Set_Value >> 1);

      //update duty cycle with controller output value only when out of zero cross startup
      //zero cross startup is in open loop in function SoftstartAfterZC_PHx_XXX
      if (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
      {
        PWM_DutyCycleSet(PWMnr, PhaseX->Controller_Values.Duty_Cycle_Set_Value);
      }
    }

    PhaseX->Control_Status_Flags.bits.VAC_Polarity_last = PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity;
  }
}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      VOUTaveraging
 * @param   void
 * @return  void
 * @brief   average voltage calculation of Vout
 * @details 
 *          Vout averaging from Vin zero cross to zero cross.
 *          for single phase with 50Hz that calculation is done every 10ms (100Hz)
 *          optional averaging of input current, 
 **********************************************************************************/

static void __inline__ VOUTaveraging(void)
{

  static uint32_t VoutAVGsum, PH1AVGCurrentsum, PH2AVGCurrentsum;
  static uint32_t PH1AVGCurrentOffset, PH2AVGCurrentOffset;
  static uint16_t vac_pol_changed_counter;
  static uint16_t PH1vac_pol_changed_counter, PH2vac_pol_changed_counter;

  Vout_Control.Vout.FilterCounter++;
  Phase_Values_PH1.Phase_Current.FilterCounter++;
  Phase_Values_PH2.Phase_Current.FilterCounter++;

  VoutAVGsum += Vout_Control.Vout.Raw;

  //averaging of input current
  PH1AVGCurrentsum += Phase_Values_PH1.Phase_Current.Rectified; //time consuming instruction, optimize with asm
  //  PH2AVGCurrentsum += Phase_Values_PH2.Phase_Current.Rectified;

  if ((Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed)
    && (!Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
  {
    vac_pol_changed_counter++;
    PH1vac_pol_changed_counter++;
    PH2vac_pol_changed_counter++;
  }

  if (((vac_pol_changed_counter > 0) && (Vout_Control.Vout.FilterCounter > 1))
    || (Vout_Control.Vout.FilterCounter > 8000))
  {
    Vout_Control.Vout.PreviousValue = Vout_Control.Vout.Filtered;
    Vout_Control.Vout.Filtered = (uint16_t) (__builtin_divud(VoutAVGsum, Vout_Control.Vout.FilterCounter - 1));
    VoutAVGsum = 0;
    vac_pol_changed_counter = 0;
    Vout_Control.Vout.FilterCounter = 0;
  }

  // after power up during initialization the current sensor calibration is done
  if (pwr_ctrl_state == PCS_INIT)
  {
    PH1AVGCurrentOffset += Phase_Values_PH1.Phase_Current.Raw;
    PH2AVGCurrentOffset += Phase_Values_PH2.Phase_Current.Raw;

    if (((PH1vac_pol_changed_counter > 1) && (Phase_Values_PH1.Phase_Current.FilterCounter > 1))
      || (Phase_Values_PH1.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      Phase_Values_PH1.Phase_Current.Offset = (uint16_t) (__builtin_divud(PH1AVGCurrentOffset, Phase_Values_PH1.Phase_Current.FilterCounter - 1));
      PH1AVGCurrentsum = 0;
      PH1AVGCurrentOffset = 0;
      PH1vac_pol_changed_counter = 0;
      Phase_Values_PH1.Phase_Current.FilterCounter = 0;
    }
    if (((PH2vac_pol_changed_counter > 1) && (Phase_Values_PH2.Phase_Current.FilterCounter > 1))
      || (Phase_Values_PH2.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      Phase_Values_PH2.Phase_Current.Offset = (uint16_t) (__builtin_divud(PH2AVGCurrentOffset, Phase_Values_PH2.Phase_Current.FilterCounter - 1));
      PH2AVGCurrentsum = 0;
      PH2AVGCurrentOffset = 0;
      PH2vac_pol_changed_counter = 0;
      Phase_Values_PH2.Phase_Current.FilterCounter = 0;
    }
  }

  //averaging of input current  
  if (pwr_ctrl_state == PCS_UP_AND_RUNNING)
  {
    if (((PH1vac_pol_changed_counter > 1) && (Phase_Values_PH1.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH1.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      Phase_Values_PH1.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH1AVGCurrentsum, Phase_Values_PH1.Phase_Current.FilterCounter - 1));
      PH1AVGCurrentsum = 0;
      PH1AVGCurrentOffset = 0;
      PH1vac_pol_changed_counter = 0;
      Phase_Values_PH1.Phase_Current.FilterCounter = 0;
    }

    //    if (((PH2vac_pol_changed_counter > 1) && (Phase_Values_PH2.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH2.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    //    {
    //      Phase_Values_PH2.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH2AVGCurrentsum, Phase_Values_PH2.Phase_Current.FilterCounter - 1));
    //      PH2AVGCurrentsum = 0;
    //      PH2AVGCurrentOffset = 0;
    //      PH2vac_pol_changed_counter = 0;
    //      Phase_Values_PH2.Phase_Current.FilterCounter = 0;
    //    }
  }

  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last = Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed;

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      ZeroCross_PHx
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   zero cross check
 * @details 
 *          check if in zero cross are where all switches are off
 *          check for polarity change
 *          zero cross range flag and polarity flag are coming from primary
 *          core based on data from AC-acquisition board
 **********************************************************************************/

static void __inline__ ZeroCross_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  //switch off main and synch switches at ZC area ---------------------------
  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range)
  {
    PWM_OverrideHighEnable(PWMnr);
    PWM_OverrideLowEnable(PWMnr);
    PWM_H_N_SetLow();
    PWM_L_N_SetLow();

    //reset duty cycle to 0 to startup later from low duty cycle values
    PWM_DutyCycleSet(PWMnr, 0);

    PhaseX->Controller_Values.CCM_Counter = 1;
    //CCM_Counter used as hysteresis around the CCM_CURRENT value 
    //in MODE_GRID_TIE_INVERTER the CCM counter can be greater as 
    //CCM level at end of half wave and synch is enabled
    //at the begin of the next half wave after ZC range synch switch is enabled
    //because of DCM counter needs to be count down after ZC range
  }

  //polarity change check ---------------------------------------------------
  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity != PhaseX->Control_Status_Flags.bits.VAC_Polarity_last)
  {
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 1;

    PhaseX->Controller_Values.BurstModeCycleCounter++; //Burst Mode cycle count
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      SoftstartAfterZC_PHx_GTIMode
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   softstart after zero cross for mode Grid Tie Inverter
 * @details 
 *          open loop startup for a certain amount of PWM pulses starting 
 *          with small duty cycle and increase the duty cycle 
 *          GTI has different openloop switches than PFC mode 
 *          GTI: no Burst Mode, no Voltage loop, no Adaptive Gain, 
 *               no Duty Cycle Feedforward, no Adaptive Stepresponse
 **********************************************************************************/

static void __inline__ SoftstartAfterZC_PHx_GTIMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  uint16_t pg1dc_;

  //switch on main switches after polarity change and ZC range is over and before ZC startup 
  if (PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
    PhaseX->Controller_Values.PWM_Counter = 0;

    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      //startup after ZC like PFC
      PWM_SwapEnable(PWMnr);
      PWM_OverrideLowDisable(PWMnr);
      PWM_OverrideHighEnable(PWMnr);
    }
    else
    {
      //startup after ZC like PFC
      PWM_SwapDisable(PWMnr);
      PWM_OverrideHighDisable(PWMnr);
      PWM_OverrideLowEnable(PWMnr);
    }
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 0;
  }

  //open loop startup after ZC
  if (PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
  {
    // openloop startup after ZC
    // number of PWM periods: 
    if (++PhaseX->Controller_Values.PWM_Counter < OPEN_LOOP_STARTUP_PULSES)
    {
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      //      GPIO_Y_L_Toggle();
      PWM_DutyCycleSet(PWMnr, pg1dc_);
    }
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0;

      PhaseX->Controller_Values.PWM_Counter = 30;

      // LF switch starts up after a few cycles
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
      {
        PWM_SwapDisable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
        PWM_OverrideLowEnable(PWMnr);

        if (pwr_ctrl_state > PCS_START_CONTROL)
        {
          PWM_H_N_SetLow();
          PWM_L_N_SetHigh();
        }
      }
      else
      {
        PWM_SwapEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
        PWM_OverrideHighEnable(PWMnr);

        if (pwr_ctrl_state > PCS_START_CONTROL)
        {
          PWM_L_N_SetLow();
          PWM_H_N_SetHigh();
        }
      }
    }
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      SoftstartAfterZC_PHx_PFCMode
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   softstart after zero cross for mode PFC
 * @details 
 *          depending on mode the PWM start after zero cross is different
 *          standard is an open loop startup for a certain amount of
 *          PWM pulses starting with small duty cycle and increase the 
 *          duty cycle until it reaches the demanded value from control
 *          PFC: Burst Mode, Voltage loop, Adaptive Gain, 
 *               Duty Cycle Feedforward, Adaptive Stepresponse
 *               Chargepump Discharge hardware option can be enabled in addition
 **********************************************************************************/

static void __inline__ SoftstartAfterZC_PHx_PFCMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  uint16_t pg1dc_;

  //switch on main switches after polarity change and ZC range is over and before ZC startup 
  if (PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
    PhaseX->Controller_Values.PWM_Counter = 0;

    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched) && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        PWM_SwapEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
        PWM_OverrideHighEnable(PWMnr);
      }
    }
    else
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched) && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        PWM_SwapDisable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
        PWM_OverrideLowEnable(PWMnr);
      }
    }
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 0;
  }

  //LF switch start after x-cycles in OL like PFC mode 
  if (PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
  {
#ifdef COMMON_MODE_DETECTION
    if (((!PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity) &&
      (AC_N.Phase_Voltage.Raw < (Vout_Control.Vout.Filtered - (Vout_Control.Vout.Filtered >> 2)))) ||
      ((PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity) &&
      (AC_N.Phase_Voltage.Raw > 500 /*200mV*/))
      )
#else
    // openloop startup after ZC  
    //    if (++PhaseX->Controller_Values.PWM_Counter < OPEN_LOOP_STARTUP_PULSES)
    if (++PhaseX->Controller_Values.PWM_Counter < OpenLoopStartupDuration)
#endif      
    {

#ifdef COMMON_MODE_DETECTION
      ++PhaseX->Controller_Values.PWM_Counter; //only for AC_N needed to get OL pulses
#endif

#ifdef CHARGEPUMP_ENABLED 
      // charge pump I/O handling: 100us on
      // if no charge pump hardware I/O pins are set without acting
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        GPIO_Y_L_SetHigh();
      else
        GPIO_Y_H_SetHigh();
#else     
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      //      GPIO_Y_L_Toggle();
      PWM_DutyCycleSet(PWMnr, pg1dc_);
#endif
    }
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0;

      PhaseX->Controller_Values.PWM_Counter = 30;

#if defined CHARGEPUMP_ENABLED  
      // charge pump I/O handling
      GPIO_Y_H_SetLow();
      GPIO_Y_L_SetLow();

      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched) && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        {
          PWM_SwapEnable(PWMnr);
          PWM_OverrideLowDisable(PWMnr);
          PWM_OverrideHighEnable(PWMnr);
        }
        else
        {
          PWM_SwapDisable(PWMnr);
          PWM_OverrideHighDisable(PWMnr);
          PWM_OverrideLowEnable(PWMnr);
        }
      }
#endif

      // LF switches are on after open loop startup,
#if defined RECTIFIER_SWITCH 
      // LF switch starts up after a few cycles
      if (!PhaseX->Controller_Values.Control_Freeze)
      {
        if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        {
          if (pwr_ctrl_state > PCS_START_CONTROL)
          {
            PWM_H_N_SetLow();
            PWM_L_N_SetHigh();
          }
        }
        else
        {
          if (pwr_ctrl_state > PCS_START_CONTROL)
          {
            PWM_L_N_SetLow();
            PWM_H_N_SetHigh();
          }
        }
      }
#endif   
    }
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      CCM_PHx
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   based on current level switch between DCM and CCM
 * @details 
 *          based on defined current level we decide if in DCM or CCM
 *          in DCM the boost stage runs asynchronous, no syncronous switch
 *          in CCM the boost stage runs with syncronous switches
 *          a hysteresis of 6 samples
 **********************************************************************************/

static void __inline__ CCM_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  uint16_t var1_ui, var2_ui;

  // current fix for DCM;
  var1_ui = PhaseX->Phase_Current.Rectified;
  var2_ui = CCM_CURRENT;

  if (var1_ui > var2_ui)
  {
    PhaseX->Controller_Values.CCM_Counter++;
  }
  else if (var1_ui < var2_ui)
  {
    PhaseX->Controller_Values.CCM_Counter--;
  }

  if (PhaseX->Controller_Values.CCM_Counter > 6)
  {
    PhaseX->Controller_Values.CCM_Counter = 6;
    PhaseX->Control_Status_Flags.bits.CCM = 1;
  }
  if (PhaseX->Controller_Values.CCM_Counter < 1)
  {
    PhaseX->Controller_Values.CCM_Counter = 1;
    PhaseX->Control_Status_Flags.bits.CCM = 0;
  }

#ifdef SYNCHRONOUS_ACTIVE_SWITCH
  if (PhaseX->Control_Status_Flags.bits.CCM)
  {
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
#ifndef MODE_GRID_TIE_INVERTER       
      PWM_OverrideHighDisable(PWMnr); //PWM ON
#else
      PWM_OverrideLowDisable(PWMnr); //PWM ON
#endif        
    }
    else
    {
#ifndef MODE_GRID_TIE_INVERTER     
      PWM_OverrideLowDisable(PWMnr); //PWM ON
#else
      PWM_OverrideHighDisable(PWMnr); //PWM ON
#endif        
    }
  }
  else
  {
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
#ifndef MODE_GRID_TIE_INVERTER     
      PWM_OverrideHighEnable(PWMnr); //PWM OFF
#else
      PWM_OverrideLowEnable(PWMnr); //PWM OFF
#endif        
    }
    else
    {
#ifndef MODE_GRID_TIE_INVERTER     
      PWM_OverrideLowEnable(PWMnr); //PWM OFF
#else
      PWM_OverrideHighEnable(PWMnr); //PWM OFF
#endif        
    }
  }
#endif 

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      DutyCycleFeedForward_PHx
 * @param   struct PHASE_VALUES_s * PhaseX
 * @return  void
 * @brief   pre-calculate a duty ratio
 * @details 
 *          The basic idea of DFF control is to pre-calculate a duty ratio 
 *          to alleviate the feedback controller of the task. 
 *          For the boost topology operating in continuous conduction mode, the
 *          duty-ratio, dFF, is:
 *                   Vout - VAC
 *             dFF = ------------
 *                      Vout      
 **********************************************************************************/

static void __inline__ DutyCycleFeedForward_PHx(struct PHASE_VALUES_s * PhaseX)
{
  uint32_t var_ul;
  uint16_t var1_ui;

  if (Vout_Control.Vout.Filtered > PhaseX->Phase_Voltage.Rectified)
  {
    var1_ui = Vout_Control.Vout.Filtered - PhaseX->Phase_Voltage.Rectified; //Vout - Vin
    var_ul = (uint32_t) var1_ui * MPER;
    var1_ui = __builtin_divud(var_ul, Vout_Control.Vout.Filtered);
    PhaseX->Controller_Values.Duty_Cycle_Set_Value = PhaseX->Controller_Values.Duty_Cycle_Set_Value + (var1_ui >> 2);
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      BurstMode_PHx
 * @param   struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return  void
 * @brief   skip cycles at light load
 * @details 
 *          AC cycle skipping improves PFC light-load efficiency. 
 *          The number of AC cycles to be skipped is inversely proportional
 *          to the load.
 **********************************************************************************/

static void __inline__ BurstMode_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  if ((Vout_Control.Reference.Voltage_Loop_Output < BURST_MODE_LEVEL_HIGH) &&
    !PhaseX->Control_Status_Flags.bits.BurstModeLatched)
  {
    PhaseX->Control_Status_Flags.bits.BurstModeDetected = 1;
  }

  if (PhaseX->Control_Status_Flags.bits.BurstModeDetected &&
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_last)
  {
    PhaseX->Control_Status_Flags.bits.BurstModeLatched = 1;
    PhaseX->Control_Status_Flags.bits.BurstModeDetected = 0;
    PhaseX->Controller_Values.Control_Freeze = 1;
    PhaseX->Controller_Values.BurstModeCycleCounter = 0;
    PhaseX->Controller_Values.BurstModeCyclesOn = 0;

    PWM_OverrideHighEnable(PWMnr);
    PWM_OverrideLowEnable(PWMnr);
    PWM_H_N_SetLow();
    PWM_L_N_SetLow();
  }

  if ((PhaseX->Controller_Values.BurstModeCycleCounter > 1))
  {
    PhaseX->Control_Status_Flags.bits.BurstModeLatched = 0;
    PhaseX->Controller_Values.Control_Freeze = 0;
    PhaseX->Controller_Values.BurstModeCycleCounter = 0;
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      VOUT_HystereticCheck
 * @param   void
 * @return  void
 * @brief   Vout dV/dT check
 * @details 
 *          if Vout rise up higher than 112% of Vref all switches 
 *          are switched off 
 *          that prevents large overshoots when load disappears or
 *          load goes down to a low level very quick 
 * 
 *          if we use Vout_Control.Vout.Filtered as the Vout comparison value, 
 *          the assessment is only made in the ZC as the Vout average value 
 *          is determined from ZC to ZC measured.
 *          if we use Vout_Control.Vout.Raw as the Vout comparison value, 
 *          the assessment is made cycle by cycle, every10us.
 *          depending on the load jump, this can lead to hickups.
 **********************************************************************************/

static void __inline__ VOUT_HystereticCheck(void)
{

  if ((Vout_Control.Vout.Filtered > Vout_Control.Reference.VOUT_dV_dt_Hystresis_High))
    //  if ((Vout_Control.Vout.Raw > Vout_Control.Reference.VOUT_dV_dt_Hystresis_High))
  {
    Vout_Control.Reference.VOUT_dV_dt_Active = 1;
    Vout_Control.Reference.VOUT_dV_dt_Up = 1;
    PWM_OverrideHighEnable(1);
    PWM_OverrideLowEnable(1);
    PWM_OverrideHighEnable(2);
    PWM_OverrideLowEnable(2);
    PWM_OverrideHighEnable(3);
    PWM_OverrideLowEnable(3);
    PWM_H_N_SetLow();
    PWM_L_N_SetLow();
  }

  if ((Vout_Control.Vout.Raw < Vout_Control.Reference.VOUT_Hystresis_High) &&
    (Vout_Control.Vout.Raw > Vout_Control.Reference.VOUT_Hystresis_Low))
  {
    Vout_Control.Reference.VOUT_dV_dt_Active = 0;
    Vout_Control.Reference.VOUT_dV_dt_Up = 0;
  }

}

/***********************************************************************************
 * @ingroup drv_pwrctrl_app_TPBLPFC
 * @fn      Adaptive_Currentcontroller_Gain
 * @param   void
 * @return  void
 * @brief   current controller gain adapation based on load
 * @details 
 *          voltage controller output reflects the load
 *          coefficients are based on gain of 10
 *          more load -> higher voltage controller output -> lower gain
 *          less load -> lower voltage controller output -> higher gain
 *          using fractional number the B-coeffs (gain) are scaled down from 
 *          gain of 10 to gain of 1 which is a gain factor of 0.1
 *          numbers range lead to factor of 0.5 to 0.1 based on load
 *          numbers are considered to be in the range of 0 to 32767, 0 to +1,
 *          based on fractional 1Q15 format at all calculations 
 *          gain is also adapted on input voltage 
 *          higher input voltage -> more gain
 **********************************************************************************/

//static void __inline__ Adaptive_Currentcontroller_Gain(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)

static void __inline__ Adaptive_Currentcontroller_Gain(uint16_t PWMnr)
{
  uint32_t mul01;
  uint16_t mul02;

#ifndef ADAPTIVE_GAIN_ENABLED
  PFC_Flags.bits.AGOn = 0;
#endif

#ifdef ADAPTIVE_GAIN_SLIDER_MODE
  Adaptive_Gain_Factor = Adaptive_Gain_Factor;
#else

#if defined (MODE_PFC) || defined (MODE_INTERLEAVED)
  if (PFC_Flags.bits.AGOn)
  {
    mul01 = ((uint32_t) Vout_Control.Reference.Voltage_Loop_Output * GAINSLOPE_HIGHLINE);
    mul02 = (uint16_t) (mul01 >> 15);
    Adaptive_Gain_Factor = GAINOFFSET_HIGHLINE - mul02;

    if (Phase_Values_PH1.Phase_Voltage.Filtered < 583) // #define machen    
    {
      mul01 = ((uint32_t) Vout_Control.Reference.Voltage_Loop_Output * GAINSLOPE_LOWLINE);
      mul02 = (uint16_t) (mul01 >> 15);
      Adaptive_Gain_Factor = GAINOFFSET_LOWLINE - mul02;
    }
    if (Adaptive_Gain_Factor < MIN_GAIN_FACTOR)
      Adaptive_Gain_Factor = MIN_GAIN_FACTOR;
  }
  else
  {
    Adaptive_Gain_Factor = 3276; //factor = 0.1
  }
#endif

#ifdef MODE_GRID_TIE_INVERTER
  Adaptive_Gain_Factor = 8192; //factor = 0.25 (32676*0.25)
#endif

#endif
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

