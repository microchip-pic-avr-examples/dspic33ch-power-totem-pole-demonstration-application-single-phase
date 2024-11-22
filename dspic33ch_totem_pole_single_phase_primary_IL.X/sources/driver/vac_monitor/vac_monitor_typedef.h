/**
 * VAC Type defintition file
 * 
 * @file      vac_monitor_typedef.h
 * 
 * @ingroup   pri-layer-VAC
 * 
 * @brief     VAC Types define
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
#ifndef VAC_MONITOR_DEVICE_DRIVER_TYPEDEF_H
#define	VAC_MONITOR_DEVICE_DRIVER_TYPEDEF_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include <stdint.h> // include standard intger data type header file
#include <stdbool.h> // include standard boolean data type header file
#include <dsp.h> // include standard DSP data type declarations header files

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_STATUS_s
 * @extends PARENT_STRUCTURE
 * @brief ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DETAILED_DESCRIPTION
 **********************************************************************************/
struct VACM_STATUS_s
{
    union {
        struct{                      
            bool ac_drop          : 1; ///< Bit #0: flag indicating that an ac drop may have occurred
            bool dc_mode          : 1; ///< Bit #1: 1 when Vin = DC, 0 when Vin = AC
            bool ac_ok            : 1; ///< Bit #2: flag indicating the status of the line voltage             
            bool zero_cross       : 1; ///< Bit #3: flag indicating that we are in the zero cross region
            bool polarity_pos     : 1; ///< Bit #4: flag = 1 when polarity is positive, 0 when negative
            bool end_of_cycle     : 1; ///< Bit #5: flag indicating that the end of cycle has just occured            
            bool slope_pos        : 1; ///< Bit #6: flag = 1 when ac line slope is positive, 0 when negative            
                       
            unsigned              : 1; ///< Bit #7: unused
            unsigned              : 1; ///< Bit #8: unused
            unsigned              : 1; ///< Bit #9: unused
            unsigned              : 1; ///< Bit #10: unused
            unsigned              : 1; ///< Bit #11: unused
            unsigned              : 1; ///< Bit #12: unused
            unsigned              : 1; ///< Bit #13: unused
            unsigned              : 1; ///< Bit #14: unused
            unsigned              : 1; ///< Bit #15: unused
        } __attribute__((packed)) bits; // data structure for single bit addressing operations

        volatile uint16_t value; // buffer for 16-bit word read/write operations
    };
};   
typedef struct VACM_STATUS_s VACM_STATUS_t; ///< Status word of the AC voltage monitor flags object

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_FAULT_s
 * @extends PARENT_STRUCTURE
 * @brief ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DETAILED_DESCRIPTION
 **********************************************************************************/
struct VACM_FAULT_s
{
    union {
        struct{
            bool ov         : 1; ///< Bit #0: flag indicating that magnitude of line voltage < UV threshold
            bool uv         : 1; ///< Bit #1: flag indicating that magnitude of line voltage > OV threshold
            bool fmin       : 1; ///< Bit #2: flag indicating that frequency of line voltage < min
            bool fmax       : 1; ///< Bit #3: flag indicating that frequency of line voltage > max
            bool zc_timeout : 1; ///< Bit #4: flag indicating that have spent too long in zero cross region   
          
            unsigned        : 1; ///< Bit #5: unused
            unsigned        : 1; ///< Bit #6: unused
            unsigned        : 1; ///< Bit #7: unused
            unsigned        : 1; ///< Bit #8: unused
            unsigned        : 1; ///< Bit #9: unused
            unsigned        : 1; ///< Bit #10: unused
            unsigned        : 1; ///< Bit #11: unused            
            unsigned        : 1; ///< Bit #12: unused
            unsigned        : 1; ///< Bit #13: unused
            unsigned        : 1; ///< Bit #14: unused
            unsigned        : 1; ///< Bit #15: unused
        } __attribute__((packed)) bits; // data structure for single bit addressing operations

        uint16_t value; // buffer for 16-bit word read/write operations
    };
};   
typedef struct VACM_FAULT_s VACM_FAULT_t; ///< Fault word of the AC voltage monitor flags object

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_VIN_s
 * @extends PARENT_STRUCTURE
 * @brief ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DETAILED_DESCRIPTION
 **********************************************************************************/
