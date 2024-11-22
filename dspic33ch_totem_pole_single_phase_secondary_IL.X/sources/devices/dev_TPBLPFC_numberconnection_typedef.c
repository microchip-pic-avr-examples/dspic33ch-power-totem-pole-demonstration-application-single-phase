/**
 * Power Controller Typedef File
 * 
 * @file      dev_TPBLPFC_numberconnection_typedef.c
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
 *             .Data[6] = phase1/2/3 status: 
 *             zero_cross_range, VAC_polarity, VACok 
 *             
 *******************************************************************************/

void dev_connect_numbers(void)
{  
  Phase_Values_PH1.Phase_Voltage.Rectified = primary_vac_values.Data[0];
  if(Phase_Values_PH1.Phase_Voltage.Rectified > Phase_Values_PH1.Phase_Voltage.Max_Peak)
      Phase_Values_PH1.Phase_Voltage.Max_Peak = Phase_Values_PH1.Phase_Voltage.Rectified;
  Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2 = primary_vac_values.Data[3];
    
  Phase_Values_PH2.Phase_Voltage.Rectified = primary_vac_values.Data[1];
  Phase_Values_PH2.Phase_Voltage.Vin_div_Averaged2 = primary_vac_values.Data[4];
  
  Phase_Values_PH1.ACcontrol_Status_Flags_perPhase.value = (primary_vac_values.Data[6] & 0x001F);
  Phase_Values_PH2.ACcontrol_Status_Flags_perPhase.value = ((primary_vac_values.Data[6] >> 5) & 0x001F);
  
  Phase_Values_PH1.Phase_Voltage.Filtered = primary_vac_values.Data[7];
  Phase_Values_PH2.Phase_Voltage.Filtered = primary_vac_values.Data[8];

  //Phase 3 is considered at AC acquisition board, measured and transfered
  //but there is no physical connection --> values are not relevant
//  Phase_Values_PH3.Phase_Voltage.Rectified = spi_vac_values.Data[2];
//  Phase_Values_PH3.Phase_Voltage.Vin_div_Averaged2 = spi_vac_values.Data[5];  
//  Phase_Values_PH3.ACcontrol_Status_Flags_perPhase.value = ((spi_vac_values.Data[6]>>10) & 0x001F);

}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

