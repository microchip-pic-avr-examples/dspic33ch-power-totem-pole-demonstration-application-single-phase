/**
  Generated drv_pwrctrl_app_TPBLPFC.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    drv_pwrctrl_app_TPBLPFC.c

  @Summary
    This is the generated using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
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

static void VOUTaverging(void);
static void VOUT_HystereticCheck(void);

static void ZeroCross_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void SoftstartAfterZC_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void Start_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void CCM_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void DutyCycleFeedForward_PHx(struct PHASE_VALUES_s* PhaseX);
static void ACDrop_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);
static void BurstMode_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);

static void Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr);


//<<< FTx Vout Balancing
int16_t VC1;
int16_t VC2;

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

void PWM_handler_PH123(void)
{

//  _LATB14 = Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity;
//  _LATB14 = Phase_Values_PH1.Controller_Values.Control_Freeze;
  DAC1DATH = Phase_Values_PH1.Controller_Values.IAC_Reference >> 1;

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

#ifdef THREE_PHASE
  //<<<<<<<<<<<  PHASE #2  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Handler_PHx(&Phase_Values_PH2, 2);
#endif 

#if defined SINGLE_PHASE_IL_L1_L3 || defined THREE_PHASE 
  //<<<<<<<<<<<  PHASE #3  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  Handler_PHx(&Phase_Values_PH3, 3);
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

static void __inline__ Handler_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{

  //   CMP1_DACDataWrite(PhaseX->Controller_Values.Duty_Cycle_Set_Value >> 3);
  //   DAC1DATH = (PhaseX->Controller_Values.Duty_Cycle_Set_Value >> 3); 

  if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_OK)
  {
    //<<ZC range detection and handling << polarity change check----------------
    ZeroCross_PHx(PhaseX, PWMnr);

    if (PhaseX->Control_Status_Flags.bits.Relay)
    {

#ifdef AC_CYCLE_SKIPPING_ENABLED 
      BurstMode_PHx(PhaseX, PWMnr);
#endif

      SoftstartAfterZC_PHx(PhaseX, PWMnr); //switch handling when NOT in light load
      //<< open loop softstart after ZC event ----------------------------------
#ifdef GRID_TIED_INVERTER
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0;
#endif
      //<< synch switch handling based on polarity -----------------------------
      if ((!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) && (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range) &&
        ((pwr_ctrl_state > PCS_SOFT_START) || PhaseX->Control_Status_Flags.bits.Reference_Changed))
      {
        if (Vout_Control.Reference.Voltage_Loop_Output > BURST_MODE_VMC_REF_FREEZE)
          CCM_PHx(PhaseX, PWMnr); //comment out for diode mode and DC mode
      }
    }

    //<< Voltage loop-----------------------------------------------------------
#ifdef VOLTAGE_LOOP
    //<<< only testing >>>
    if ((Vout_Control.Vout.FilterCounter < 1) && (PWMnr == 1))
      //    if ((Vout_Control.Vout.FilterCounter < 1) && (PWMnr == 2))
      //    if ((Vout_Control.Vout.FilterCounter < 1) && (PWMnr == 3))
    {
      //<< outer voltage loop
      if (PhaseX->Controller_Values.Control_Freeze) //Freezed ?
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

#ifdef ADAPTIVE_STEPRESPONSE_ENABLED
      //<< dV/dt speed up ?? -----------------------------------------------------------------------------------------------------

      if (Vout_Control.Reference.VOUT_dV_dt_Down)
      {
        Vout_Control.Reference.Voltage_Loop_Output = Vout_Control.Reference.VOUT_dV_dt_VoltageLoopOutputValue; //30000; //<<<<< magic number


        //        PhaseX->Phase_Voltage.Vin_div_Averaged2 = PhaseX->Phase_Voltage.Vin_div_Averaged2_Freezed; 

        if (PhaseX->Controller_Values.ZeroCrossCounter < 1)//Vout_Control.Reference.VOUT_dV_dt_Max_VoltageControllerOutput_Count)
        {
          Start_PHx(PhaseX, PWMnr);
        }

        if (PhaseX->Controller_Values.ZeroCrossCounter >= Vout_Control.Reference.VOUT_dV_dt_Max_VoltageControllerOutput_Count)
        {
          Vout_Control.Reference.VOUT_dV_dt_Down = 0; //?????
        }
      }
#endif
      //---------------------------------------------------------------------------------------------------------------------------

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

#ifdef ADAPTIVE_GAIN_ENABLED 
        //        PHx_AVG_CM2p2z[PWMnr].KfactorCoeffsB = 0x7FFF;//0x55C2;//0x4000;
        PHx_AVG_CM2p2z[PWMnr].KfactorCoeffsB = Adaptive_Gain_Factor;
#endif

#ifdef V2_BALANCER_ENABLED 
        //<<< FTx Vout/2 Balancing
        PhaseX->Controller_Values.IAC_Reference += (Vout_Control.Vout_MidPoint.Offset >> 7);
#endif        

        XFT_SMPS_Controller2P2ZUpdate(&PHx_AVG_CM2p2z[PWMnr], &PhaseX->Phase_Current.Rectified_Shift, PhaseX->Controller_Values.IAC_Reference,
          //        SMPS_Controller2P2ZUpdate(&PHx_AVG_CM2p2z[PWMnr], &PhaseX->Phase_Current.Rectified_Shift, PhaseX->Controller_Values.IAC_Reference,
          &(PhaseX->Controller_Values.Duty_Cycle_Set_Value));
      }


#ifdef DUTY_RATIO_FEEDFORWARD_ENABLED     
      //<< DC ff
      DutyCycleFeedForward_PHx(PhaseX);
#endif

      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value < DCMIN) PhaseX->Controller_Values.Duty_Cycle_Set_Value = 0; //<<<<<FTX DCMIN;
      if (PhaseX->Controller_Values.Duty_Cycle_Set_Value > DCMAX) PhaseX->Controller_Values.Duty_Cycle_Set_Value = DCMAX;

      PWM_DutyCycleSet(PWMnr, PhaseX->Controller_Values.Duty_Cycle_Set_Value);
    }

#ifdef V2_BALANCER_ENABLED 
    //<<< FTx Vout/2 Balancing, Calc
    VC1 = Vout_Control.Vout.Raw - (Vout_Control.Vout_MidPoint.Raw);
    VC2 = Vout_Control.Vout_MidPoint.Raw;
    Vout_Control.Vout_MidPoint.Offset = (VC1 - VC2);
    //    SMPS_ControllerPIDUpdate(&BALANCER_PI, &VC1, VC2, &Vout_Control.Vout_MidPoint.Offset);
#endif

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

static void __inline__ VOUTaverging(void)
{
  static uint32_t VoutAVGsum, PH1AVGCurrentsum, PH2AVGCurrentsum, PH3AVGCurrentsum;
  static uint32_t PH1AVGCurrentOffset, PH2AVGCurrentOffset, PH3AVGCurrentOffset;
  static uint16_t vac_pol_changed_counter;
  static uint16_t PH1vac_pol_changed_counter, PH2vac_pol_changed_counter, PH3vac_pol_changed_counter;

  Vout_Control.Vout.FilterCounter++;
  Phase_Values_PH1.Phase_Current.FilterCounter++;
  Phase_Values_PH2.Phase_Current.FilterCounter++;
  Phase_Values_PH3.Phase_Current.FilterCounter++;

  VoutAVGsum += Vout_Control.Vout.Raw;

  //  PH1AVGCurrentsum += Phase_Values_PH1.Phase_Current.Rectified;
  //  PH2AVGCurrentsum += Phase_Values_PH2.Phase_Current.Rectified;
  //  PH3AVGCurrentsum += Phase_Values_PH3.Phase_Current.Rectified;

  //<<< only testing >>>
  if ((Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed)
    && (!Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
    //  if ((Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed)
    //    && (!Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
    //  if ((Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed)
    //    && (!Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
  {
    vac_pol_changed_counter++;
    PH1vac_pol_changed_counter++;
    //    PH2vac_pol_changed_counter++;
    //    PH3vac_pol_changed_counter++;
  }

  //3PHonly  
#ifdef THREE_PHASE 
  if ((Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed)
    && (!Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
  {
    vac_pol_changed_counter++;
    PH2vac_pol_changed_counter++;
  }

  if ((Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed)
    && (!Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed_last))
  {
    vac_pol_changed_counter++;
    PH3vac_pol_changed_counter++;
  }
#endif  

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
    PH3AVGCurrentOffset += Phase_Values_PH3.Phase_Current.Raw;

    //28_07_2022 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    if (((PH1vac_pol_changed_counter > 1) && (Phase_Values_PH1.Phase_Current.FilterCounter > 1))
      || (Phase_Values_PH1.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      //    Phase_Values_PH1.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH1AVGCurrentsum, Phase_Values_PH1.Phase_Current.FilterCounter - 1));
      Phase_Values_PH1.Phase_Current.Offset = (uint16_t) (__builtin_divud(PH1AVGCurrentOffset, Phase_Values_PH1.Phase_Current.FilterCounter - 1));
      PH1AVGCurrentsum = 0;
      PH1AVGCurrentOffset = 0;
      PH1vac_pol_changed_counter = 0;
      Phase_Values_PH1.Phase_Current.FilterCounter = 0;
    }
    if (((PH2vac_pol_changed_counter > 1) && (Phase_Values_PH2.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH2.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      //    Phase_Values_PH2.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH2AVGCurrentsum, Phase_Values_PH2.Phase_Current.FilterCounter - 1));
      Phase_Values_PH2.Phase_Current.Offset = (uint16_t) (__builtin_divud(PH2AVGCurrentOffset, Phase_Values_PH2.Phase_Current.FilterCounter - 1));
      PH2AVGCurrentsum = 0;
      PH2AVGCurrentOffset = 0;
      PH2vac_pol_changed_counter = 0;
      Phase_Values_PH2.Phase_Current.FilterCounter = 0;
    }
    if (((PH3vac_pol_changed_counter > 1) && (Phase_Values_PH3.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH3.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
    {
      //    Phase_Values_PH3.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH3AVGCurrentsum, Phase_Values_PH3.Phase_Current.FilterCounter - 1));
      Phase_Values_PH3.Phase_Current.Offset = (uint16_t) (__builtin_divud(PH3AVGCurrentOffset, Phase_Values_PH3.Phase_Current.FilterCounter - 1));
      PH3AVGCurrentsum = 0;
      PH3AVGCurrentOffset = 0;
      PH3vac_pol_changed_counter = 0;
      Phase_Values_PH3.Phase_Current.FilterCounter = 0;
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
      if (((PH3vac_pol_changed_counter > 1) && (Phase_Values_PH3.Phase_Current.FilterCounter > 1)) || (Phase_Values_PH3.Phase_Current.FilterCounter > 4000)) //4000=40ms/10us
      {
        Phase_Values_PH3.Phase_Current.Filtered = (uint16_t) (__builtin_divud(PH3AVGCurrentsum, Phase_Values_PH3.Phase_Current.FilterCounter - 1));
        PH3AVGCurrentsum = 0;
        PH3AVGCurrentOffset = 0;
        PH3vac_pol_changed_counter = 0;
        Phase_Values_PH3.Phase_Current.FilterCounter = 0;
      }
    }
   */

  Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed_last = Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed;
  Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed_last = Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed;
  Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed_last = Phase_Values_PH3.Control_Status_Flags.bits.VAC_Polarity_Changed;

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
    //@ in GRID_TIED_INVERTER the DCM counter can be greater CCM level at end of half wave
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

