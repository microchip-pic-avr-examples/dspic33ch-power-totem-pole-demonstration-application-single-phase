/**
  PWM Generated Driver Header File
 
  @Company
    Microchip Technology Inc.
 
  @File Name
    pwm.h
 
  @Summary
    This is the generated driver header file for the PWM driver using CCL
 
  @Description
    This file provides APIs declarations for PWM driver.
    Generation Information :
        Product Revision  :  CCL -
        Driver Revision   :  0.1.0
        Device            :  dsPIC33CH512MP506S1
*/

#ifndef PWM_H
#define PWM_H

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

/**
  @Description
    Initializes PWM module, using the given initialization data. 
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
      PWM_Initialize();
    </code>
*/
void PWM_Initialize (void);

#endif //PWM_H

/**
 End of File
*/