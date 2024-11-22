/**
 * VAC Monitor Driver Header File
 * 
 * @file      vac_monitor.h
 * 
 * @ingroup   pri-layer-VAC
 * 
 * @brief     header of the VAC
 *
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef VAC_MONITOR_H
#define	VAC_MONITOR_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "vac_monitor_typedef.h"
#include "../../../revision.h"
#include "../sources_common/Totempole_Application_Setup.h"

/*********************************************************************************
 * @ingroup public-macros
 * @def     rnd()
 * @brief   rounding floats to uint16_t
 * @details
 * rounds float to 16-bit unsigned integer.
 * **********************************************************************************/
#define rnd(a)      ((uint16_t)((a)+((a)<0?-0.5:0.5)))

/*********************************************************************************
 * @ingroup public-macros
 * @def     min()
 * @brief   min of 2 integers
 * @details
 * determines minimum value of 2 unsigned 16-bit integers.
 * **********************************************************************************/
#define min(a,b)    ((uint16_t)(((a)>(b)) ? (b):(a)))

/*********************************************************************************
 * @ingroup public-macros
 * @def     max()
 * @brief   max of 2 integers
 * @details
 * determines maximum value of 2 unsigned 16-bit integers.
 * **********************************************************************************/
#define max(a,b)    ((uint16_t)(((a)>(b)) ? (a):(b)))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     FCALL_AC_MONITOR
 * @brief   frequency at which AC monitor code is executed (in Hertz)
 * @details
 * To measure time (for example zero cross timeout), the firmware needs to know the 
 * frequency at which the AC monitor state machine(s) is called. This frequency (in Hz)
 * is set via this macro.
 * @warning Usually the AC monitor state machines are called from a periodic interrupt.
 * This will be set by the calling firmware, which is outside the scope of this 
 * project. If the call frequency is changed in the calling firmware, it needs to be 
 * changed here also.
 * **********************************************************************************/
#define FCALL_AC_MONITOR         (1.0/10.0e-6)

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     AC_SENSE_GAIN
 * @brief   gain between AC input and ADC pin at which it is sensed (Volts/Volts)
 * @details
 * Each AC line voltage (works for DC also) is sensed at a ADC input pin. This parameter
 * defines the gain between the AC line and the ADC pin. It is different for high 
 * and low voltage hardware configurations.
 * **********************************************************************************/
#if HIGH_VOLTAGE == 1 // defined in revision.h
#define AC_SENSE_GAIN             (3.771e-3)
#else 
#define AC_SENSE_GAIN             (37.585e-3)
#endif // #if HIGH_VOLTAGE == 1 

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     FIN_MIN
 * @brief   min AC line frequency (Hertz)
 * @details
 * AC okay flag is cleared if the AC line frequency is below this value.
 * **********************************************************************************/
#define FIN_MIN                   (40.0)

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     FIN_MAX
 * @brief   max AC line frequency (Hertz)
 * @details
 * AC okay flag is cleared if the AC line frequency is above this value.
 * **********************************************************************************/
#define FIN_MAX                   (65.0)

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     UV_TRIG
 * @brief   Input under voltage trigger threshold (Volts)
 * @details
 * Based on the average of the rectified AC input voltage (not RMS). This average
 * is computed once per half line cycle. AC okay flag cleared if Vin drops below
 * this threshold
 * **********************************************************************************/
#if HIGH_VOLTAGE == 1
#define UV_TRIG               10//<<<(60.0)
#else
#define UV_TRIG               (8.0)
#endif // #if HIGH_VOLTAGE == '1'

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     UV_HYS
 * @brief   Input under voltage hysteresis (Volts)
 * @details
 * Vin UV fault is cleared if Vin goes back above (UV_TRIG + UV_HYS) Volts. 
 * Value is based on the average of the rectified AC input voltage (not RMS). 
 * **********************************************************************************/