static void __inline__ SoftstartAfterZC_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  //<< switch on main switches after polarity change and ZC range is over and before ZC startup 
  if (PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed &&
    (!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    
    
//////    PWM_DutyCycleSet(PWMnr, 0);
    
    
    
    
    
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
      PhaseX->Controller_Values.PWM_Counter = 0;
      
      
      
//////PG1CONHbits.UPDMOD = 0b001;
//////PWM_DutyCycleSet(PWMnr, 0);
//////PG1STATbits.UPDREQ = 1;
//////while(PG1STATbits.UPDREQ);




//////      //switch handling when NOT in light load
//////      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
//////      {
//////#ifndef GRID_TIED_INVERTER
//////        //#ifdef THREE_PHASE
//////#ifndef CHARGEPUMP_ENABLED
//////        //<<< FTx standard mode PWM enable, comment out for current sink/source option
//////        PWM_SwapEnable(PWMnr);
//////        PWM_OverrideLowDisable(PWMnr);
//////        PWM_OverrideHighEnable(PWMnr); //<<< FTx
//////#endif
//////        //#endif
//////#else
//////        //////        PWM_OverrideLowEnable(PWMnr);
//////        PWM_OverrideHighDisable(PWMnr);
//////        PWM_SwapDisable(PWMnr);
//////#ifdef SINGLE_PHASE_L1
//////        PWM_H_N_SetLow();
//////        while (_LATD1 == 1); //@??? does that help to prevent race conditions ?
//////        PWM_L_N_SetHigh();
//////        while (_LATD0 == 0); //synch switch ON
//////#endif
//////#endif
//////      }
       
    }
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 1;
      PhaseX->Controller_Values.PWM_Counter = 0;

//////      //switch handling when NOT in light load
//////      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
//////      {
//////#ifndef GRID_TIED_INVERTER   
//////  #ifndef CHARGEPUMP_ENABLED
//////        PWM_SwapDisable(PWMnr);
//////        PWM_OverrideHighDisable(PWMnr);
//////        PWM_OverrideLowEnable(PWMnr); //<<< FTx
//////  #endif
//////#else   
//////        //////        PWM_OverrideHighEnable(PWMnr);
//////        PWM_OverrideLowDisable(PWMnr);
//////        PWM_SwapEnable(PWMnr);
//////#ifdef SINGLE_PHASE_L1
//////        PWM_L_N_SetLow();
//////        while (_LATD0 == 1); //@??? does that help to prevent race conditions ?
//////        PWM_H_N_SetHigh();
//////        while (_LATD1 == 0); //synch switch ON
//////#endif
//////#endif
//////      }
    }

    PhaseX->Control_Status_Flags.bits.VAC_Polarity_Changed = 0;
  }

