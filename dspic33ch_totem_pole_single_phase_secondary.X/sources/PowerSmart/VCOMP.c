/* *********************************************************************************
 * PowerSmart Digital Control Library Designer, Version 1.9.15.709
 * *********************************************************************************
 * 2p2z controller function declarations and compensation filter coefficients
 * derived for following operating conditions:
 * *********************************************************************************
 *
 *  Controller Type:    2P2Z - Basic Current Mode Compensator
 *  Sampling Frequency: 100 Hz
 *  Fixed Point Format: Q15
 *  Scaling Mode:       2 - Single Bit-Shift with Output Factor Scaling
 *  Input Gain:         1
 *
 * *********************************************************************************
 * CGS Version:         3.0.11
 * CGS Date:            01/06/2022
 * *********************************************************************************
 * User:                M91114
 * Date/Time:           06/30/2023 09:45:11
 * ********************************************************************************/

#include "VCOMP.h"

/* *********************************************************************************
 * Data Arrays:
 * ============
 *
 * This source file declares the default parameters of the z-domain compensation
 * filter. The NPNZ16b_s data structure contains two pointers to A- and B-
 * coefficient arrays and two pointers to control and error history arrays.
 *
 * For optimized data processing during DSP computations, these arrays must be
 * located in specific memory locations (X-space for coefficient arrays and
 * Y-space for control and error history arrays).
 *
 * The following declarations are used to define the array data contents, their
 * length and memory location. These declarations are made publicly accessible
 * through extern declarations in header file VCOMP.h
 * ********************************************************************************/

volatile struct VCOMP_CONTROL_LOOP_COEFFICIENTS_s __attribute__((space(xmemory), near)) VCOMP_coefficients; // A/B-Coefficients
volatile uint16_t VCOMP_ACoefficients_size = (sizeof(VCOMP_coefficients.ACoefficients)/sizeof(VCOMP_coefficients.ACoefficients[0])); // A-coefficient array size
volatile uint16_t VCOMP_BCoefficients_size = (sizeof(VCOMP_coefficients.BCoefficients)/sizeof(VCOMP_coefficients.BCoefficients[0])); // B-coefficient array size

volatile struct VCOMP_CONTROL_LOOP_HISTORIES_s __attribute__((space(ymemory), far)) VCOMP_histories; // Control/Error Histories
volatile uint16_t VCOMP_ControlHistory_size = (sizeof(VCOMP_histories.ControlHistory)/sizeof(VCOMP_histories.ControlHistory[0])); // Control history array size
volatile uint16_t VCOMP_ErrorHistory_size = (sizeof(VCOMP_histories.ErrorHistory)/sizeof(VCOMP_histories.ErrorHistory[0])); // Error history array size

/* *********************************************************************************
 * Pole&Zero Placement:
 * *********************************************************************************
 *
 *    fP0:    150 Hz
 *    fP1:    250 Hz
 *    fZ1:    3.25 Hz
 *
 * *********************************************************************************
 * Filter Coefficients and Parameters:
 * ********************************************************************************/

volatile int32_t VCOMP_ACoefficients [2] =
{
    0x000000A5, // Coefficient A1 will be multiplied with controller output u(n-1)
    0x00000233  // Coefficient A2 will be multiplied with controller output u(n-2)
};

volatile int32_t VCOMP_BCoefficients [3] =
{
    0x00007FFF, // Coefficient B0 will be multiplied with error input e(n-0)
    0x000017B8, // Coefficient B1 will be multiplied with error input e(n-1)
    0x000097B9  // Coefficient B2 will be multiplied with error input e(n-2)
};

// Coefficient normalization factors
volatile int16_t VCOMP_pre_shift = 3;             // Bit-shift value used to perform input value normalization
volatile int16_t VCOMP_post_shift_A = -6;         // Bit-shift value A used to perform control output value backward normalization
volatile int16_t VCOMP_post_shift_B = 0;          // Bit-shift value B used to perform control output value backward normalization
volatile fractional VCOMP_post_scaler = 0x5A44;   // Q15 fractional factor used to perform control output value backward normalization


// User-defined NPNZ16b_s controller data object
volatile struct NPNZ16b_s VCOMP;                  // user-controller data object

/* ********************************************************************************/

/* *********************************************************************************
 * Controller Initialization:
 * ==========================
  *
 * Public controller initialization function loading known default settings
 * into the NPNZ16b data structure.
 *
 * ********************************************************************************/

volatile uint16_t VCOMP_Initialize(volatile struct NPNZ16b_s* controller)
{
    volatile uint16_t i=0;

    // Initialize controller data structure at runtime with pre-defined default values
    controller->status.value = NPNZ_STATUS_CLEAR; // clear all status flag bits (will turn off execution))

    controller->Filter.ptrACoefficients = &VCOMP_coefficients.ACoefficients[0]; // initialize pointer to A-coefficients array
    controller->Filter.ptrBCoefficients = &VCOMP_coefficients.BCoefficients[0]; // initialize pointer to B-coefficients array
    controller->Filter.ptrControlHistory = &VCOMP_histories.ControlHistory[0]; // initialize pointer to control history array
    controller->Filter.ptrErrorHistory = &VCOMP_histories.ErrorHistory[0]; // initialize pointer to error history array
    controller->Filter.normPostShiftA = VCOMP_post_shift_A; // initialize A-coefficients/single bit-shift scaler
    controller->Filter.normPostShiftB = VCOMP_post_shift_B; // initialize B-coefficients/dual/post scale factor bit-shift scaler
    controller->Filter.normPostScaler = VCOMP_post_scaler; // initialize control output value normalization scaling factor
    controller->Filter.normPreShift = VCOMP_pre_shift; // initialize A-coefficients/single bit-shift scaler
    
    controller->Filter.ACoefficientsArraySize = VCOMP_ACoefficients_size; // initialize A-coefficients array size
    controller->Filter.BCoefficientsArraySize = VCOMP_BCoefficients_size; // initialize B-coefficients array size
    controller->Filter.ControlHistoryArraySize = VCOMP_ControlHistory_size; // initialize control history array size
    controller->Filter.ErrorHistoryArraySize = VCOMP_ErrorHistory_size; // initialize error history array size

    // Load default set of A-coefficients from user RAM into controller A-array located in X-Space
    for(i=0; i<controller->Filter.ACoefficientsArraySize; i++)
    {
        VCOMP_coefficients.ACoefficients[i] = VCOMP_ACoefficients[i]; // Load coefficient A(n) value into VCOMP coefficient(i) data space
    }

    // Load default set of B-coefficients from user RAM into controller B-array located in X-Space
    for(i=0; i<controller->Filter.BCoefficientsArraySize; i++)
    {
        VCOMP_coefficients.BCoefficients[i] = VCOMP_BCoefficients[i]; // Load coefficient B(n) value into VCOMP coefficient(i) data space
    }

    // Clear error and control histories of the 2P2Z controller
    VCOMP_Reset(&VCOMP);
    
    return(1);
}


//**********************************************************************************
// Download latest version of this tool here:
//      https://www.microchip.com/powersmart
//**********************************************************************************

