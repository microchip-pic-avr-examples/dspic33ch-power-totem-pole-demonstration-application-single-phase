/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    dev_TPBLPFC_numberconnection_typedef.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CH512MP506S1
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v5.50
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

/**
  Section: Included Files
 */
#include <p33CH512MP506S1.h>

#include "dev_TPBLPFC_numberconnection_typedef.h"

#include "dev_TPBLPFC_typedef.h"

/*******************************************************************************
 * @fn         dev_connect_numbers
 * @brief      
 * @param      none
 * @return     none
 * @details    reveived message from primary core
 *             .Data[0] = phase1.vin.rectified;
 *             .Data[1] = phase2.vin.rectified;
 *             .Data[2] = phase3.vin.rectified;
 *             .Data[3] = phase1.vin.vloop_ff;
 *             .Data[4] = phase2.vin.vloop_ff;
 *             .Data[5] = phase3.vin.vloop_ff;  
 *             .Data[6] = phase1/2/3 status
 *             
 *******************************************************************************/

void dev_connect_numbers(void)
{  
  Phase_Values_PH1.Phase_Voltage.Rectified = spi_vac_values.Data[0];
  if(Phase_Values_PH1.Phase_Voltage.Rectified > Phase_Values_PH1.Phase_Voltage.Max_Peak)
      Phase_Values_PH1.Phase_Voltage.Max_Peak = Phase_Values_PH1.Phase_Voltage.Rectified;
  Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2 = spi_vac_values.Data[3];
  
  //Phase 2 and 3 are considered at AC acquisition board, measured and transfered
  //but there is no physical connection --> values are not relevant
  
  Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.value = (spi_vac_values.Data[6] & 0x001F);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