#ifndef GRID_TIED_INVERTER  
  //<< open loop startup after ZC
  uint16_t pg1dc_;

  if (PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross)
  {

#ifdef THREE_PHASE
    if (++PhaseX->Controller_Values.PWM_Counter < OPEN_LOOP_STARTUP_PULSES)
    {
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      if (pg1dc_ > PhaseX->Controller_Values.Duty_Cycle_Set_Value)
      {
        PhaseX->Controller_Values.PWM_Counter = 30;
      }
      PWM_DutyCycleSet(PWMnr, pg1dc_);
    }

#else
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
      // duty cycle fixed set to 50%
      pg1dc_ = (MPER >> 1);
      // duty cycle increase to compensator value
      pg1dc_ = (PhaseX->Controller_Values.PWM_Counter << OPEN_LOOP_PWM_COUNTER_SHIFT);
      if (pg1dc_ > PhaseX->Controller_Values.Duty_Cycle_Set_Value)
      {
        PhaseX->Controller_Values.PWM_Counter = 30;
      }
      PWM_DutyCycleSet(PWMnr, pg1dc_);
      
      
      
      
      
      
      //31.1.24
      if (PhaseX->Controller_Values.PWM_Counter == 1)
      {
        TP35_RB14_SetHigh();

        if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
        {
          PWM_SwapEnable(PWMnr);
          PWM_OverrideLowDisable(PWMnr);
          PWM_OverrideHighEnable(PWMnr); //<<< FTx
        }
        else
        {
          PWM_SwapDisable(PWMnr);
          PWM_OverrideHighDisable(PWMnr);
          PWM_OverrideLowEnable(PWMnr); //<<< FTx
        }
        PG1CONHbits.UPDMOD = 0b000;
        TP35_RB14_SetLow();
      }
      
      
      
      
      
      
      
      
      
      
      
#endif
    }
