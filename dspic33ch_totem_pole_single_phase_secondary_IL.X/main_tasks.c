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

#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/system/interrupt.h"

#include "sources/driver/msi/drv_msi.h"
#include "sources/devices/dev_TPBLPFC_typedef.h"
#include "sources/driver/power_controller/drv_pwrctrl_app_misc_TPBLPFC.h"
#include "sources/misc/MCC_add_on.h"

#include "PFC_frameworkSetup.h"
#include "sources/driver/power_controller/drv_controller_TPBLPFC.h"

/*******************************************************************************
 * @details static functions in main_task.c    
 *          functions are performance optimized inlined        
 *******************************************************************************/

static PWR_CTRL_STATE_e Init_StateHandler(void);
static PWR_CTRL_STATE_e WaitIfFaultActive_StateHandler(void);
static PWR_CTRL_STATE_e StandBy_StateHandler(void);
static PWR_CTRL_STATE_e VacOk_StateHandler(void);
static PWR_CTRL_STATE_e VoutPreLoad_StateHandler(void);
static PWR_CTRL_STATE_e RelayOn_StateHandler(void);
static PWR_CTRL_STATE_e DelayAfterRelayOn_StateHandler(void);
static PWR_CTRL_STATE_e StartControl_StateHandler(void);
static PWR_CTRL_STATE_e SoftStart_StateHandler(void);
static PWR_CTRL_STATE_e UpAndRunning_StateHandler(void);
static PWR_CTRL_STATE_e VACOK_Check(void);

/***********************************************************************************
 * @ingroup main_task
 * @fn      main_task
 * @brief   definitions for timing and startup
 * @details 
 *          time definitions based on 100us 
 *          startup definitions for ramp accuracy
 *          startup definitions for calibration accuracy
 **********************************************************************************/

#define t2sec 20000 //100us steps for 2s
#define t1sec 10000 //100us steps for 1s
#define t20ms 200   //100us steps for 20ms
#define t50ms 500   //100us steps for 30ms
#define VoutStartUpVary 150 //Startup ramp accuracy range to ramp further
#define CurrentOffsetLowLimit 2020  //typ. ADC value is 2048
#define CurrentOffsetHighLimit 2070 //typ. ADC value is 2048

/*******************************************************************************
 * @details static variable   
 *                
 *******************************************************************************/

static uint16_t RelayTimeCounter = t2sec, RelayTimeCounterStart = t20ms;
static uint16_t CalibrationTimeCounter = t2sec;

