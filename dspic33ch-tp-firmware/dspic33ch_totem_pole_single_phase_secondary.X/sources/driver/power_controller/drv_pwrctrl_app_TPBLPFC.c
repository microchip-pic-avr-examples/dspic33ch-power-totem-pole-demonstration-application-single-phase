/*
© [2024] Microchip Technology Inc. and its subsidiaries.

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

#include "drv_controller_TPBLPFC.h"
#include "../../devices/dev_TPBLPFC_typedef.h"
#
#include "PFC_frameworkSetup.h"

#include "../../misc/MCC_add_on.h"
#include "drv_pwrctrl_app_TPBLPFC.h"

/*******************************************************************************
 * @details static functions in main_task.c    
 *          functions are performance optimized inlined        
 *******************************************************************************/

static void VOUTaverging(void);
static void VOUT_HystereticCheck(void);
static void ZeroCross_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void SoftstartAfterZC_PHx_GTIMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr); //test function for GTI
static void SoftstartAfterZC_PHx_PFCMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr); //origin function
static void CCM_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void DutyCycleFeedForward_PHx(struct PHASE_VALUES_s* PhaseX);
static void BurstMode_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);

/*******************************************************************************
 * @fn         PWM_handler_PH123
 * @brief      
 * @param      none
 * @return     none
 * @details    ISR handler for switch control PH1, PH2, PH3 time optimized for
 *             1/2/3PH different modes based on drv_usersetup_TPBLPFC.h settings 
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

void PWM_handler_PH123(void)
{

  //  _LATB14 = Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity;
  DAC1DATH = AC_N.Phase_Voltage.Raw;
  //show VAC only for demo purpose
  //    DAC1DATH = (Phase_Values_PH1.Phase_Voltage.Rectified); 

  VOUTaverging();

  //<<<<<<<<<<<  Vout limits and dV/dt check  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#ifdef ADAPTIVE_STEPRESPONSE_ENABLED
  if ((pwr_ctrl_state == PCS_UP_AND_RUNNING))
  {
    VOUT_HystereticCheck();
  }
#endif

  //<<<<<<<<<<<  PHASE #1  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Handler_PHx(&Phase_Values_PH1, 1);

#if defined MODE_INTERLEAVED
  //<<<<<<<<<<<  PHASE #2  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Handler_PHx(&Phase_Values_PH2, 2);
#endif

}

/*******************************************************************************
 * @fn         Handler_PHx
 * @brief      
 * @param      struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_OK)
  {
    //<<ZC range detection and handling << polarity change check----------------
    ZeroCross_PHx(PhaseX, PWMnr);

    if (PhaseX->Control_Status_Flags.bits.Relay)
    {
#ifdef AC_CYCLE_SKIPPING_ENABLED 
      // Burst mode checked after startup ramp or if ref changed during run mode
      if((pwr_ctrl_state == PCS_UP_AND_RUNNING) || PhaseX->Control_Status_Flags.bits.Reference_Changed)
      {  
        BurstMode_PHx(PhaseX, PWMnr);
      }
#endif
#ifdef MODE_GRID_TIE_INVERTER
      SoftstartAfterZC_PHx_GTIMode(PhaseX, PWMnr); //GTI special switch handling 
#endif
#if defined MODE_PFC || defined MODE_INTERLEAVED
      SoftstartAfterZC_PHx_PFCMode(PhaseX, PWMnr); //PFC mode standard switch handling
#endif
      //<< synch switch handling based on polarity -----------------------------
      if ((!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) && 
          (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range) &&
          ((pwr_ctrl_state == PCS_UP_AND_RUNNING) || PhaseX->Control_Status_Flags.bits.Reference_Changed))
      {
#ifdef VOLTAGE_LOOP
        if (Vout_Control.Reference.Voltage_Loop_Output > BURST_MODE_VMC_REF_FREEZE)
#endif
          CCM_PHx(PhaseX, PWMnr); //comment out for diode mode and DC mode
      }
    }

    //<< Voltage loop-----------------------------------------------------------
#ifdef VOLTAGE_LOOP
    //<<< only testing >>>
    if ((Vout_Control.Vout.FilterCounter < 1) && (PWMnr == 1))
    {
      //<< outer voltage loop
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
    //<< Current loop-----------------------------------------------------------
    if ((!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
    {
#ifdef VOLTAGE_LOOP
      PhaseX->Controller_Values.IAC_Reference_l = (__builtin_mulss((Vout_Control.Reference.Voltage_Loop_Output), (PhaseX->Phase_Voltage.Vin_div_Averaged2)));
#else
      PhaseX->Controller_Values.IAC_Reference_l = (__builtin_mulss((Vout_Control.Reference.Reference_Internal << 3), PhaseX->Phase_Voltage.Vin_div_Averaged2)); // comment out for V loop#endif
#endif

      PhaseX->Controller_Values.IAC_Reference = (PhaseX->Controller_Values.IAC_Reference_l >> 15); //shift only 15 because of 2 signed bits in mulss

      //<<< Current clamp option
      //      if(PhaseX->Controller_Values.IAC_Reference > I_INCLAMP) 
      //      {
      //        PhaseX->Controller_Values.IAC_Reference = I_INCLAMP;
      //      }

#ifndef VOLTAGE_LOOP
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VDC_Input_Voltage)
      {
        PhaseX->Controller_Values.IAC_Reference = (Vout_Control.Reference.Reference_Internal << 1);
      }
#endif     

      PhaseX->Phase_Current.Rectified_Shift = (uint16_t) (PhaseX->Phase_Current.Rectified << 4); //4); //3/4/5??? je mehr left shift je mehr gain, je unruhiger, aber THD wird immer besser
      //<< inner current loop
      if (!PhaseX->Controller_Values.Control_Freeze) //Freezed ?
      {

#ifndef ADAPTIVE_GAIN_ENABLED
        PFC_Flags.bits.AGOn = 0;
#endif
        // 16000 = 0.5 fract; 3276 = 0.1 fract 
        // Gain should be within 0.1 and 0.5
        // compensator calculated with Gain of 10

        //comment out for slider Gain
        if (PFC_Flags.bits.AGOn && (Vout_Control.Reference.Voltage_Loop_Output < 16000))
        {
          Adaptive_Gain_Factor = 16000 - Vout_Control.Reference.Voltage_Loop_Output;
          if (Adaptive_Gain_Factor < 3276) Adaptive_Gain_Factor = 3276;
        }
        else
        {
          Adaptive_Gain_Factor = 3276; //factor = 0.1
        }
        //comment out for slider Gain

        PHx_AVG_CM2p2z.KfactorCoeffsB = Adaptive_Gain_Factor;

        XFT_SMPS_Controller2P2ZUpdate(&PHx_AVG_CM2p2z, &PhaseX->Phase_Current.Rectified_Shift, PhaseX->Controller_Values.IAC_Reference,
          &(PhaseX->Controller_Values.Duty_Cycle_Set_Value));
      }

#ifdef DUTY_RATIO_FEEDFORWARD_ENABLED     
      //<< DC ff / switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        DutyCycleFeedForward_PHx(PhaseX);
      }
#endif

      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value < DCMIN) PhaseX->Controller_Values.Duty_Cycle_Set_Value = 0; //<<<<<FTX DCMIN;
      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value > DCMAX) PhaseX->Controller_Values.Duty_Cycle_Set_Value = DCMAX;

      PWM_DutyCycleSet(PWMnr, PhaseX->Controller_Values.Duty_Cycle_Set_Value);
    }

    PhaseX->Control_Status_Flags.bits.VAC_Polarity_last = PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity;
  }
  
}

/*******************************************************************************
 * @fn         VOUTaverging
 * @brief      Average value calculations
 * @param      none
 * @return     none
 * @details    Averaging of Vout, filter Vout, (Iin) 
 *             from ZC to ZC whatever phase
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ VOUTaverging(void)
{
  static uint32_t VoutAVGsum, PH1AVGCurrentsum, PH2AVGCurrentsum;
  static uint32_t PH1AVGCurrentOffset, PH2AVGCurrentOffset;
  static uint16_t vac_pol_changed_counter;
  static uint16_t PH1vac_pol_changed_counter, PH2vac_pol_changed_counter;

  Vout_Control.Vout.FilterCounter++;
  Phase_Values_PH1.Phase_Current.FilterCounter++;
  Phase_Values_PH2.Phase_Current.FilterCounter++;
  //    Phase_Values_PH3.Phase_Current.FilterCounter++;

  VoutAVGsum += Vout_Control.Vout.Raw;

  //  PH1AVGCurrentsum += Phase_Values_PH1.Phase_Current.Rectified;
  //  PH2AVGCurrentsum += Phase_Values_PH2.Phase_Current.Rectified;

  //<<< only testing >>>
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
    Vout_Control.Vout.PreviousValue = Vout_Control.Vout.Filtered; //<< check slop of Vout change
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

  /*  
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
     if (((PH2vac_pol_changed_counter > 1) && (Phase_Values_PH2.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH2.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
     {
       Phase_Values_PH2.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH2AVGCurrentsum, Phase_Values_PH2.Phase_Current.FilterCounter - 1));
       PH2AVGCurrentsum = 0;
       PH2AVGCurrentOffset = 0;
       PH2vac_pol_changed_counter = 0;
       Phase_Values_PH2.Phase_Current.FilterCounter = 0;
     }
   }
   */
  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last = Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed;

}