#endif
    else
    {
      PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross = 0; //check startup must be OFF 

      PhaseX->Controller_Values.PWM_Counter = 30; //<<< FTx

#if defined SINGLE_PHASE_L1  
      // charge pump I/O handling
      TP31_RD6_L_N_SetLow();
      TP33_RD5_H_N_SetLow();

//////      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
//////      {
//////        PWM_SwapEnable(PWMnr);
//////        PWM_OverrideLowDisable(PWMnr);
//////        PWM_OverrideHighEnable(PWMnr);
//////      }
//////      else
//////      {
//////        PWM_SwapDisable(PWMnr);
//////        PWM_OverrideHighDisable(PWMnr);
//////        PWM_OverrideLowEnable(PWMnr);
//////      }
#endif

      // LF switches are on after open loop startup,
#if defined SINGLE_PHASE_L1  
      // LF switch starts up after a few cycles#################
      if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
      {
        if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
        {
          PWM_H_N_SetLow();
          while (_LATD1 == 1); //@??? does that help to prevent race conditions ?
          PWM_L_N_SetHigh();
          while (_LATD0 == 0); //synch switch ON
        }
      }
      else
      {
        if (pwr_ctrl_state > PCS_START_CONTROL) //== PCS_UP_AND_RUNNING)
        {
          PWM_L_N_SetLow();
          while (_LATD0 == 1); //@??? does that help to prevent race conditions ?
          PWM_H_N_SetHigh();
          while (_LATD1 == 0); //synch switch ON
        }
      }
#endif   

    }
  }
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
#ifdef RECTIFIER_SWITCH_ENABLED
  if (PhaseX->Control_Status_Flags.bits.CCM)
  {
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
#ifndef GRID_TIED_INVERTER        
      PWM_OverrideHighDisable(PWMnr); //PWM ON
#if defined RECTIFIER_SWITCH_ENABLED && defined SINGLE_PHASE_L1
      //////        PWM_L_N_SetHigh();
#endif
#else
      PWM_OverrideLowDisable(PWMnr); //PWM ON
#endif        
    }
    else
    {
#ifndef GRID_TIED_INVERTER     
      PWM_OverrideLowDisable(PWMnr); //PWM ON
#if defined RECTIFIER_SWITCH_ENABLED && defined SINGLE_PHASE_L1
      //////        PWM_H_N_SetHigh();
#endif
#else
      PWM_OverrideHighDisable(PWMnr); //PWM ON
#endif        
    }
  }
  else
  {
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
#ifndef GRID_TIED_INVERTER     
      PWM_OverrideHighEnable(PWMnr); //PWM OFF
#if defined RECTIFIER_SWITCH_ENABLED && defined SINGLE_PHASE_L1      
      //////        PWM_L_N_SetLow();
#endif
#else
      PWM_OverrideLowEnable(PWMnr); //PWM OFF
      //    MPER = 18000;
#endif        
    }
    else
    {
#ifndef GRID_TIED_INVERTER     
      PWM_OverrideLowEnable(PWMnr); //PWM OFF
#if defined RECTIFIER_SWITCH_ENABLED && defined SINGLE_PHASE_L1
      //////        PWM_H_N_SetLow();
#endif
#else
      PWM_OverrideHighEnable(PWMnr); //PWM OFF
      //    MPER = 18000;
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

static void __inline__ DutyCycleFeedForward_PHx(struct PHASE_VALUES_s * PhaseX)
{
  uint32_t var_ul;
  uint16_t var1_ui;

  if (Vout_Control.Vout.Filtered > PhaseX->Phase_Voltage.Rectified)
  {
    var1_ui = Vout_Control.Vout.Filtered - PhaseX->Phase_Voltage.Rectified; //Vout - Vin
    var_ul = (uint32_t) var1_ui * MPER;
    var1_ui = __builtin_divud(var_ul, Vout_Control.Vout.Filtered);

    ////    Buck / Inverter Mode
    //    var1_ui = Vout_Control.Vout.Filtered; //Vout - Vin
    //    var_ul = (uint32_t) PhaseX->Phase_Voltage.Rectified * MPER;
    //    var1_ui = __builtin_divud(var_ul, var1_ui);

#ifdef THREE_PHASE  
    PhaseX->Controller_Values.Duty_Cycle_Set_Value = PhaseX->Controller_Values.Duty_Cycle_Set_Value + (var1_ui >> 4); //3
#else
    PhaseX->Controller_Values.Duty_Cycle_Set_Value = PhaseX->Controller_Values.Duty_Cycle_Set_Value + (var1_ui >> 2); //tests done with 2
#endif 
  }
}

/*******************************************************************************
 * @fn         ACDrop_PHx
 * @brief      check for AC drop NOT USED (for later investigations)
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/

static void __inline__ ACDrop_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  //<< AC drop check ---------------------------------------------------
  if ((PhaseX->Phase_Voltage.Rectified & 0x0800) == 0x0800)
  {
    //    AC_Ctrl_Flags.bits.PH1_VACdrop = 1;
  }
  else
  {
    //    AC_Ctrl_Flags.bits.PH1_VACdrop = 0;
  }

  //  if (AC_Ctrl_Flags.bits.PH1_VACdrop)
  {
    PWM_OverrideHighEnable(PWMnr);
    PWM_OverrideLowEnable(PWMnr);
    PWM_H_N_SetLow(); //synch switch OFF
    PWM_L_N_SetLow(); //synch switch OFF

    PhaseX->Controller_Values.CCM_Counter = 1;
    //@ DCM_Counter used as hysteresis for fixed current set DCM
    //@ in GRID_TIED_INVERTER the DCM counter can be greater CCM level at end of half wave
    //@ at the begin of the next half wave after ZC range synch switch is enabled
    //@ because of DCM counter needs to be count down after ZC range
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
//////    PhaseX->Controller_Values.Control_Freeze = 1;
    PhaseX->Controller_Values.BurstModeCycleCounter = 0;
    PhaseX->Controller_Values.BurstModeCyclesOn = 0;

//    TP35_RB14_SetHigh();
    
    PWM_OverrideHighEnable(PWMnr);
    PWM_OverrideLowEnable(PWMnr);
    PWM_H_N_SetLow(); //synch switch OFF
    PWM_L_N_SetLow(); //synch switch OFF
  }

  if ((PhaseX->Controller_Values.BurstModeCycleCounter > 1) )//PhaseX->Controller_Values.BurstModeCyclesOff)
    //|| (Vout_Control.Reference.VOUT_dV_dt_Down))
  {
    PhaseX->Control_Status_Flags.bits.BurstModeLatched = 0;
    PhaseX->Controller_Values.Control_Freeze = 0;
    PhaseX->Controller_Values.BurstModeCycleCounter = 0;
    
//    TP35_RB14_SetLow();

  }
}

/*******************************************************************************
 * @fn         Start_PHx
 * @brief      activate PWM to start immediately when dV/dt detected PH #X
 * @param      none
 * @return     none
 * @details    
 *             
 *              
 *             
 *******************************************************************************/

static void __inline__ Start_PHx(struct PHASE_VALUES_s* PhaseX, uint16_t PWMnr)
{
  //<< switch on main switches after polarity change and ZC range is over and before ZC startup 
  if ((!PhaseX->Control_Status_Flags.bits.Soft_Start_Zero_Cross) &&
    (!PhaseX->ACcontrol_Status_Flags_perPhase.bits.Zero_Cross_Range))
  {
    if (PhaseX->ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity)
    {
      //switch handling when NOT in light load
      //      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
#ifndef GRID_TIED_INVERTER
        PWM_SwapEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
#else
        PWM_OverrideLowEnable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
        PWM_SwapDisable(PWMnr);
#ifdef SINGLE_PHASE_L1
        PWM_H_N_SetLow();
        while (_LATD1 == 1); //@??? does that help to prevent race conditions ?
        PWM_L_N_SetHigh();
        while (_LATD0 == 0); //synch switch ON
#endif
#endif
      }
    }
    else
    {
      //switch handling when NOT in light load
      //      if ((!PhaseX->Control_Status_Flags.bits.BurstModeLatched))// && (!Vout_Control.Reference.VOUT_dV_dt_Active))
      {
#ifndef GRID_TIED_INVERTER   
        PWM_SwapDisable(PWMnr);
        PWM_OverrideHighDisable(PWMnr);
#else   
        PWM_OverrideHighEnable(PWMnr);
        PWM_OverrideLowDisable(PWMnr);
        PWM_SwapEnable(PWMnr);
#ifdef SINGLE_PHASE_L1
        PWM_L_N_SetLow();
        while (_LATD0 == 1); //@??? does that help to prevent race conditions ?
        PWM_H_N_SetHigh();
        while (_LATD1 == 0); //synch switch ON
#endif
#endif
      }
    }
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

static void __inline__ VOUT_HystereticCheck(void)
{
  uint16_t zw_;

  /*  <<commented out so dV/dt only works on load OFF
   * 
    //<< dV/dt check on VOUT only influence only out of Burst mode
    if ((Vout_Control.Vout.Filtered < Vout_Control.Reference.VOUT_dV_dt_Hystresis_Low) &&
      (!Vout_Control.Reference.VOUT_dV_dt_Active))
      //&&(Vout_Control.Reference.Voltage_Loop_Output > BURST_MODE_LEVEL_HIGH)
    {
      //    Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2_Freezed = Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2;
      //    Phase_Values_PH2.Phase_Voltage.Vin_div_Averaged2_Freezed = Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2;
      //    Phase_Values_PH3.Phase_Voltage.Vin_div_Averaged2_Freezed = Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2;
      Phase_Values_PH1.Controller_Values.ZeroCrossCounter = 0;
      Phase_Values_PH2.Controller_Values.ZeroCrossCounter = 0;
      Phase_Values_PH3.Controller_Values.ZeroCrossCounter = 0;

      Vout_Control.Reference.VOUT_dV_dt_Active = 1;
      Vout_Control.Reference.VOUT_dV_dt_Down = 1;

      zw_ = (abs(Vout_Control.Vout.Filtered - Vout_Control.Vout.PreviousValue));

      if (zw_ < VOUT_HYSTERETIC_RAMP_VALUE_LOW)
      {
        Vout_Control.Reference.VOUT_dV_dt_Max_VoltageControllerOutput_Count = VOUT_HYSTERETIC_VMC_REF_MAX_OFF_CYCLES_LOW; //75=2V/10ms
        Vout_Control.Reference.VOUT_dV_dt_VoltageLoopOutputValue = 10000;
      }
      if (zw_ > VOUT_HYSTERETIC_RAMP_VALUE_MIDDLE)
      {
        Vout_Control.Reference.VOUT_dV_dt_Max_VoltageControllerOutput_Count = VOUT_HYSTERETIC_VMC_REF_MAX_OFF_CYCLES_MIDDLE; //125=3.35V/10ms 
        Vout_Control.Reference.VOUT_dV_dt_VoltageLoopOutputValue = 20000;
      }

      if (zw_ > VOUT_HYSTERETIC_RAMP_VALUE_HIGH)
      {
        Vout_Control.Reference.VOUT_dV_dt_Max_VoltageControllerOutput_Count = VOUT_HYSTERETIC_VMC_REF_MAX_OFF_CYCLES_HIGH;
        Vout_Control.Reference.VOUT_dV_dt_VoltageLoopOutputValue = 30000;
      }

    }
    else*/ if ((Vout_Control.Vout.Filtered > Vout_Control.Reference.VOUT_dV_dt_Hystresis_High))
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
    Vout_Control.Reference.VOUT_dV_dt_Down = 0;
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

