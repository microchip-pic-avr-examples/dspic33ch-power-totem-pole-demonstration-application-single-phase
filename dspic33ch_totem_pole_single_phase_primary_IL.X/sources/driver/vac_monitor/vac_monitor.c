/**
 * VAC Driver Source File
 * 
 * @file      vac_monitor.c
 * 
 * @ingroup   pri-layer-VAC
 * 
 * @brief     This is the driver source file for vac driver
*/

/*
    (c) 2022 Microchip Technology Inc. and its subsidiaries. You may use this
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

#include <stdint.h>
#include <xc.h>
#include "vac_monitor.h"
#include "vac_monitor_typedef.h"

//------------------------------------------------------------------------------
// #defines for thresholds 
//------------------------------------------------------------------------------
#define VACM_DC_ACCEPTANCE_THRESHOLD        (200)
#define VACM_DC_REJECTION_THRESHOLD         (100)
#define VACM_AC_DROP_VOLTAGE_THRESHOLD      (100)
#define VACM_AC_DROP_PERSISTANCE_COUNT      (2)
#define VACM_POLARITY_HYSTERESIS            (10)
#define VCAM_ZERO_CROSS_THRESHOLD           (50)
#define VCAM_STARTUP_HALF_GRID_CYCLE_COUNT  (20)

//------------------------------------------------------------------------------
// private function declarations
//------------------------------------------------------------------------------
void __attribute__((always_inline)) vacm_state_standby(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_dc_detect(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_dc_mode(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_wait_for_zc(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_ac_sync(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_online(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_state_ac_drop(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_vin_average(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_polarity_update(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_zerocross_detect(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_vloop_feedforward_update(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_acok_assert(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_capture_input(struct VACM_s* vacm_obj);

void __attribute__((always_inline)) vacm_timebase_update(struct VACM_s* vacm_obj);
bool __attribute__((always_inline)) vacm_dc_input_accept(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_uv_ov_test(struct VACM_s* vacm_obj);
void __attribute__((always_inline)) vacm_reset_state_machine(struct VACM_s* vacm_obj);

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_reset_phase_monitor_object()
 * @brief clear appropriate members of phase monitor object
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * called before going to state VACM_STATE_STANDBY
 *********************************************************************************/
