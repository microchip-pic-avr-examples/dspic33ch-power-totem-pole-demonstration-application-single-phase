/**
 * ADC Generated Driver Types Header File
 * 
 * @file      adc_types.h
 *            
 * @ingroup   adcdriver
 *            
 * @brief     This is the generated driver types header file for the ADC driver
 *            
 * @skipline @version   Firmware Driver Version 1.4.4
 *
 * @skipline @version   PLIB Version 2.4.3
 *            
 * @skipline  Device : dsPIC33CH512MP506S1
*/

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

#ifndef ADC_TYPES_H
#define ADC_TYPES_H

// Section: Included Files

// Section: Data Type Definitions


/**
 @ingroup  adcdriver
 @enum     ADC_CHANNEL
 @brief    Defines the ADC channles that are selected from the MCC Melody 
           User Interface for the ADC conversions.
 @note     The enum list in the Help document might be just a reference to show 
           the analog channel list. Generated enum list is based on the configuration 
           done by user in the MCC Melody user interface.
*/
enum ADC_CHANNEL
{
    FB_Vout,    /**<Channel Name:S1AN10   Assigned to:Shared Channel*/
    FB_AC_N,    /**<Channel Name:S1AN15   Assigned to:Shared Channel*/
    IL1_F,    /**<Channel Name:S1AN0   Assigned to:Dedicated Core0*/
    IL2_F,    /**<Channel Name:S1AN1   Assigned to:Dedicated Core1*/
    MAX_CHANNELS = 4,    /**< MAX_CHANNELS will be removed in future MCC releases. Use ADC_MAX_CHANNELS instead */
    ADC_MAX_CHANNELS = 4    /**< Maximum number of channels configured by user in MCC Melody User Interface */
};


/**
 @ingroup  adcdriver
 @enum     ADC_DEDICATED_CORE
 @brief    Defines the ADC cores that are 
           available for the module to use.
 @note     The enum list in the Help document might be just a reference to to show 
           the dedicated core list. Generated enum list is based on the configuration 
           done by user in the MCC Melody user interface.
*/
enum ADC_DEDICATED_CORE
{
    CORE_0 = 0,    /**<CORE_0 will be removed in future MCC releases. Use ADC_CORE_0 instead. */
    ADC_CORE_0 = 0,    /**<Core name:ADC_CORE_0*/
    CORE_1 = 1,    /**<CORE_1 will be removed in future MCC releases. Use ADC_CORE_1 instead. */
    ADC_CORE_1 = 1,    /**<Core name:ADC_CORE_1*/
    MAX_DEDICATED_CORES = 2,    /**< MAX_DEDICATED_CORES will be removed in future MCC releases. Use ADC_MAX_DEDICATED_CORES instead */
    ADC_MAX_DEDICATED_CORES = 2    /**< Maximum cores configured by user in MCC Melody User Interface */
};

/**
 @ingroup  adcdriver
 @enum     ADC_RESOLUTION_TYPE
 @brief    Defines the supported ADC resolution types.
*/
enum ADC_RESOLUTION_TYPE
{
    ADC_6_BIT_RESOLUTION = 0x0,     /**< ADC Resolution of 6 bit*/
    ADC_8_BIT_RESOLUTION = 0x1,     /**< ADC Resolution of 8 bit*/
    ADC_10_BIT_RESOLUTION = 0x2,     /**< ADC Resolution of 10 bit*/
    ADC_12_BIT_RESOLUTION = 0x3,     /**< ADC Resolution of 12 bit*/
};

/**
 @ingroup  adcdriver
 @enum     ADC_CMP
 @brief    Defines the ADC comparators that are 
           available for the module to use.
*/
enum ADC_CMP
{
    MAX_CMPS = 0,    /**< MAX_CMPS will be removed in future MCC releases. Use ADC_MAX_CMPS instead */
    ADC_MAX_CMPS = 0    /**< Maximum Comparators configured by user in MCC Melody User Interface */
};

/**
 @ingroup  adcdriver
 @enum     ADC_PWM_INSTANCE
 @brief    Defines the ADC PWM trigger sources that are 
           available for the module to use.
 @note     Refer \ref PWM_GENERATOR enum for mapping between custom name and instance 
*/
enum ADC_PWM_INSTANCE
{
    ADC_PWM_HS_NOT_AVAILABLE,     /**< PWM HS not available */
};

/**
 @ingroup  adcdriver
 @enum     ADC_PWM_TRIGGERS
 @brief    Defines the PWM triggers that are available in each individual PWM.
*/
enum ADC_PWM_TRIGGERS
{
    ADC_PWM_TRIGGERS_NOT_AVAILABLE,     /**< PWM HS triggers available */
};

#endif  //ADC_TYPES_H
