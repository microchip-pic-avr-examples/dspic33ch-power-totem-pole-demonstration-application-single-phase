/**
 * ADC Generated Driver Interface Header File
 * 
 * @file      adc_interface.h
 *            
 * @defgroup  adcdriver ADC Multicore Driver
 *            
 * @brief     High-Speed, 12-Bit Multiple SARs Analog-to-Digital Converter driver 
 *            using dsPIC MCUs.
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

#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

// Section: Included Files
#include <stdint.h>
#include <stdbool.h>
#include "adc_types.h"
#include "../system/interrupt_types.h"

#ifdef __cplusplus  //Provide C++ Compatibility

    extern "C" {

#endif

// Section: Data Type Definitions
  
/**
 @ingroup  adcdriver
 @struct   ADC_INTERFACE
 @brief    Structure containing the function pointers of ADC driver
*/
struct ADC_INTERFACE
{   
    void (*Initialize)(void);              
    ///< Pointer to ADCx_Initialize e.g. \ref ADC1_Initialize
    
    void (*Deinitialize)(void);            
    ///< Pointer to ADCx_Deinitialize e.g. \ref ADC1_Deinitialize
    
    void (*Enable)(void);                  
    ///< Pointer to ADCx_Enable e.g. \ref ADC1_Enable
    
    void (*Disable)(void);                 
    ///< Pointer to ADCx_Disable e.g. \ref ADC1_Disable
    
    void (*SoftwareTriggerEnable)(void);   
    ///< Pointer to ADCx_SoftwareTriggerEnable e.g. \ref ADC1_SoftwareTriggerEnable
    
    void (*SoftwareTriggerDisable)(void);  
    ///< Pointer to ADCx_SoftwareTriggerDisable e.g. \ref ADC1_SoftwareTriggerDisable
    
    void (*ChannelSelect)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_ChannelSelect e.g. \ref ADC1_ChannelSelect
    
    uint16_t (*ConversionResultGet)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_ConversionResultGet e.g. \ref ADC1_ConversionResultGet
    
    bool (*IsConversionComplete)(enum ADC_CHANNEL channel);     
    ///< Pointer to ADCx_IsConversionComplete e.g. \ref ADC1_IsConversionComplete

    void (*ResolutionSet)(enum ADC_RESOLUTION_TYPE resolution);
    ///< Pointer to ADCx_ResolutionSet e.g. \ref ADC1_ResolutionSet

    void (*InterruptEnable)(void);        
    ///< Pointer to ADCx_InterruptEnable e.g. \ref ADC1_InterruptEnable
    
    void (*InterruptDisable)(void);       
    ///< Pointer to ADCx_InterruptDisable e.g. \ref ADC1_InterruptDisable
    
    void (*InterruptFlagClear)(void);     
    ///< Pointer to ADCx_InterruptFlagClear e.g. \ref ADC1_InterruptFlagClear
    
    void (*InterruptPrioritySet)(uint16_t priorityValue);
    ///< Pointer to ADCx_InterruptPrioritySet e.g. \ref ADC1_InterruptPrioritySet
    
    void (*CommonCallbackRegister)(void (*callback)(void));  
    ///< Pointer to ADCx_CommonCallbackRegister e.g. \ref ADC1_CommonCallbackRegister
    
    void (*Tasks)(void);                                       
    ///< Pointer to ADCx_Tasks e.g. \ref ADC1_Tasks (Supported only in polling mode) 
    
    const struct ADC_MULTICORE *adcMulticoreInterface; 
    ///< Pointer to \ref ADC_MULTICORE
    
};

/**
 @ingroup  adcdriver
 @struct   ADC_MULTICORE
 @brief    Structure containing the function pointers of ADC driver
*/
struct ADC_MULTICORE
{
    void (*ChannelCallbackRegister)(void(*callback)(enum ADC_CHANNEL channel, uint16_t adcVal));       
    ///< Pointer to ADCx_ChannelCallbackRegister e.g. \ref ADC1_ChannelCallbackRegister
    
    void (*ComparatorCallbackRegister)(void(*callback)(enum ADC_CMP comparator));    
    ///< Pointer to ADCx_ComparatorCallbackRegister e.g. \ref ADC1_ComparatorCallbackRegister
    
    void (*ChannelTasks)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_ChannelTasks e.g. \ref ADC1_ChannelTasks
    
    void (*ComparatorTasks)(enum ADC_CMP comparator);  
    ///< Pointer to ADCx_ComparatorTasks e.g. \ref ADC1_ComparatorTasks
    
    void (*IndividualChannelInterruptEnable)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_IndividualChannelInterruptEnable e.g. \ref ADC1_IndividualChannelInterruptEnable
    
    void (*IndividualChannelInterruptDisable)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_IndividualChannelInterruptDisable e.g. \ref ADC1_IndividualChannelInterruptDisable
    
    void (*IndividualChannelInterruptFlagClear)(enum ADC_CHANNEL channel);  
    ///< Pointer to ADCx_IndividualChannelInterruptFlagClear e.g. \ref ADC1_IndividualChannelInterruptFlagClear
    
    void (*IndividualChannelInterruptPrioritySet)(enum ADC_CHANNEL channel,enum INTERRUPT_PRIORITY priorityValue);  
    ///< Pointer to ADCx_IndividualChannelInterruptPrioritySet e.g. \ref ADC1_IndividualChannelInterruptPrioritySet
    
    void (*CorePowerEnable)(enum ADC_DEDICATED_CORE core);  
    ///< Pointer to ADCx_CorePowerEnable e.g. \ref ADC1_CorePowerEnable
    
    void (*SharedCorePowerEnable) (void);  
    ///< Pointer to ADCx_SharedCorePowerEnable e.g. \ref ADC1_SharedCorePowerEnable
    
    
    void (*PWMTriggerSourceSet)(enum ADC_CHANNEL channel, enum ADC_PWM_INSTANCE pwmInstance,  enum ADC_PWM_TRIGGERS triggerNumber);
    ///< Pointer to ADCx_PWMTriggerSourceSet e.g. \ref ADC1_PWMTriggerSourceSet
};

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //ADC_INTERFACE_H