void vacm_reset_phase_monitor_object(struct VACM_s* vacm_obj)
{
  vacm_obj->avgcalc_buffer.acc = 0;
  vacm_obj->avgcalc_buffer.counter = 0;
  vacm_obj->vin.avg = 0;
  vacm_obj->status.value = 0; // clear all flags
  vacm_obj->timebase.ac_drop_timer = 0;
  vacm_obj->timebase.half_cycle_timer = 0;
  vacm_obj->timebase.startup_counter = 0;
  vacm_obj->timebase.vin_present_counter = 0;
  vacm_obj->timebase.zero_cross_timer = 0;
  vacm_obj->timebase.dc_loss_counter = 0;
  vacm_obj->timebase.dc_present_counter = 0;
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_reset_state_machine()
 * @brief reset state variable
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * 
 *********************************************************************************/
void vacm_reset_state_machine(struct VACM_s* vacm_obj)
{
  vacm_obj->state = VACM_STATE_STANDBY;
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_machine()
 * @brief top level state machine
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * Called every ISR pass for each phase
 *********************************************************************************/
void vacm_state_machine(struct VACM_s* vacm_obj)
{
  vacm_capture_input(vacm_obj);
  vacm_vloop_feedforward_update(vacm_obj);
  switch (vacm_obj->state)
  {
    case VACM_STATE_STANDBY:
      vacm_state_standby(vacm_obj);
      break;

    case VACM_STATE_DCDETECT:
      vacm_state_dc_detect(vacm_obj);
      break;

    case VACM_STATE_DCMODE:
      vacm_state_dc_mode(vacm_obj);
      break;

    case VACM_STATE_WAIT_ZC:
      vacm_state_wait_for_zc(vacm_obj);
      break;

    case VACM_STATE_ACSYNC:
      vacm_state_ac_sync(vacm_obj);
      break;

    case VACM_STATE_ONLINE:
      vacm_state_online(vacm_obj);
      break;

    case VACM_STATE_ACDROP:
      vacm_state_ac_drop(vacm_obj);
      break;

    default:
      vacm_obj->state = VACM_STATE_STANDBY;
      break;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_standby()
 * @brief handler for state VACM_STATE_STANDBY
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_standby(struct VACM_s* vacm_obj)
{
  vacm_vin_average(vacm_obj);
  if ((vacm_obj->vin.avg) && (!vacm_obj->fault.bits.uv))
  {
    // there is some Vin present 

    // clear zc timeout flag, as re-starting zero cross detection
    // also reset fmin and fmax flags, as will be re-measuring the line
    // frequency.                 
    vacm_obj->fault.bits.zc_timeout = false;
    vacm_obj->fault.bits.fmax = false;
    vacm_obj->fault.bits.fmin = false;

    // proceed to next state
    vacm_obj->state = VACM_STATE_DCDETECT;
  }
}

/*******************************************************************************
 * @fn	vacm_state_dc_detect()
 * @ingroup pri-layer-VAC
 * @brief handler for state VACM_STATE_DCDETECT
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * goal is to determine if input is DC or not
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_dc_detect(struct VACM_s* vacm_obj)
{
  vacm_vin_average(vacm_obj);
  bool dc_detect_processing_complete = vacm_dc_input_accept(vacm_obj);

  if (vacm_obj->fault.value)
  {
    // fault present, reset state machine
    vacm_reset_phase_monitor_object(vacm_obj);
    vacm_obj->state = VACM_STATE_STANDBY;
  }
  else if (dc_detect_processing_complete)
  {
    if (vacm_obj->status.bits.dc_mode)
    {
      // DC mode
      vacm_obj->state = VACM_STATE_DCMODE;
    }
    else
    {
      // AC mode
      vacm_obj->state = VACM_STATE_WAIT_ZC;
    }
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_dc_mode()
 * @brief handler for state VACM_STATE_DCMODE
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * runs DC mode processing
 * if there is a fault (UV or OV only possible faults)
 * or if the input drops below DC acceptance threshold
 * then reset state machine and re-consider if input is AC or DC
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_dc_mode(struct VACM_s* vacm_obj)
{
  vacm_vin_average(vacm_obj);
  if ((vacm_obj->fault.value) || (vacm_obj->vin.rectified < VACM_DC_REJECTION_THRESHOLD)) 
  {
    // fault, or signal looks like it could be AC, so reset state machine
    vacm_reset_phase_monitor_object(vacm_obj);
    vacm_obj->state = VACM_STATE_STANDBY;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_wait_for_zc()
 * @brief handler for state VACM_STATE_WAIT_ZC
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * if here, signal looks like AC
 * wait for first zero crossing so we can sync to this event
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_wait_for_zc(struct VACM_s* vacm_obj)
{
  // wait for next zero crossing
  vacm_vin_average(vacm_obj);
  vacm_zerocross_detect(vacm_obj);
  vacm_polarity_update(vacm_obj);

  if (vacm_obj->fault.value)
  {
    vacm_reset_phase_monitor_object(vacm_obj);
    vacm_obj->state = VACM_STATE_STANDBY;
  }
  else if (vacm_obj->status.bits.end_of_cycle)
  {
    // zero crossing has been detected

    // going to start to measure AC line cycle from this point
    // so clear the cycle timer
    vacm_obj->timebase.half_cycle_timer = 0;

    vacm_obj->state = VACM_STATE_ACSYNC;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_ac_sync()
 * @brief handler for state VACM_STATE_ACSYNC
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * compared to state VACM_STATE_WAIT_ZC, we are now measuring the AC line cycle period
 * if we have 10 consecutive AC line periods with stable Vin
 * then we proceed to VACM_STATE_ONLINE and assert the ac_ok flag
 * note that if an AC drop occurs here, it will be treated as a fault
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_ac_sync(struct VACM_s* vacm_obj)
{
  vacm_vin_average(vacm_obj);
  vacm_zerocross_detect(vacm_obj);
  vacm_polarity_update(vacm_obj);
  vacm_timebase_update(vacm_obj);
  vacm_acok_assert(vacm_obj);

  if (vacm_obj->fault.value)
  {
    // some fault present. Go back to standby
    vacm_reset_phase_monitor_object(vacm_obj);
    vacm_obj->state = VACM_STATE_STANDBY;
  }
  else if (vacm_obj->status.bits.ac_ok)
  {
    // AC OK flag is set, go to next state
    vacm_obj->state = VACM_STATE_ONLINE;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_online()
 * @brief handler for state VACM_STATE_ONLINE
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * if an AC drop is detected in this state, it is not treated as a fault
 * instead we move to VACM_STATE_ACDROP, where the AC drop timeout is implemented
 * note that AC drop is only detected if Vin drops to 0 (like zero cross)
 * a severe voltage sag that will not result in the AC drop flag being set
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_online(struct VACM_s* vacm_obj)
{
  vacm_vin_average(vacm_obj);
  vacm_zerocross_detect(vacm_obj);
  vacm_polarity_update(vacm_obj);
  vacm_timebase_update(vacm_obj);

  if (vacm_obj->fault.value)
  {
    // some fault is set. if it is 
    // a) zero cross timeout
    // b) fmax
    // then it is potentially an AC drop
    // otherwise, reset the state machine
    if ((vacm_obj->fault.bits.fmax) || (vacm_obj->fault.bits.zc_timeout))
    {
      // potentially an AC drop
      vacm_obj->status.bits.ac_drop = true;

      if (vacm_obj->fault.bits.zc_timeout)
      {
        // AC drop has already been present for ZC_TIMEOUT seconds
        // so preset ac drop timer accordingly
        vacm_obj->timebase.ac_drop_timer = (ZC_TIMEOUT_TICKS + 1);
      }
      else
      {
        vacm_obj->timebase.ac_drop_timer = 0; // start at 0
      }

      // clear zero cross, as during AC drop
      // even though in zero cross region, it is artificial
      vacm_obj->status.bits.zero_cross = false;

      // not running Vin average calculation in the next state
      // as it will trigger a UV fault
      // so clear the accumulator and counter used to calculate the average
      vacm_obj->avgcalc_buffer.acc = 0;
      vacm_obj->avgcalc_buffer.counter = 0;

      vacm_obj->state = VACM_STATE_ACDROP; // measure length of AC drop
    }
    else
    {
      // OV, UV or fmin fault. Reset state machine
      vacm_reset_phase_monitor_object(vacm_obj);
      vacm_obj->state = VACM_STATE_STANDBY;
    }
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_state_ac_drop()
 * @brief handler for state VACM_STATE_ACDROP
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * if AC drop is present for longer than AC_DROP_TIMEOUT seconds
 * this is a fault, so reset state machine
 * if Vin recovers before AC_DROP_TIMEOUT seconds has elapsed
 * then go to state VACM_STATE_WAIT_ZC to try to re-sync to AC line
 *********************************************************************************/
void __attribute__((always_inline)) vacm_state_ac_drop(struct VACM_s* vacm_obj)
{
  vacm_obj->timebase.ac_drop_timer++;

  // if there is some voltage present, then clear flag
  if (vacm_obj->vin.rectified > VACM_AC_DROP_VOLTAGE_THRESHOLD) 
  {
    // some input voltage is present
    if (vacm_obj->timebase.vin_present_counter++ > VACM_AC_DROP_PERSISTANCE_COUNT) 
    {
      vacm_obj->timebase.vin_present_counter = 0;

      // AC drop condition no longer present, so clear flag and timer
      vacm_obj->status.bits.ac_drop = false;
      vacm_obj->timebase.ac_drop_timer = 0;

      // reset cycle timer
      vacm_obj->timebase.half_cycle_timer = 0;

      // clear fault flags
      vacm_obj->fault.bits.fmax = false;
      vacm_obj->fault.bits.zc_timeout = false;

      vacm_obj->state = VACM_STATE_WAIT_ZC; // next state      
    }
  }
  else
  {
    // AC drop still present        
    vacm_obj->timebase.vin_present_counter = 0;

    if (vacm_obj->timebase.ac_drop_timer > AC_DROP_TIMEOUT_TICKS)
    {
      // AC drop is present for too long, so reset state machine
      vacm_reset_phase_monitor_object(vacm_obj);
      vacm_obj->state = VACM_STATE_STANDBY;
    }
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_vin_average()
 * @brief average the input voltage over 1/2 line cycle
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * average the input voltage over 1/2 line cycle
 * and store the result in vacm_obj->vin.avg
 * also average the input voltage over 4 line cycles
 * and store the result in vacm_obj->vin.avg_4_ac_cycles
 * each time averages are calculated, check against UV and OV thresholds
 * also, calculate Vavg squared. This is used to compute the feedforward term
 * (Vin/Vavg^2) for the PFC voltage loop 
 *
 * note that the average does not necessarily occur at the zero crossings
 * but it does run once per 1/2 AC line cycle
 * by default, it is run assuming Vin freq is 50Hz (so at 10ms)
 * once we have a valid measurement of the input frequency, the average computation
 * is then run at 2x the measured line frequency
 * this way, the average computation can be left to run at all times, even when
 * the input is DC
 *********************************************************************************/
void __attribute__((always_inline)) vacm_vin_average(struct VACM_s* vacm_obj)
{
  //----------------------------------------------------------------------------
  // average (the rectified) ADC readings over a half line cycle
  //----------------------------------------------------------------------------    
  vacm_obj->avgcalc_buffer.acc += vacm_obj->vin.rectified;
  vacm_obj->avgcalc_buffer.counter++;

  // check if a 1/2 line half cycle has elapsed
  // vacm_obj->avgcalc_buffer.counter_reset should be equal to the length of a 1/2 line cycle
  // as measured in IRQ ticks
  if (vacm_obj->avgcalc_buffer.counter >= vacm_obj->avgcalc_buffer.counter_reset)
  {
    // compute the average of the rectified input voltage over a half line cycle
    vacm_obj->vin.avg_prev = vacm_obj->vin.avg; // store old value
    vacm_obj->vin.avg = (uint16_t) __builtin_divud(vacm_obj->avgcalc_buffer.acc, vacm_obj->avgcalc_buffer.counter);

    vacm_obj->avgcalc_buffer.acc = 0;
    vacm_obj->avgcalc_buffer.counter = 0;

    // check UV and OV
    vacm_uv_ov_test(vacm_obj);

    //----------------------------------------------------------------------
    // compute Vavg squared. This is used to compute Vin/Vavg^2
    // this is feed-forward term for the voltage loop on the main controller
    // note that vacm_obj->vin.avg is 11 bit (not 12-bit) 
    // because it is based on the rectified ADC measurement
    //----------------------------------------------------------------------
    uint32_t vin_avg_sqrd = __builtin_muluu(vacm_obj->vin.avg, vacm_obj->vin.avg); // 11bit * 11bit = 22bit;
    vacm_obj->vin.avg_sqrd = (uint16_t) (vin_avg_sqrd >> 6); // 22bit / 6bit = 16bit                       
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_polarity_update()
 * @brief determine the input polarity and update flag accordingly
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * 
 *********************************************************************************/
void __attribute__((always_inline)) vacm_polarity_update(struct VACM_s* vacm_obj)
{
  //----------------------------------------------------------------------------
  // determine polarity of AC input signal
  //----------------------------------------------------------------------------
  // AC input = 180 degrees out of phase with sensed signals from op-amps
  // so if 
  // >> sensed signal is < (raw AC  - hysteresis): AC input is positive
  // >> sensed signal is > (raw AC  + hysteresis): AC input is negative         
  int16_t offset = (int16_t) * vacm_obj->vin.ptr_offset;
  if (vacm_obj->vin.raw < (offset - VACM_POLARITY_HYSTERESIS)) 
  {
    vacm_obj->status.bits.polarity_pos = true;
  }
  else if (vacm_obj->vin.raw > (offset + VACM_POLARITY_HYSTERESIS)) 
  {
    vacm_obj->status.bits.polarity_pos = false;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_zerocross_detect()
 * @brief determine whether or not Vin is in the zero cross region
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * 
 *********************************************************************************/
void __attribute__((always_inline)) vacm_zerocross_detect(struct VACM_s* vacm_obj)
{
  // 1. coming into zero cross region from below
  // -> slope = 1, polarity = 0
  // 2. coming out of zero cross region from below
  // -> slope = 1, polarity = 1
  // 3. coming into zero cross region from above
  // -> slope = 0, polarity = 1
  // 4. coming out of zero cross region from above
  // -> slope = 0, polarity = 0

  // hence
  // >> when coming into zero cross region: slope != polarity
  // >> when coming out of zero cross region: slope = polarity

  // check if coming into zero cross region
  if ((vacm_obj->vin.rectified < VCAM_ZERO_CROSS_THRESHOLD) && // Vin < threshold 
      (vacm_obj->status.bits.slope_pos != vacm_obj->status.bits.polarity_pos) &&
      (!vacm_obj->status.bits.zero_cross)) // haven't been in zero cross region
  {
    vacm_obj->status.bits.zero_cross = true; // in zero cross region
    vacm_obj->status.bits.end_of_cycle = true; // end of cycle has occurred (should happen once per 1/2 period)
  }
    // check if coming out of zero cross region
  else if ((vacm_obj->vin.rectified > VCAM_ZERO_CROSS_THRESHOLD) && // Vin > threshold 
           (vacm_obj->status.bits.slope_pos == vacm_obj->status.bits.polarity_pos) &&
           (vacm_obj->status.bits.zero_cross)) // have been in zero cross region
  {
    vacm_obj->status.bits.end_of_cycle = false;
    // stay in zero cross region for minimum amount of time
    if (vacm_obj->timebase.zero_cross_timer >= ZC_TMIN_TICKS)
    {
      vacm_obj->status.bits.zero_cross = false; // no longer in zero cross region
    }
  }
  else
  {
    vacm_obj->status.bits.end_of_cycle = false;
  }

  //----------------------------------------------------------------------------
  // measure the length of time in zero cross range
  //----------------------------------------------------------------------------
  if (vacm_obj->status.bits.zero_cross)
  {
    vacm_obj->timebase.zero_cross_timer++;
    if (vacm_obj->timebase.zero_cross_timer > ZC_TIMEOUT_TICKS)
    {
      vacm_obj->timebase.zero_cross_timer = 0;
      vacm_obj->fault.bits.zc_timeout = true;
      vacm_obj->status.bits.zero_cross = false; // reset zero cross flag
    }
  }
  else
  {
    vacm_obj->timebase.zero_cross_timer = 0;
    vacm_obj->fault.bits.zc_timeout = false;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_vloop_feedforward_update()
 * @brief Compute PFC voltage loop feedforward term
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * Compute voltage loop feedforward term, that is Vin / Vavg^2. 
 * store result in vacm_obj->vin.vloop_ff
 *********************************************************************************/
void __attribute__((always_inline)) vacm_vloop_feedforward_update(struct VACM_s* vacm_obj)
{
  if (vacm_obj->vin.avg_sqrd > 0) // check against 0 as dividing by this number    
  {
    // rectified_scaled: 11bit * 17bit = 28 bit
    uint32_t rectified_scaled = ((uint32_t) vacm_obj->vin.rectified) << 17; 

    // vacm_obj->vin.vloop_ff: 28bit / 16bit = 12bit
    vacm_obj->vin.vloop_ff = (uint16_t) (__builtin_divud(rectified_scaled, vacm_obj->vin.avg_sqrd));
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_timebase_update()
 * @brief measure length of 1/2 AC line cycle in IRQ ticks
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * measure length of 1/2 AC line cycle in IRQ ticks
 * check if freq is within allowed max/min range
 *********************************************************************************/
void __attribute__((always_inline)) vacm_timebase_update(struct VACM_s* vacm_obj)
{
  vacm_obj->timebase.half_cycle_timer++;

  if (vacm_obj->status.bits.end_of_cycle)
  {
    // "end_of_cycle" set on the pass where a zero crossing is detected
    // so a "cycle" is defined actually a 1/2 of a AC line period
    // store measured period (actually its a measured of a half of the line cycle)
    vacm_obj->timebase.half_cycle = vacm_obj->timebase.half_cycle_timer;
    vacm_obj->timebase.half_cycle_timer = 0;

    // check against Fmax. Fmax = min period, so check if ticks < min period
    // in ticks. Divide by 2 as measuring over 1/2 cycle instead of full cycle
    if (vacm_obj->timebase.half_cycle < (FIN_MAX_TICKS >> 1))
    {
      vacm_obj->fault.bits.fmax = true;
      vacm_obj->fault.bits.fmin = false;
    }
    else
    {
      // measured line frequency is within limits
      // update reset value of timer used to calculate average of Vin rectified
      // only update vin_average_counter_reset when get a valid period measurement
      vacm_obj->avgcalc_buffer.counter_reset = vacm_obj->timebase.half_cycle;

      // clear fault flags
      vacm_obj->fault.bits.fmin = false;
      vacm_obj->fault.bits.fmax = false;
    }
  }
  else
  {
    // not end of cycle
    // check against Fmin (max period)
    // divide by 2 as measuring over a 1/2 AC line cycle
    if (vacm_obj->timebase.half_cycle_timer > (FIN_MIN_TICKS >> 1))
    {
      vacm_obj->timebase.half_cycle_timer = 0; // reset timer
      vacm_obj->fault.bits.fmin = true;
      vacm_obj->fault.bits.fmax = false;
    }
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_acok_assert()
 * @brief assert ac ok flag after 10 stable AC line periods
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 *********************************************************************************/
void __attribute__((always_inline)) vacm_acok_assert(struct VACM_s* vacm_obj)
{
  // set AC OK flag when Vin has been in a settled range for 10 consecutive AC line cycles
  // can determine by checking if the average half period value doesn't move so much between periods
  if (vacm_obj->status.bits.end_of_cycle)
  {
    if (!vacm_obj->status.bits.ac_ok)
    {
      // check how much Vin has changed from 1 half-period to the next        
      int16_t delta_vin = abs((int16_t) (vacm_obj->vin.avg) - (int16_t) (vacm_obj->vin.avg_prev));

      if (delta_vin < ((vacm_obj->vin.avg) >> 4))
      {
        // average line voltage is stable           
        if (vacm_obj->timebase.startup_counter++ == VCAM_STARTUP_HALF_GRID_CYCLE_COUNT) 
        {
          vacm_obj->status.bits.ac_ok = true;
          vacm_obj->timebase.startup_counter = 0;
        }
      }
      else
      {
        // Vin is moving by too much, so reset startup counter
        vacm_obj->timebase.startup_counter = 0;
      }
    }
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_capture_input()
 * @brief measure input voltage with ADC
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 * rectify measurement (using measured op amp bias voltage)
 * determine slope of the AC input
 *********************************************************************************/
void __attribute__((always_inline)) vacm_capture_input(struct VACM_s* vacm_obj)
{
  // read the ADC result
  vacm_obj->vin.raw_prev = vacm_obj->vin.raw; // store previous value

  int16_t vac_buffer = (int16_t) * vacm_obj->vin.ptr_adcbuf;
  vacm_obj->vin.raw = (uint16_t) vac_buffer;

  // rectify
  int16_t offset = (int16_t) * vacm_obj->vin.ptr_offset;
  vacm_obj->vin.rectified = abs(vac_buffer - offset);

  // clamp at 11 bit value
  if (vacm_obj->vin.rectified > 2047)
  {
    vacm_obj->vin.rectified = 2047;
  }

  // determine slope of input
  // used for zero crossing detection
  // account for 180 degree phase shift between AC input and sensed voltage
  if (vacm_obj->vin.raw < vacm_obj->vin.raw_prev)
  {
    // sensed signal is descending, so actual AC line is ascending
    vacm_obj->status.bits.slope_pos = true;
  }
  else
  {
    // sensed signal is ascending, so actual AC line is descending
    vacm_obj->status.bits.slope_pos = false;
  }
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_dc_input_accept()
 * @brief determine if the input is DC or AC
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 *********************************************************************************/
bool __attribute__((always_inline)) vacm_dc_input_accept(struct VACM_s* vacm_obj)
{
  bool processing_complete = false; // return value

  // Check if the input voltage is above the user-specified minimum threshold
  // for a period of at least four maximum grid periods
  if (vacm_obj->vin.rectified > VACM_DC_ACCEPTANCE_THRESHOLD) 
  {
    vacm_obj->timebase.dc_loss_counter = 0;
    vacm_obj->timebase.dc_present_counter++;
  }

    // If input voltage disappears, set time-out and clear DC Mode flag bits
    // To indicate potential AC input voltage or grid loss
  else if (vacm_obj->vin.rectified < VACM_DC_REJECTION_THRESHOLD) 
  {
    vacm_obj->timebase.dc_loss_counter++;
    vacm_obj->timebase.dc_present_counter = 0;
  }

  // If the time-base counter exceeds the limit of at least four half periods, Accept DC mode
  if (vacm_obj->timebase.dc_present_counter > DC_ACCEPTANCE_PERIOD_TICKS)
  {
    // signal looks like DC
    vacm_obj->timebase.dc_present_counter = 0;

    vacm_obj->status.bits.dc_mode = true; // Set DC Mode flag bit

    // set other status flags appropriately
    vacm_obj->status.bits.ac_ok = true;
    vacm_obj->status.bits.ac_drop = false;
    vacm_obj->status.bits.end_of_cycle = false;
    vacm_obj->status.bits.polarity_pos = true;
    vacm_obj->status.bits.slope_pos = false;
    vacm_obj->status.bits.zero_cross = false;

    // tell calling function that a decision has been made
    processing_complete = true;
  }

    // if voltage drops below detection threshold more that 1/2 of 
    // the min zero cross time, then signal must be considered as AC
  else if (vacm_obj->timebase.dc_loss_counter > (max((ZC_TMIN_TICKS >> 1), 1)))
  {
    // signal looks like it might be AC 
    vacm_obj->timebase.dc_loss_counter = 0;
    vacm_obj->status.bits.dc_mode = false; // clear DC Mode flag bit

    // tell calling function that a decision has been made
    processing_complete = true;
  }

    // While test is on-going, keep DC mode and period time-base timeout flag bits cleared
  else
  {
    vacm_obj->status.bits.dc_mode = false;
    processing_complete = false; // tell calling function that a decision has been made
  }

  return (processing_complete);
}

/*******************************************************************************
 * @ingroup pri-layer-VAC
 * @fn	vacm_uv_ov_test()
 * @brief check average Vin against UV and OV    
 * @param	pointer to phase monitor object
 * @return None
 *  
 * @details
 *********************************************************************************/
void __attribute__((always_inline)) vacm_uv_ov_test(struct VACM_s* vacm_obj)
{
  // check Vin against UV thresholds
  if (!vacm_obj->fault.bits.uv)
  {
    // Vin not UV
    // if Vin drops below trigger threshold, set UV flag
    if (vacm_obj->vin.avg < UV_TRIG_ADC)
    {
      vacm_obj->fault.bits.uv = true;
    }
  }
  else
  {
    // Vin in UV
    // if Vin goes above clear threshold, clear UV flag
    if (vacm_obj->vin.avg > UV_CLR_ADC)
    {
      vacm_obj->fault.bits.uv = false;
    }
  }

  // check against OV thresholds
  if (!vacm_obj->fault.bits.ov)
  {
    // not in OV
    // if Vin > OV trigger threshold, set OV fault flag
    if (vacm_obj->vin.avg > OV_TRIG_ADC)
    {
      vacm_obj->fault.bits.ov = true;
    }
  }
  else
  {
    // in OV
    // if Vin < OV clear threshold, clear OV fault flag
    if (vacm_obj->vin.avg < OV_CLR_ADC)
    {
      vacm_obj->fault.bits.ov = false;
    }
  }
}