/***********************************************************************************
 * @ingroup main_task
 * @fn      Tasks_100us
 * @param   void
 * @return  void
 * @brief   powercontroller statemachine
 * @details 
 *          100us easy performance optimized scheduler
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

void Tasks_100us(void)
{
  ReceiveMSIAMessage(); //<< check CAN receive

  //<< send data via CAN 
  if ((SI1FIFOCSbits.SWFEN) && (SI1FIFOCSbits.SWFEMPTY))
  {
    SendMSIFIFOMessage();
  }
  //<<----------------------------------------------------------------------

  switch (pwr_ctrl_state)
  {
      //<<--------------------------------------------------------------------
    case PCS_INIT://#1
      //wait for VACok and after 2sec check if calibration data are valid
      pwr_ctrl_state = Init_StateHandler();

      break;

      //<<--------------------------------------------------------------------
    case PCS_WAIT_IF_FAULT_ACTIVE://#2
      //stall when current sensor calibration fails
      Nop();

      break;

      //<<--------------------------------------------------------------------
    case PCS_STANDBY://#3
      pwr_ctrl_state = StandBy_StateHandler();

      break;

      //<<--------------------------------------------------------------------
    case PCS_VACOK://#4
      pwr_ctrl_state = VacOk_StateHandler();

      break;

      //<<--------------------------------------------------------------------
    case PCS_VOUT_PRELOAD://#5 
      pwr_ctrl_state = VoutPreLoad_StateHandler();

      break;

      //<<--------------------------------------------------------------------
    case PCS_RELAYON://#6
      pwr_ctrl_state = RelayOn_StateHandler();

      break;

      //<<--------------------------------------------------------------------
    case PCS_DELAY_AFTER_RELAYON://#7

      // wait 50ms delay after switch on to enable relay bits for Phase Handler
      pwr_ctrl_state = DelayAfterRelayOn_StateHandler();
    
      break;

      //<<--------------------------------------------------------------------
    case PCS_START_CONTROL://#8
      // wait 50ms delay to initialize controller and Vout softstart reference based
      // on the actual Vout measurement 
      pwr_ctrl_state = StartControl_StateHandler();

#ifndef VOLTAGE_LOOP
      Vout_Control.Reference.Reference_Internal = 0;
#endif
      
      break;

      //<<--------------------------------------------------------------------
    case PCS_SOFT_START://#9

      pwr_ctrl_state = SoftStart_StateHandler();

#ifdef VOLTAGE_LOOP
      //ramp further if actual Vout is in a certain range
      if ((Vout_Control.Vout.Raw > (Vout_Control.Reference.Reference_Internal - VoutStartUpVary)) &&
        (Vout_Control.Vout.Raw < (Vout_Control.Reference.Reference_Internal + VoutStartUpVary)))
#endif
      {
        pwr_ctrl_state = Softstart_reference();
      }
      
      break;

      //<<--------------------------------------------------------------------
    case PCS_UP_AND_RUNNING://#10

      pwr_ctrl_state = UpAndRunning_StateHandler();
      
      break;

      //<<--------------------------------------------------------------------
    case PCS_STALL_DEBUG://#11
      //stall when VACOK is false or anything goes completely wrong
      //for debug purposes
      Nop();

      break;

      //<<--------------------------------------------------------------------
    default:

      break;
  }
  //<<----------------------------------------------------------------------

  //fast AC loss check
  pwr_ctrl_state = VACOK_Check();

}

/***********************************************************************************
 * @ingroup main_task
 * @fn      Init_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   VACOK and current sensor calibration
 * @details 
 *          wait on VACOK 
 *          and check current sensor AD value at 0A to calibrate the Zero value
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e Init_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  //<<< only testing >>>
  if (Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_OK)
  {
    if ((CalibrationTimeCounter--) == 0)
    {
      //<<< only testing >>>
      if ((Phase_Values_PH1.Phase_Current.Offset > CurrentOffsetLowLimit) || (Phase_Values_PH1.Phase_Current.Offset < CurrentOffsetHighLimit))
      {
        returnvalue = PCS_STANDBY;
      }
      else
        returnvalue = PCS_WAIT_IF_FAULT_ACTIVE;
    }
  }
  else
    returnvalue = PCS_INIT;

  return returnvalue;

}

/***********************************************************************************
 * @ingroup main_task
 * @fn      WaitIfFaultActive_StateHandler
 * @param   void
 * @return  WR_CTRL_STATE_e
 * @brief   NOT USED / PLACEHOLDER
 * @details 
 *          in this application that state is only a place holder
 *          faults are handled in ISR direct and staus flag is set
 *          
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e WaitIfFaultActive_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  returnvalue = pwr_ctrl_state;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      StandBy_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   standby state wait on Run command 
 * @details 
 *          wait on Run from CAN, then start application
 *          if AUTO_START option is defined then application starts immediately
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e StandBy_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

#ifdef AUTO_START
  if ((Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_OK)) //testsetup to startPFC automatic without CAN GUI
  {
    Vout_Control.Reference.Reference_Set = VOUT_REF;
#else          
  if (PFC_Flags.bits.Run)
  {
#endif
    PFC_Flags.bits.Run = 1;
    PFC_Flags.bits.Fault = 0;
    PFC_Flags.bits.OC_PH1 = 0;
    PFC_Flags.bits.OC_PH2 = 0;
    PFC_Flags.bits.OC_PH3 = 0;
    PFC_Flags.bits.Stop = 0;
    PFC_Flags.bits.OV_Vout = 0;

    Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_last = Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity = 0;
    Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_last = Phase_Values_PH2.ACcontrol_Status_Flags_perPhase.bits.VAC_Polarity = 0;

    RelayTimeCounter = t1sec;
    returnvalue = PCS_VACOK;
  }
  else
    returnvalue = PCS_STANDBY;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      VacOk_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   delay before relay on
 * @details 
 *          1 sec. delay/wait to insure input is stable before relay on
 *          VACOK from VAC input is checked in init state
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e VacOk_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if ((RelayTimeCounter--) == 0)
  {
#ifdef  VOUT_PRECHARGE_PCS_ENABLE
    returnvalue = PCS_VOUT_PRELOAD;
#else
    returnvalue = PCS_RELAYON;
#endif
  }
  else
    returnvalue = PCS_VACOK;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      VoutPreLoad_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   delay before relay on
 * @details 
 *          Charge Vout with fixed open loop duty cycle before start control loops
 *          Needed to prevent startup over current shoots because of greater
 *          difference Vin_peak greater Vout
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e VoutPreLoad_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (!PFC_Flags.bits.Run)
  {
    Drv_PwrCtrl_TPBLPFC_Stop();
    returnvalue = PCS_STANDBY;
  }
  else
  {
    if (Vout_Control.Vout.Filtered > VOUT_PRECHARGE_VALUE)
    {
      returnvalue = PCS_RELAYON;
    }
    else
      returnvalue = PCS_VOUT_PRELOAD;
  }
  
  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      RelayOn_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   relay on
 * @details 
 *          50ms delay/wait
 *          relay is switched on
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e RelayOn_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  RELAY_SetHigh(); //Relay ON
  RelayTimeCounter = t20ms;
  returnvalue = PCS_DELAY_AFTER_RELAYON;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      DelayAfterRelayOn_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   delay/Wait after relay on
 * @details 
 *          wait 50ms after relay on to be out of relay bouncing
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e DelayAfterRelayOn_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (((RelayTimeCounter--) == 0) && (!Phase_Values_PH1.Control_Status_Flags.bits.Relay))
  {
    Phase_Values_PH1.Control_Status_Flags.bits.Relay = 1;
    Phase_Values_PH2.Control_Status_Flags.bits.Relay = 1;
    RelayTimeCounterStart = 1;  //xx t50ms;

    returnvalue = PCS_START_CONTROL;
  }
  else
    returnvalue = PCS_DELAY_AFTER_RELAYON;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      StartControl_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   initialize control loops
 * @details 
 *          wait 50ms delay to initialize controller and Vout softstart reference based
 *          on the actual Vout measurement 
 *          delay is needed to be sure that Vout is ramped up after PWM started switching
 *          20ms before
 *          during softstart Vout is checked if value is in the range of reference
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e StartControl_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (((RelayTimeCounterStart--) == 0) && (Phase_Values_PH1.Control_Status_Flags.bits.Relay))
  {
    INTERRUPT_GlobalDisable();
    Phase_Values_PH1.Controller_Values.CCM_Counter = 1;
    Phase_Values_PH1.Control_Status_Flags.bits.CCM = 0;
    Phase_Values_PH2.Controller_Values.CCM_Counter = 1;
    Phase_Values_PH2.Control_Status_Flags.bits.CCM = 0;

    SMPS_Controller2P2ZInitialize(&PHx_AVG_CM2p2z[1]); // Clear histories
    SMPS_Controller2P2ZInitialize(&PHx_AVG_CM2p2z[2]); // Clear histories
    SMPS_Controller2P2ZInitialize(&VMC_2p2z); // Clear histories

    Vout_Control.Reference.Reference_Internal = Vout_Control.Vout.Filtered;

    INTERRUPT_GlobalEnable();

#ifdef MODE_GRID_TIE_INVERTER              
    //<< initiate change bit to enable PWM in SoftstartAfterZC_PH1 for DC mode
    Phase_Values_PH1.Control_Status_Flags.bits.VAC_Polarity_Changed = 1;
    Phase_Values_PH2.Control_Status_Flags.bits.VAC_Polarity_Changed = 1;
#endif              
    returnvalue = PCS_SOFT_START;
  }
  else
    returnvalue = PCS_START_CONTROL;

  return returnvalue;

}

/***********************************************************************************
 * @ingroup main_task
 * @fn      SoftStart_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   check run bit during soft start
 * @details 
 *          check run bit and switch off if demanded
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e SoftStart_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (!PFC_Flags.bits.Run)
  {
    Phase_Values_PH1.Control_Status_Flags.bits.Relay = 0;
    Phase_Values_PH2.Control_Status_Flags.bits.Relay = 0;
    Drv_PwrCtrl_TPBLPFC_Stop();
    RELAY_SetLow(); //Relay OFF

    returnvalue = PCS_STANDBY;
  }
  else
    returnvalue = pwr_ctrl_state;

  return returnvalue;

}

/***********************************************************************************
 * @ingroup main_task
 * @fn      UpAndRunning_StateHandler
 * @param   void
 * @return  PWR_CTRL_STATE_e
 * @brief   up and running state
 * @details 
 *          checks during normal run if application is forced to switch off
 *          checks for reference change and starts new ramp to the new ref
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e UpAndRunning_StateHandler(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (!PFC_Flags.bits.Run)
  {
    Phase_Values_PH1.Control_Status_Flags.bits.Relay = 0;
    Phase_Values_PH2.Control_Status_Flags.bits.Relay = 0;
    Drv_PwrCtrl_TPBLPFC_Stop();
    RELAY_SetLow(); //Relay OFF

    returnvalue = PCS_STANDBY;
  }
  else if ((Vout_Control.Reference.Reference_Set > (Vout_Control.Reference.Reference_Internal + 20)) ||
    (Vout_Control.Reference.Reference_Set < (Vout_Control.Reference.Reference_Internal - 20)))
  {
    Phase_Values_PH1.Control_Status_Flags.bits.Reference_Changed = 1;
    returnvalue = PCS_SOFT_START;
  }
  else
    returnvalue = PCS_UP_AND_RUNNING;

  return returnvalue;
}

/***********************************************************************************
 * @ingroup main_task
 * @fn      VACOK_Check
 * @param   PWR_CTRL_STATE_e
 * @return  void
 * @brief   check VACok flag(s) in fast while loop
 * @details 
 *          check for VACok flag all the time outside the 100us scheduler task
 *          to detect any AC loss as fast as possible
 *          if AC loss ever everything is switched off and stall until POR
 **********************************************************************************/

static __inline__ PWR_CTRL_STATE_e VACOK_Check(void)
{
  static PWR_CTRL_STATE_e returnvalue = 1;

  if (pwr_ctrl_state > PCS_VACOK)
  {
    if ((!Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.bits.VAC_OK))
    {
      RELAY_SetLow(); //Relay OFF
      Drv_PwrCtrl_TPBLPFC_Stop();
      returnvalue = PCS_STALL_DEBUG;
      PFC_Flags.bits.Run = 0;
      PFC_Flags.bits.Stop = 1;
    }
    else
      returnvalue = pwr_ctrl_state;
  }
  else
    returnvalue = pwr_ctrl_state;

  return returnvalue;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

