/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef REVISION_H
#define	REVISION_H

#define MAJOR       '1'
#define MINOR       '1'
#define PATCH       '0'

/*******************************************************************************
 * @fn      revision history 4.2.3.0
 * @details DC input mode added to test current loop
 *          option also based on #ifdefs in drv_controller_TPBLPFC.h                      
 *                                
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 4.2.2.0
 * @details dedicated compensators for 1/3Ph and LV/HV added
 *          selected via #ifdefs in drv_controller_TPBLPFC.h                      
 *                                
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 4.2.1.0
 * @details ADCON0 ISR optimized: 7.4us with out dff
 *                                8.2us with dff
 *          Single Phase 2 legs IL is based on L1 and L3                      
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 4.2.0.0
 * @details delay after relay on and wait until startup ramp both set to 50ms
 *          ADCON0 ISR optimized
 *          Single Phase 2 legs IL is based on L1 and L2 
 *          tested for 3-phase low voltage to ship             
 *******************************************************************************/
/*******************************************************************************
 * @fn      revision history 9.0.2.0
 * @details Ported to Melody
 *          Usage of framework configurator from artifact as release candidate
 *          Testoption for testing all 3 phases individual as 
 *          single phase option look for <<< only testing >>>       
 *******************************************************************************/

#endif	/* REVISION_H */

