
/**
 * Power Controller Typedef File
 * 
 * @file      dev_TPBLPFC_typedef.h
 * 
 * @ingroup   p-controller-type-TPBLPFC
 * 
 * @brief     This is the typedef file for power controller
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

#ifndef _DEV_TPBLPFC_TYPEDEF_H
#define	_DEV_TPBLPFC_TYPEDEF_H

#include <xc.h>
#include <stdint.h>

//------------------------------------------------------------------------------
// @brief power controller states
//------------------------------------------------------------------------------

typedef enum
{
  PCS_INIT                  = 1,    // Init peripherals that are involved
  PCS_WAIT_IF_FAULT_ACTIVE  = 2,    // goes on if no fault is active
  PCS_STANDBY               = 3,    //
  PCS_VACOK                 = 4,    // 
  PCS_VOUT_PRELOAD          = 5,    //
  PCS_RELAYON               = 6,    // 
  PCS_DELAY_AFTER_RELAYON   = 7,    //        
  PCS_START_CONTROL         = 8,    //        
  PCS_SOFT_START            = 9,    // 
  PCS_UP_AND_RUNNING        = 10,   // 
  PCS_STALL_DEBUG           = 11    // for debug purposes
} PWR_CTRL_STATE_e;

extern PWR_CTRL_STATE_e pwr_ctrl_state;

//------------------------------------------------------------------------------
// @brief Analog values
//------------------------------------------------------------------------------

struct ANALOG_VALUES_s
{
  volatile int16_t  Raw;
  volatile int16_t  Rectified;
  volatile uint16_t Rectified_Shift;
  volatile int16_t  RectifiedSum;
  volatile uint16_t Filtered;
  volatile int16_t  Vin_div_Averaged2;
  volatile int16_t  Vin_div_Averaged2_Freezed;
  volatile uint16_t FilterCounter;
  volatile int16_t  Offset;
  volatile uint16_t PreviousValue;
  volatile uint16_t Max_Peak;
};
typedef struct ANALOG_VALUES_s ANALOG_VALUES_t;

//------------------------------------------------------------------------------
// @brief Reference set points for Vout
//------------------------------------------------------------------------------

struct CONTROL_REFERENCE_VALUES_s
{
  volatile  uint16_t Reference_Internal;
  volatile  uint16_t Reference_Set;
  volatile  uint16_t Voltage_Loop_Output;  
  volatile  uint16_t VOUT_dV_dt_Hystresis_High;
  volatile  uint16_t VOUT_dV_dt_Hystresis_Low;
  volatile  uint16_t VOUT_Hystresis_High;
  volatile  uint16_t VOUT_Hystresis_Low;
  
  volatile unsigned  VOUT_dV_dt_Up      : 1;  
  volatile unsigned  VOUT_dV_dt_Active  : 1;           
};
typedef struct CONTROL_REFERENCE_VALUES_s CONTROL_REFERENCE_VALUES_t; 
    
//------------------------------------------------------------------------------
// @brief Values for current control loop 
//------------------------------------------------------------------------------

struct CONTROLLER_VALUES_s
{
  volatile  uint16_t PWM_Counter;           
  volatile  uint16_t Duty_Cycle_Set_Value;
  volatile  uint16_t CCM_Counter;
  volatile  uint16_t DFF_Value;
  volatile  uint32_t IAC_Reference_l;
  volatile  uint16_t IAC_Reference;
  volatile  uint16_t BurstModeCycleCounter;
  volatile  uint16_t BurstModeCyclesOff;
  volatile  uint16_t BurstModeCyclesOn;
  volatile  uint16_t ZeroCrossCounter;
  volatile  uint16_t VariableGainCounter;
  
  volatile  unsigned Control_Freeze     : 1;
};
typedef struct CONTROLLER_VALUES_s CONTROLLER_VALUES_t;

//------------------------------------------------------------------------------
// @brief Power controller status flags
//------------------------------------------------------------------------------

struct CONTROL_STATUS_FLAGS_s
{
  union{
  struct{
    volatile unsigned Soft_Start_Zero_Cross         : 1; // Bit  0: 
    volatile unsigned VAC_Polarity_last             : 1; // Bit  1:
    volatile unsigned Relay                         : 1; // Bit  2:
    volatile unsigned Stop                          : 1; // Bit  3:
    volatile unsigned Fault                         : 1; // Bit  4:
    volatile unsigned VAC_Polarity_Changed          : 1; // Bit  5:
    volatile unsigned VAC_Polarity_Changed_last     : 1; // Bit  6: 
    volatile unsigned CCM                           : 1; // Bit  7: 
    volatile unsigned BurstModeDetected             : 1; // Bit  8:
    volatile unsigned BurstModeLatched              : 1; // Bit  9:
    volatile unsigned Reference_Changed             : 1; // needed to decide if ref change after startup or during run 
    volatile unsigned VariableGain                  : 1; // Gain changes around ZC 
    
  } __attribute__((packed)) bits; 
  volatile uint16_t value; ///< buffer for 16-bit word read/write operations

  };
};  
typedef struct CONTROL_STATUS_FLAGS_s CONTROL_STATUS_FLAGS_t;

//------------------------------------------------------------------------------
// @brief Values AC Acquisition bits coming direct from AC Acquisition board via SPI and MSIB
//------------------------------------------------------------------------------

struct ACCONTROL_PHASE_STATUS_FLAGS_s
{
    union{
    struct{
        volatile unsigned VACdrop                   : 1; // Bit  0:
        volatile unsigned VDC_Input_Voltage         : 1; // Bit  1: 
        volatile unsigned VAC_OK                    : 1; // Bit  2: 
        volatile unsigned Zero_Cross_Range          : 1; // Bit  3: 
        volatile unsigned VAC_Polarity              : 1; // Bit  4: 
        volatile unsigned nu5                       : 1; // Bit  5:
        volatile unsigned nu6                       : 1; // Bit  6:
        volatile unsigned nu7                       : 1; // Bit  7:
    } __attribute__((packed)) bits; 
    volatile uint16_t value;
    
    };
};
typedef struct ACCONTROL_PHASE_STATUS_FLAGS_s ACCONTROL_PHASE_STATUS_FLAGS_t;

//------------------------------------------------------------------------------
// @brief Values for the individual 3 phase
//------------------------------------------------------------------------------

struct PHASE_VALUES_s
{
  ANALOG_VALUES_t Phase_Voltage;  
  ANALOG_VALUES_t Phase_Current;  
  CONTROL_STATUS_FLAGS_t Control_Status_Flags;
  CONTROLLER_VALUES_t Controller_Values;
  ACCONTROL_PHASE_STATUS_FLAGS_t ACcontrol_Status_Flags_perPhase;
};
typedef struct PHASE_VALUES_s PHASE_VALUES_t;
extern PHASE_VALUES_t Phase_Values_PH1, Phase_Values_PH2, AC_N;

//------------------------------------------------------------------------------
// @brief Values for the outer voltage loop
//------------------------------------------------------------------------------

struct VMC_VALUES_s
{
  uint16_t VBode;
  ANALOG_VALUES_t Vout;  
  ANALOG_VALUES_t Vout_MidPoint;   
  CONTROL_REFERENCE_VALUES_t Reference;
};
typedef struct VMC_VALUES_s VMC_VALUES_t;
extern VMC_VALUES_t Vout_Control;

//------------------------------------------------------------------------------
// @brief common PFC mode bits
//------------------------------------------------------------------------------

struct PFC_FLAGS_s
{
  union{
  struct{
    volatile unsigned Run                           : 1; // Bit  0 
    volatile unsigned Stop                          : 1; // Bit  1 
    volatile unsigned Fault                         : 1; // Bit  2 
    volatile unsigned OC_PH1                        : 1; // Bit  3
    volatile unsigned OC_PH2                        : 1; // Bit  4
    volatile unsigned OC_PH3                        : 1; // Bit  5
    volatile unsigned OV_Vout                       : 1; // Bit  6
    //Run Modes
    volatile unsigned AC_PFC_1_PH                   : 1; // Bit  7
    volatile unsigned AC_AVGCM_no_VMC_1_PH          : 1; // Bit  8 
    volatile unsigned DC_AVGCM_no_VMC_1_PH          : 1; // Bit  9 
    volatile unsigned AC_AVGCM_no_VMC_1_PH_reverse  : 1; // Bit  10 
    volatile unsigned AC_PFC_2PH_1_PH               : 1; // Bit  11     
    volatile unsigned AGOn                          : 1; // Bit  12      
    volatile unsigned                               : 1; // Bit  13
    volatile unsigned                               : 1; // Bit  14
    volatile unsigned                               : 1; // Bit  15
    
  } __attribute__((packed)) bits; 
  volatile uint16_t value; // buffer for 16-bit word read/write operations
  };
};  
typedef struct PFC_FLAGS_s PFC_FLAGS_t;
extern PFC_FLAGS_t PFC_Flags;

//==============================================================================
// @brief extern declarations
//==============================================================================

typedef struct
{
  uint16_t Data[10];
  volatile unsigned DataReceivedOK : 1;
} Primary_Data_Handling;
extern Primary_Data_Handling primary_vac_values;

//==============================================================================
// @brief function prototypes
//==============================================================================
extern uint16_t OpenLoopStartupDuration;
extern uint16_t Adaptive_Gain_Factor;

void Init_pwr_ctrl (void);

//------------------------------------------------------------------------------
// END
//------------------------------------------------------------------------------
#endif	/* XC_HEADER_TEMPLATE_H */