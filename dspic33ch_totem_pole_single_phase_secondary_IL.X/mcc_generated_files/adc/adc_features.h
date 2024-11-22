/**
 * ADC Generated Feature Header File
 * 
 * @file      adc_features.h
 *            
 * @ingroup   adcdriver
 *            
 * @brief     This is the generated module feature header file for ADC driver
 *            This file provides module feature list available on the selected device. 
 *            The macros defined in this file provides the flexibility to easily migrate 
 *            the user application to other device which might have varied feature list. 
 *            The content in this file is strictly "read only" and should not be altered.
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


#ifndef ADC_FEATURES
#define ADC_FEATURES

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
        
/*******************************************************************************
********************************************************************************
******************************************************************************** 
                            ********IMPORTANT********

This file defines the ADC module feature C-macros available 
on the selected device. The macros defined in this file provides the 
flexibility to easily migrate the user application to other device which might 
have varied feature list.

The file has to be manually included in main.c, if the user intends to migrate the
application to another device which might have varied feature list.
  
  
 ***************The content in this file is strictly "read only" and should 
 not be altered. Failing to do so, the migration is not guaranteed.*************

********************************************************************************
********************************************************************************         
********************************************************************************/

/*******************************************************************************
            Macros defined for features supported in the device
*******************************************************************************/

/** 
 @ingroup  adcdriver
 @brief    This macro defines the ADC core power enable functionality

  <b>APIs Supported:</b><br>
   void ADCx_CorePowerEnable (enum ADC_CHANNEL channel);<br>
   void ADC_SharedCorePowerEnable (void);<br>
   
   x denotes instance of ADC in ADCx, channel denotes dedicated Core. 
   Refer to device specific datasheet to check number of cores and ADC module instance.
   Refer driver header file for detailed description of the APIs.
  
*/
#define ADC_POWER_ENABLE_FEATURE_AVAILABLE  1

/** 
 @ingroup  adcdriver
 @brief    This macro defines the macro associated with ADC core calibration

  <b>APIs Supported:</b><br>
   void ADCx_CoreCalibration (enum ADC_CHANNEL channel);<br>
   void ADC_SharedCoreCalibration (void);<br>

   x denotes instance of ADC in ADCx, channel denotes dedicated Core. 
   Refer to device specific datasheet to check number of cores and ADC module instance.
   Refer driver header file for detailed description of the APIs.
  
*/
#define ADC_CALIBRATION_FEATURE_AVAILABLE  1

/** 
 @ingroup  adcdriver
 @brief    Defines the macro associated with ADC indvidual channel interrupts

  <b>APIs Supported:</b><br>
   void ADCx_IndividualChannelInterruptEnable (enum ADC_CHANNEL channel);<br>
   void ADCx_IndividualChannelInterruptDisable (enum ADC_CHANNEL channel);<br>
   void ADCx_IndividualChannelInterruptFlagClear (enum ADC_CHANNEL channel);<br>
   
   x denotes instance of ADC, channel denotes dedicated ADC channel. 
   Refer to device specific datasheet to check number of cores and ADC module instance.
   Refer driver header file for detailed description of the APIs.
  
*/
#define ADC_INDIVIDUAL_CHANNEL_INTERRUPT_FEATURE_AVAILABLE  1

/** 
 @ingroup  adcdriver
 @brief    Defines the macro associated with ADC comparator feature

  <b>APIs Supported:</b><br>
   void ADCx_ComparatorEnable(void);<br>
   void ADCx_ComparatorDisable(void);<br>
   void ADCx_ComparatorLowThresholdSet(uint16_t threshold );<br>
   void ADCx_ComparatorHighThresholdSet(uint16_t threshold );<br>
   uint16_t ADCx_ComparatorEventStatusGet(void);<br>
   uint16_t ADCx_ComparatorEventChannelGet(void);<br>

   x denotes instance of ADC in ADCx.
   Refer to device specific datasheet to check number of comparators and ADC module instance.
   Refer driver header file for detailed description of the APIs.
  
*/
#define ADC_COMPARATOR_FEATURE_AVAILABLE   1

/*******************************************************************************
            Macros defined for features not supported in the device
*******************************************************************************/

/**<None*/

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //ADC_MODULE_FEATURES