#define UV_HYS                (2.0)

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     OV_TRIG
 * @brief   Input over voltage trigger threshold (Volts)
 * @details
 * Based on the average of the rectified AC input voltage (not RMS). This average
 * is computed once per half line cycle. AC okay flag cleared if Vin goes above
 * this threshold
 * **********************************************************************************/
#if HIGH_VOLTAGE == 1
#define OV_TRIG               350//<<<(240.0)
#else
#define OV_TRIG               (80.0)
#endif // #if HIGH_VOLTAGE == '1'

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     OV_HYS
 * @brief   Input over voltage hysteresis (Volts)
 * @details
 * Vin OV fault is cleared if Vin goes back below (OV_TRIG - UV_HYS) Volts. 
 * Value is based on the average of the rectified AC input voltage (not RMS). 
 * **********************************************************************************/
#define OV_HYS                (2.0)

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     ZC_TIMEOUT
 * @brief   zero cross timeout (seconds)
 * @details
 * If in zero cross region for longer than this time, AC ok flag is cleared
 * **********************************************************************************/
// ZC_TIMEOUT: 
#define ZC_TIMEOUT             (2.0e-3) 

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     ZC_TMIN
 * @brief   min zero cross time (seconds)
 * @details
 * when zero cross region is entered (zero cross flag set), will spend at least this
 * amount if time in zero cross region, even if exit conditions are satisfied
 * **********************************************************************************/
#define ZC_TMIN                (100.0e-6)  

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     AC_DROP_TIMEOUT
 * @brief   AC drop timeout (seconds)
 * @details
 * max amount of time that AC drop can be present before resetting the AC OK flag
 * note that for an AC drop to be detected
 * -# the AC input voltage must drop below the zero cross threshold
 * -# the state machine for the phase must be in the ONLINE state
 * **********************************************************************************/
#define AC_DROP_TIMEOUT           (25.0e-3)

//------------------------------------------------------------------------------
// convert values above from SI units to integers for use by the firmware
// these macros should not need to be modified by the end user
//------------------------------------------------------------------------------

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     DC_ACCEPTANCE_PERIOD_TICKS
 * @brief   DC acceptance period in ticks 
 * @details
 * if Vin > UV trigger threshold and it doesn't change value 
 * over 2 full line cycles, then consider that the signal might be DC.
 * this is measured with a software timer that is incremented every time the 
 * state machine is called. 
 * **********************************************************************************/
#define DC_ACCEPTANCE_PERIOD_TICKS      (rnd(FCALL_AC_MONITOR * 40.0e-3))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     FIN_MIN_TICKS
 * @brief   FIN_MIN (Hertz) converted to ticks for software timer
 * @details
 * number of ticks in corresponding to period of 1 / FIN_MIN.
 * **********************************************************************************/
#define FIN_MIN_TICKS                 (rnd(FCALL_AC_MONITOR / FIN_MIN))  

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     FIN_MAX_TICKS
 * @brief   FIN_MAX (Hertz) converted to ticks for software timer
 * @details
 * number of ticks in corresponding to period of 1 / FIN_MAX.
 * **********************************************************************************/
#define FIN_MAX_TICKS                 (rnd(FCALL_AC_MONITOR / FIN_MAX))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     ZC_TIMEOUT_TICKS
 * @brief   ZC_TIMEOUT (seconds) converted to ticks for software timer
 * **********************************************************************************/
#define ZC_TIMEOUT_TICKS           (rnd(FCALL_AC_MONITOR * ZC_TIMEOUT))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     ZC_TMIN_TICKS
 * @brief   ZC_TMIN (seconds) converted to ticks for software timer
 * **********************************************************************************/
#define ZC_TMIN_TICKS          (rnd(FCALL_AC_MONITOR * ZC_TMIN))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     AC_DROP_TIMEOUT_TICKS
 * @brief   AC_DROP_TIMEOUT (seconds) converted to ticks for software timer
 * **********************************************************************************/
#define AC_DROP_TIMEOUT_TICKS     (rnd(FCALL_AC_MONITOR * AC_DROP_TIMEOUT))
   