/*******************************************************************************
 * @fn         ZeroCross_PHx
 * @brief      check for zero cross range PH #X
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ ZeroCross_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  
  //<< switch off main and synch switches at ZC area ---------------------------
  //  if (AC_Ctrl_Flags.bits.PH1_Zero_Cross_Range)
  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range)
  {
    PWM_OverrideHighEnable(PWMnr);
    PWM_OverrideLowEnable(PWMnr);

    //<<< FTx 3-phase noise test
    PWM_H_N_SetLow(); //synch switch OFF
    PWM_L_N_SetLow(); //synch switch OFF

    PhaseX->Controller_Values.CCM_Counter = 1;
    //@ DCM_Counter used as hysteresis for fixed current set DCM
    //@ in MODE_GRID_TIE_INVERTER the DCM counter can be greater CCM level at end of half wave
    //@ at the begin of the next half wave after ZC range synch switch is enabled
    //@ because of DCM counter needs to be count down after ZC range
  }

  //<< polarity change check ---------------------------------------------------
  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity != PhaseX->Control_Status_Flags.bits.VAC_Polarity_last)
  {
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 1;

    PhaseX->Controller_Values.BurstModeCycleCounter++; //<<Burst Mode
    PhaseX->Controller_Values.ZeroCrossCounter++; //<<Count Zero Crosses for dV/dt
  }
  
}

/*******************************************************************************
 * @fn         SoftstartAfterZC_PHx
 * @brief      activate softstart after ZC PH #X
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ SoftstartAfterZC_PHx_GTIMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  
  //<< switch on main switches after polarity change and ZC range is over and before ZC startup 
  if (PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
    PhaseX->Controller_Values.PWM_Counter = 0;

    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        //startup after ZC like PFC
        PWM_SwapEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
        PWM_OverrideHighEnable(PWMnr);

        //startup after ZC GTI
        //////        PWM_SwapDisable(PWMnr);       
        //////        PWM_OverrideHighDisable(PWMnr);
        //GIT LF switch start direct with PWM //////
        //////        PWM_H_N_SetLow();
        //////        PWM_L_N_SetHigh();
      }
    }
    else
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
        //startup after ZC like PFC
        PWM_SwapDisable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
        PWM_OverrideLowEnable(PWMnr);

        //startup after ZC GTI
        //////        PWM_OverrideLowDisable(PWMnr);
        //////        PWM_SwapEnable(PWMnr);
        //GIT LF switch start direct with PWM //////
        //////        PWM_L_N_SetLow();?
        //////        PWM_H_N_SetHigh();
      }
    }
    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 0;
  }

  //<< open loop startup after ZC <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  uint16_t pg1dc_;

  if (PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
  {
    // openloop startup after ZC
    // number of PWM periods: 
    if (++PhaseX->Controller_Values.PWM_Counter < OPEN_LOOP_STARTUP_PULSES)
    {
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      PWM_DutyCycleSet(PWMnr, pg1dc_);
    }
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0; //check startup must be OFF 

      PhaseX->Controller_Values.PWM_Counter = 30; //<<< FTx

      // LF switches are on after open loop startup,
      // LF switch starts up after a few cycles#################
      if (!PhaseX->Controller_Values.Control_Freeze) //added 08.02.24 for Burst Mode
      {
        if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        {
          //startup after ZC GTI
          PWM_SwapDisable(PWMnr);
          PWM_OverrideHighDisable(PWMnr);
          PWM_OverrideLowEnable(PWMnr);

          if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
          {
            PWM_H_N_SetLow();
            PWM_L_N_SetHigh();
          }
        }
        else
        {
          //startup after ZC GTI
          PWM_SwapEnable(PWMnr);
          PWM_OverrideLowDisable(PWMnr);
          PWM_OverrideHighEnable(PWMnr);

          if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
          {
            PWM_L_N_SetLow();
            PWM_H_N_SetHigh();
          }
        }
      }
    }
  }
  //GIT LF switch start after x-cycles in OL like PFC mode //////
  
}

/*******************************************************************************
 * @fn         SoftstartAfterZC_PHx
 * @brief      activate softstart after ZC PH #X
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ SoftstartAfterZC_PHx_PFCMode(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  //<< switch on main switches after polarity change and ZC range is over and before ZC startup 
  if (PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
    PhaseX->Controller_Values.PWM_Counter = 0;

    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
#if defined MODE_PFC  || defined MODE_INTERLEAVED && !defined CHARGEPUMP_ENABLED
        //<<< FTx standard mode PWM enable, comment out for current sink/source option
        PWM_SwapEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
        PWM_OverrideHighEnable(PWMnr); //<<< FTx
#endif
      }
    }
    else
    {
      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
#if defined MODE_PFC || defined MODE_INTERLEAVED && !defined CHARGEPUMP_ENABLED
        PWM_SwapDisable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
        PWM_OverrideLowEnable(PWMnr); //<<< FTx
#endif
      }
    }

    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 0;
  }

  //LF switch start after x-cycles in OL like PFC mode //////
#if defined MODE_PFC || defined MODE_INTERLEAVED 
  //<< open loop startup after ZC
  uint16_t pg1dc_;

  if (PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
  {
    // openloop startup after ZC
    // number of PWM periods: 
    if (++PhaseX->Controller_Values.PWM_Counter < OPEN_LOOP_STARTUP_PULSES)
    {
#ifdef CHARGEPUMP_ENABLED 
      // charge pump I/O handling: 100us on
      // if no charge pump hardware I/O pins are set without acting
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        TP31_RD6_L_N_SetHigh();
      else
        TP33_RD5_H_N_SetHigh();
#else     
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      PWM_DutyCycleSet(PWMnr, pg1dc_);
#endif
    }
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0; //check startup must be OFF 

      PhaseX->Controller_Values.PWM_Counter = 30; //<<< FTx

#if defined CHARGEPUMP_ENABLED  
      // charge pump I/O handling
      TP31_RD6_L_N_SetLow();
      TP33_RD5_H_N_SetLow();

      //switch handling when NOT in light load
      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
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
      // LF switch starts up after a few cycles#################
      if (!PhaseX->Controller_Values.Control_Freeze) //added 08.02.24 for Burst Mode
      {
        if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        {
          if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
          {
            PWM_H_N_SetLow();
            PWM_L_N_SetHigh();
          }
        }
        else
        {
          if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
          {
            PWM_L_N_SetLow();
            PWM_H_N_SetHigh();
          }
        }
      }
#endif   
    }
  }
  //LF switch start after x-cycles in OL like PFC mode //////
#endif
  
}

/*******************************************************************************
 * @fn         CCM_PHx
 * @brief      CCM Mode check PH #X. If DCM run asynchron 
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
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

  // <<< FTx LF switch direct after startup  
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

/*******************************************************************************
 * @fn         DutyCycleFeedForward_PHx
 * @brief      Duty Cycle feedforward calculation
 * @param      none
 * @return     none
 * @details         
 *                  Vout - VAC
 *             d = ------------
 *                      Vout            
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
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
    PhaseX->Controller_Values.Duty_Cycle_Set_Value = PhaseX->Controller_Values.Duty_Cycle_Set_Value + (var1_ui >> 2); //tests done with 2
  }
  
}

/*******************************************************************************
 * @fn         Burst_PHx
 * @brief      check for Burst Mode to skip cycles at light load
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
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
    PWM_H_N_SetLow(); //synch switch OFF
    PWM_L_N_SetLow(); //synch switch OFF
  }

  if ((PhaseX->Controller_Values.BurstModeCycleCounter > 1))
  {
    PhaseX->Control_Status_Flags.bits.BurstModeLatched = 0;
    PhaseX->Controller_Values.Control_Freeze = 0;
    PhaseX->Controller_Values.BurstModeCycleCounter = 0;
  }
  
}

/*******************************************************************************
 * @fn         VOUTavergingFast
 * @brief      Average value calculations
 * @param      none
 * @return     none
 * @details    Averaging of Vout, filter Vout over 512 raw samples 
 *             fast averaging is needed for dV/dt
 *              
 *             
 *******************************************************************************/
/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

static void __inline__ VOUT_HystereticCheck(void)
{
 
  if ((Vout_Control.Vout.Filtered > Vout_Control.Reference.VOUT_dV_dt_Hystresis_High))
  {
    Vout_Control.Reference.VOUT_dV_dt_Active = 1;
    Vout_Control.Reference.VOUT_dV_dt_Up = 1;
    PWM_OverrideHighEnable(1);
    PWM_OverrideLowEnable(1);
    PWM_OverrideHighEnable(2);
    PWM_OverrideLowEnable(2);
    PWM_OverrideHighEnable(3);
    PWM_OverrideLowEnable(3);
    PWM_H_N_SetLow(); //synch switch OFF
    PWM_L_N_SetLow(); //synch switch OFF
  }

  if ((Vout_Control.Vout.Filtered < Vout_Control.Reference.VOUT_Hystresis_High) &&
    (Vout_Control.Vout.Filtered > Vout_Control.Reference.VOUT_Hystresis_Low))
  {
    Vout_Control.Reference.VOUT_dV_dt_Active = 0;
    Vout_Control.Reference.VOUT_dV_dt_Up = 0;
  }
  
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

