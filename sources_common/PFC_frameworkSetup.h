/**
 * PFC framework Setup
 * 
 * @file      PFC_frameworkSetup.h
 * 
 * @ingroup   setup
 * 
 * @brief     Allows the user to change the advanced operating parameters 
*/
/*
 [2024] Microchip Technology Inc. and its subsidiaries.

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
//#define CHARGEPUMP_ENABLED      //chargepump option for startup after ZC
//#define COMMON_MODE_DETECTION   //AC_N measurement to limit startup after ZC time
#undef AUTO_START               //PFC starts after poweron. not via GUI

//*** DEBUG ONLY *** 
//#define ADAPTIVE_GAIN_SLIDER_MODE   //only for debug purposes
//#define VOUT_PRECHARGE_PCS_ENABLE   //only for debug purposes
//*** ---------- *** 

#if HIGH_VOLTAGE
    #define VOUT_REF 3152   //400 V
    #define IIN_OC 930      //30 A
    #define VOUT_OV 3546    //450 V
    #define CCM_CURRENT 62  //2.0 A
    #define DEAD_TIME 400   //100 ns
    #define VOUT_PRECHARGE_VOLTAGE 360  // Volt
    #define VOUT_PRECHARGE_VALUE (uint16_t)(VOUT_PRECHARGE_VOLTAGE * 7.88)    //2834   //360V 
#else
    #define VOUT_REF 1494   //40 V
    #define IIN_OC 930      //30 A
    #define VOUT_OV 3362    //90 V
    #define CCM_CURRENT 62  //2.0 A
    #define DEAD_TIME 320   //80 ns
    #define VOUT_PRECHARGE_VOLTAGE 36   // Volt
    #define VOUT_PRECHARGE_VALUE (uint16_t)(VOUT_PRECHARGE_VOLTAGE * 37.23) 
#endif

#if HIGH_VOLTAGE
    #define BURST_MODE_VMC_REF_FREEZE 200
    #define BURST_MODE_LEVEL_HIGH 150
#else 
#ifdef MODE_INTERLEAVED
    #define BURST_MODE_VMC_REF_FREEZE 950
    #define BURST_MODE_LEVEL_HIGH 900
#else
    #define BURST_MODE_VMC_REF_FREEZE 1900
    #define BURST_MODE_LEVEL_HIGH 1800
#endif
#endif

//*** Open Loop Startup after Zero Cross *** 
#if HIGH_VOLTAGE
    #define OPEN_LOOP_STARTUP_PULSES 16
    #define OPEN_LOOP_PWM_COUNTER_SHIFT 10 
#else
    #define OPEN_LOOP_STARTUP_PULSES 10 
    #define OPEN_LOOP_PWM_COUNTER_SHIFT 10 
#endif

#if defined MODE_PFC 
    #define M_HIGHLINE 0.64 //(0,75 - 0,4) / (20000 - 2000) *32767
    #define BX_HIGHLINE 2000
    #define GAINSLOPE_HIGHLINE  (uint16_t)(M_HIGHLINE * 32767)
    #define GAINOFFSET_HIGHLINE (uint16_t)(0.75 * 32767 + BX_HIGHLINE * M_HIGHLINE)

    #define M_LOWLINE 0.96 //(0,66 - 0,30) / (13200 - 1000) * 32767
    #define BX_LOWLINE 1000
    #define GAINSLOPE_LOWLINE  (uint16_t)(M_LOWLINE * 32767)
    #define GAINOFFSET_LOWLINE (uint16_t)(0.66 * 32767 + BX_LOWLINE * M_LOWLINE)
#endif

#if defined MODE_INTERLEAVED
    #define M_HIGHLINE 0.64 //(0,75 - 0,4) / (20000 - 2000) *32767
    #define BX_HIGHLINE 2000
    #define GAINSLOPE_HIGHLINE  (uint16_t)(M_HIGHLINE * 32767)
    #define GAINOFFSET_HIGHLINE (uint16_t)(0.75 * 32767 + BX_HIGHLINE * M_HIGHLINE)

    #define M_LOWLINE 0.96 //(0,66 - 0,30) / (13200 - 1000) * 32767
    #define BX_LOWLINE 1000
    #define GAINSLOPE_LOWLINE  (uint16_t)(M_LOWLINE * 32767)
#define GAINOFFSET_LOWLINE (uint16_t)(0.66 * 32767 + BX_LOWLINE * M_LOWLINE)
#endif

/*******************************************************************************
 * @fn          definitions for MCC API's    
 * @brief       
 * @details     
 * @ingroup     setup             
 *              
 *******************************************************************************/

#define PH1_PWM 1
#define PH2_PWM 2
#define PH3_PWM 3

//==============================================================================
//==============================================================================

#endif	/* DRV_USERSETUP_TPBLPFC_H */

