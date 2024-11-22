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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef REVISION_H
#define	REVISION_H

#define MAJOR       '9'
#define MINOR       '0'
#define PATCH       '2'


/*******************************************************************************
 * @fn      revision history 1.1.0
 * @details dedicated single phase version
 *          1ph-PFC, 1ph-GTI, 1ph-interleave 2 legs running PWM in parallel
 *          Burst:                  all modes same numbers
 *          Adaptive Gain:          no Vin consideration
 *          DFF:                    all modes
 *          Adaptive Step Response: only load release
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.2
 * @details dedicated single phase version
 *          1ph-PFC, 1ph-GTI, 1ph-interleave 2 legs running PWM in parallel
 *          Burst:                  all modes same numbers
 *          Adaptive Gain:          no Vin consideration
 *          DFF:                    all modes
 *          Adaptive Step Response: only load release
 *          startup after ZC modified:
 *              extra check before write DC
 *              reset DC to 0 before startup after ZC 
 *          protocolB increased to 10 words to get Vin avg          
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.3
 * @details dedicated single phase version addition to 1.1.2
 *          Adaptive Gain:  with Vin consideration  
 *                          #ifdef for GTO with fixed gain of 0.25
 *          Iac average current calculation for L1 
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.4
 * @details dedicated single phase version addition to 1.1.3
 *          Adaptive Gain:  #ifdef in "drv_pwrctrl_TPBLPFC.h"
 *                          for MAX_GAIN_FACTOR difference LV/HV
 *                          HV has lower adaptive Gain than LV
 *                          needs further investigations
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.5
 * @details 
 *          variable for open loop startup time added as PWM cycle counter (10us)
 *          this is especially to check chargepump option with variable time
 *          where the chargepump is on (fix on time before implementing the
 *          AC_N measurement
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.6
 * @details 
 *          additional power control state for Vout precharge added
 *          precharge after switch on and before Relais on
 *          precharge to fixed level 360V at HV
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.6
 * @details 
 *          additional power control state for Vout precharge added
 *          precharge after switch on and before Relais on
 *          precharge to fixed level 360V at HV
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 9.0.0
 * @details 
 *          Interleaved, 2 legs
 *          Changes: PWM setup edge aligned with PER=40000
 *                   ADC-ISR's
 *                   DCDT generated h- files max value
 *                   2p2z asm file to exceed 32767 limit
 *                   Gains double because of PWM PER is now double
 *                   PWM setup with Melody lib
 * 
 * @fn     revision history 9.0.1
 * @details 
 *              adaptive gain with a straight calculation
 *              2 straight calculation values <> 140V
 *  
 * @fn     revision history 9.0.2
 * @details 
 *              Interleaved mode option in place and working
 *******************************************************************************/

#endif	/* REVISION_H */

