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

#ifndef DRV_USERSETUP_TPBLPFC_H
#define	DRV_USERSETUP_TPBLPFC_H

#include "Totempole_Application_Setup.h"

/***********************************************************************************
 * @ingroup setup
 * @fn      PFC_framewrokSetup
 * @brief   settings coming from Totem-Pole Application Setup header file
 * @details 
  *         #ifdefs based on the settings in the file 
 *          "Totempole_Application_Setup.h"
 *          file can be edit by hand, NOT RECOMMENDED
 *          Current Sensor: 0.025 V/A 
 **********************************************************************************/

#define SYNCHRONOUS_ACTIVE_SWITCH 
#define RECTIFIER_SWITCH 

#if defined MODE_PFC || defined MODE_INTERLEAVED
    #define VOLTAGE_LOOP 
    #define DUTY_RATIO_FEEDFORWARD_ENABLED 
    #define AC_CYCLE_SKIPPING_ENABLED 
    #define ADAPTIVE_GAIN_ENABLED 
    #define ADAPTIVE_STEPRESPONSE_ENABLED 
#endif

//always considered in PFC and GTI
#undef CHARGEPUMP_ENABLED 
#undef AUTO_START 
#undef COMMON_MODE_DETECTION    //future hardware release

#ifdef HIGHVOLTAGE
    #define VOUT_REF 3152  //400 V
    #define IIN_OC 930  //30 A
    #define VOUT_OV 3546  //450 V
    #define CCM_CURRENT 62  //2.0 A
    #define DEAD_TIME 400  //100 ns
#else
    #define VOUT_REF 1494  //40 V
    #define IIN_OC 930  //30 A
    #define VOUT_OV 3362  //90 V
    #define CCM_CURRENT 62  //2.0 A
    #define DEAD_TIME 320  //80 ns
#endif

#ifdef HIGHVOLTAGE
    #define BURST_MODE_VMC_REF_FREEZE 200
    #define BURST_MODE_LEVEL_HIGH 150
#else
    #define BURST_MODE_VMC_REF_FREEZE 1900
    #define BURST_MODE_LEVEL_HIGH 1800
#endif

//*** Open Loop Startup after Zero Cross *** 
#define OPEN_LOOP_STARTUP_PULSES 10 
#define OPEN_LOOP_PWM_COUNTER_SHIFT 10 

/*******************************************************************************
 * @fn          definitions for MCC API's    
 * @brief       
 * @details     
 *              
 *              
 *******************************************************************************/

#define PH1_PWM 1
#define PH2_PWM 2
#define PH3_PWM 3

//==============================================================================
//==============================================================================

#endif	/* DRV_USERSETUP_TPBLPFC_H */