struct VACM_VIN_s
{
    uint16_t* ptr_adcbuf;    ///< Pointer to register or variable where the value is read from (e.g. ADCBUFx) or written to (e.g. PGxDC) 
    uint16_t raw;            ///> raw ADC reading of input voltage, before offset removed and rectified
    uint16_t raw_prev;       ///> previous reading of raw input voltage 
    uint16_t rectified;      ///> rectified input voltage  
    int16_t*  ptr_offset;    ///> pointer to offset upon which sensed input voltage sits before it is digitized by the ADC 
    uint16_t avg;            ///> average of the rectified input voltage over 1/2 line cycle
    uint16_t avg_prev;       ///> previous reading of average (over 1/2 line cycle)
    uint16_t avg_sqrd;       ///> square of the average input voltage 
    uint16_t vloop_ff;       ///> voltage loop feed-forward term (Vin/Vavg^2)
};
typedef struct VACM_VIN_s VACM_VIN_t;

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_RUNTIME_BUFFER_s
 * @extends PARENT_STRUCTURE
 * @brief ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DETAILED_DESCRIPTION
 **********************************************************************************/
struct VACM_AVGCALC_BUFFER_s
{
    uint16_t counter;       ///> counter used for computing the average of the rectified input voltage 
    uint16_t counter_reset; ///> when avg_counter reaches this value, compute average
    uint32_t acc;           ///> accumulator used for calculating the average of the rectified input voltage
};
typedef struct VACM_AVGCALC_BUFFER_s VACM_AVGCALC_BUFFER_t;

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_TIMEBASE_s
 * @extends PARENT_STRUCTURE
 * @brief ADD_SHORT_DESCRIPTION
 * @details
 *  ADD_DETAILED_DESCRIPTION
 **********************************************************************************/
struct VACM_TIMEBASE_s
{
    uint16_t half_cycle;  ///> measurement of half line cycle in IRQ ticks
    uint16_t half_cycle_timer;  ///> used to time duration of half cycle
    uint16_t startup_counter; ///> used to count periods at startup
    uint16_t zero_cross_timer; ///> used to time duration of zero cross 
    uint16_t ac_drop_timer;   ///> used to time duration of AC drop event
    uint16_t vin_present_counter; ///> used to confirm that some input voltage is present
    uint16_t dc_present_counter;  ///> TODO: add comment
    uint16_t dc_loss_counter;  ///> TODO: add comment
};
typedef struct VACM_TIMEBASE_s VACM_TIMEBASE_t;


/****************************************************************************************************
 * @ingroup pri-layer-VAC
 * @enum    VACM_STATES_e
 * @brief   Enumeration of state machine operating states 
 * @extends VACM_MONITOR_e
 **************************************************************************************************** */
enum VACM_STATES_e 
{       
    VACM_STATE_STANDBY        = 0x00, ///< state #0:  wait for launch trigger
    VACM_STATE_DCDETECT       = 0x01, ///< state #1:  check if input votlage is DC or AC
    VACM_STATE_DCMODE         = 0x02, ///< state #2:  catch-up state in DC mode, bypassing any further AC analysis of the input voltage
    VACM_STATE_WAIT_ZC        = 0x03, ///< state #3:  wait for first zero crossing
    VACM_STATE_ACSYNC         = 0x04, ///< state #4:  startup-step synchronization on first few zero crossings after AC detected
    VACM_STATE_ONLINE         = 0x05, ///< state #5:  normal AC input operation
    VACM_STATE_ACDROP         = 0x06  ///< state #6:  AC drop detected    
    
}; ///< Enumeration of state machine operating states 
typedef enum VACM_STATES_e VACM_STATES_t; ///< Enumeration of state machine operating states data type

/***********************************************************************************
 * @ingroup pri-layer-VAC
 * @struct VACM_s
 * @brief  Main AC Monitor data object data type declaration
 * @details
 **********************************************************************************/

struct VACM_s
{
    VACM_STATES_t state;             ///< state machine operating state ID    
    VACM_STATUS_t status;            ///< AC monitor status flags
    VACM_FAULT_t fault;              ///< AC monitor fault flags
    VACM_TIMEBASE_t timebase;        ///< information related to line cycle and zero cross timing
    VACM_VIN_t vin;                  ///< information related to input voltage
    VACM_AVGCALC_BUFFER_t avgcalc_buffer; ///< used for store information needed at runtime for calculation of average of rectified input voltage    
};
typedef struct VACM_s VACM_t;


#endif	/* VAC_MONITOR_DEVICE_DRIVER_TYPEDEF_H */