/**
 * Totempole Application Setup
 * 
 * @file      Totempole_Application_Setup.h
 * 
 * @ingroup   setup
 * 
 * @brief     Allows the user to easily change the operation modes 
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

#ifndef DRV_USERSETUP_TOTEMPOLEAPPLICATION_H
#define	DRV_USERSETUP_TOTEMPOLEAPPLICATION_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h> 
#include <dsp.h>

/***********************************************************************************
 * @ingroup setup
 * @fn      totempole_application_setup
 * @brief   defines for mode setup
 * @details 
 *          definitions to select the 3 modes for single phase input
 *          PFC
 *          grid tie inverter
 *          interleave with 2 switching legs
 *          set CAN-FD ID
 * 
 *          set one #define for only one mode
 *          comment ot the other 2 options
 * 
 *          #define MODE_PFC
 *          //#define MODE_GRID_TIE_INVERTER
 *          //#define MODE_INTERLEAVED
 * 
 *          //#define MODE_PFC
 *          #define MODE_GRID_TIE_INVERTER
 *          //#define MODE_INTERLEAVED

 *          //#define MODE_PFC
 *          //#define MODE_GRID_TIE_INVERTER
 *          #define MODE_INTERLEAVED
 *
 *          #define HIGH_VOLTAGE 0 for low voltage
 *          #define HIGH_VOLTAGE 1 for high voltage
 **********************************************************************************/

#define HIGH_VOLTAGE 1

//#define MODE_PFC
//#define MODE_GRID_TIE_INVERTER
#define MODE_INTERLEAVED

//==============================================================================

#ifdef  MODE_GRID_TIE_INVERTER
    #define CANSendID 0x302
    #define CANReceiveID 0x303
#else
    #define CANSendID 0x202
    #define CANReceiveID 0x203
#endif

//==============================================================================

#if defined (MODE_PFC) && defined (MODE_GRID_TIE_INVERTER)
    #error "MODE SETUP ERROR: #defines in Totempole_Application_Setup.h is wrong"
#endif
#if defined (MODE_PFC) && defined (MODE_INTERLEAVED)
    #error "MODE SETUP ERROR: #defines in Totempole_Application_Setup.h is wrong"
#endif
#if defined (MODE_INTERLEAVED) && defined (MODE_GRID_TIE_INVERTER)
    #error "MODE SETUP ERROR: #defines in Totempole_Application_Setup.h is wrong"
#endif

//==============================================================================
//<<< EXPERTS ONLY 
/******************************************************************************
 * @ingroup setup
 * @fn      
 * @brief   special mode to measure current loop
 * @details 
 *          setup to drive PFC with DC input voltage and only 
 *          current loop running
 *          measure current loop frequency response with tool like 
 *          Bode100 from Omicron
 *          load is typ. running in Constant Voltage to limit Vout
 *          see dedicated chapter/video for setup
 *******************************************************************************/
//#define CURRENT_LOOP_BODE_MEASUREMENT
        
//==============================================================================
//==============================================================================

#endif	/* DRV_USERSETUP_TOTEMPOLEAPPLICATION_H */

