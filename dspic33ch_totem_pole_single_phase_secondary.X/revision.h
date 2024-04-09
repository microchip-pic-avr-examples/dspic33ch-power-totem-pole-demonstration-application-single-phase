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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef REVISION_H
#define	REVISION_H

#define MAJOR       '1'
#define MINOR       '1'
#define PATCH       '2'


/*******************************************************************************
 * @fn      revision history 1.1.0
 * @details dedicated single phase version
 *          1ph-PFC, 1ph-GTI, 1ph-interleave 2 legs running PWM in parallel
 *          Burst:                  all modes same numbers
 *          Adaptive Gain:          no Vin consideration
 *          DFF:                    all modes
 *          Adaptive Step Response: only load release
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 1.1.2
 * @details dedicated single phase version
 *          1ph-PFC, 1ph-GTI, 1ph-interleave 2 legs running PWM in parallel
 *          Burst:                  all modes same numbers
 *          Adaptive Gain:          no Vin consideration
 *          DFF:                    all modes
 *          Adaptive Step Response: only load release
 *          startup after ZC modified:
 *              extra check before write DC
 *              reset DC to 0 before startup after ZC 
 *          protocolB increased to 10 words to get Vin avg          
 *******************************************************************************/

#endif	/* REVISION_H */