// ADC thresholds corresponding to UV and OV levels
// these are compared to the averaged rectified AC values


/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     UV_TRIG_ADC
 * @brief   UV_TRIG (UV trigger threshold) converted from Volts to ADC codes
 * @details
 * Note that the ADC reference voltage is 3.3V and the scaling is 4096 (2^12).
 * **********************************************************************************/
#define UV_TRIG_ADC           (rnd((UV_TRIG*AC_SENSE_GAIN)/3.3*4096))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     UV_CLR_ADC
 * @brief   UV clear threshold converted from Volts to ADC codes
 * @details
 * (UV_TRIG + UV_HYS) Volts converted to ADC code for firmware.
 * Note that the ADC reference voltage is 3.3V and the scaling is 4096 (2^12).
 * **********************************************************************************/
#define UV_CLR_ADC            (rnd(((UV_TRIG + UV_HYS)*AC_SENSE_GAIN)/3.3*4096))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     OV_TRIG_ADC
 * @brief   OV_TRIG_ADC (OV trigger threshold) converted from Volts to ADC codes
 * @details
 * Note that the ADC reference voltage is 3.3V and the scaling is 4096 (2^12).
 * **********************************************************************************/
#define OV_TRIG_ADC           (rnd((OV_TRIG*AC_SENSE_GAIN)/3.3*4096))

/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     OV_CLR_ADC
 * @brief   OV clear threshold converted from Volts to ADC codes
 * @details
 * (OV_TRIG - OV_HYS) Volts converted to ADC code for firmware.
 * Note that the ADC reference voltage is 3.3V and the scaling is 4096 (2^12).
 * **********************************************************************************/
#define OV_CLR_ADC            (rnd(((OV_TRIG - OV_HYS)*AC_SENSE_GAIN)/3.3*4096))


/*********************************************************************************
 * @ingroup pri-layer-VAC
 * @def     HV_FLAG
 * @brief   Used when sending status word from primary to secondary core
 * @details
 * **********************************************************************************/
#if HIGH_VOLTAGE == 1
#define HV_FLAG     (1<<15)
#else
#define HV_FLAG     (0<<15)     
#endif // #if HIGH_VOLTAGE == '1'

// OP AMP Bias, ideal value
#define OPAMP_BIAS_IDEAL            ((uint16_t)(1.65/3.3*4096)) // in ADC codes

// OPAMP_BIAS_HIGH_LIMIT and OPAMP_BIAS_LOW_LIMIT: measured op amp bias should be within 5% of ideal value
#define OPAMP_BIAS_HIGH_LIMIT       ((uint16_t)(OPAMP_BIAS_IDEAL*1.05)) // in ADC codes
#define OPAMP_BIAS_LOW_LIMIT        ((uint16_t)(OPAMP_BIAS_IDEAL*0.95)) // in ADC codes

//------------------------------------------------------------------------------
// function declarations
//------------------------------------------------------------------------------
void vacm_reset_phase_monitor_object(struct VACM_s* vacm_obj);
void vacm_state_machine(struct VACM_s* vacm_obj);
void vacm_reset_state_machine(struct VACM_s* vacm_obj);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */
  
  // TODO If C++ is being used, regular C code needs function names to have C 
  // linkage so the functions can be used by the c code. 

//------------------------------------------------------------------------------
// compile time protection
//------------------------------------------------------------------------------
#ifndef HIGH_VOLTAGE
#error HIGH_VOLTAGE must be defined as either 0 or 1 in revision.h
#else 
#if HIGH_VOLTAGE != 0 && HIGH_VOLTAGE != 1
#error HIGH_VOLTAGE must be defined as either 0 or 1 in revision.h
#endif 
#endif
  
//------------------------------------------------------------------------------
// variables  
//------------------------------------------------------------------------------
extern VACM_t phase1;
extern VACM_t phase2;
extern VACM_t phase3;
  
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* VAC_MONITOR_H */
