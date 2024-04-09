/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    drv_msi.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CH512MP506S1
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.70
        MPLAB 	          :  MPLAB X v6.00
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

/**
  Section: Included Files
 */
#include <p33CH512MP506S1.h>

#include "../../../mcc_generated_files/main_core/main_core.h"
#include "../power_controller/../../devices/dev_TPBLPFC_typedef.h"
#include "../../../revision.h"
#include "../sources_common/PFC_frameworkSetup.h"

//------------------------------------------------------------------------------

uint16_t pMSIAdata[16];

void FIFOData_SendToPrim(void);

/*******************************************************************************
 * @fn         ReceiveMSIAMessage
 * @brief      
 * @param      none
 * @return     none
 * @details    reveive message from primary core
 *             
 *              
 *             
 *******************************************************************************/

void ReceiveMSIAMessage(void)
{

  if (MAIN_CORE_ProtocolRead(MSI1_ProtocolA, &pMSIAdata[0]))
  {
    uint8_t command_id = (uint8_t) ((pMSIAdata[0] & 0xFF00) >> 8);
    uint8_t control_byte = (uint8_t) (pMSIAdata[0] & 0x00FF);

    if (command_id == 0x55) // turn PFC stage on or off
    {
      if (control_byte == 0x00) // stop PFC
      {
        if (PFC_Flags.bits.Run)
        {
          PFC_Flags.bits.Run = 0;
          PFC_Flags.bits.Stop = 1;
        }
      }
      else if (control_byte == 0x01) // start PFC
      {
        if (!PFC_Flags.bits.Run)
        {
          PFC_Flags.bits.Run = 1;
          PFC_Flags.bits.Stop = 0;
        }
      }
      else if (control_byte == 0xA0) // stop adaptive Gain
      {
        PFC_Flags.bits.AGOn = 0;
      }
      else if (control_byte == 0xA1) // start adaptive Gain
      {
        PFC_Flags.bits.AGOn = 1;
      }
      pMSIAdata[0] = 0;
    }
    else if (command_id == 0xEF) // change output voltage setpoint
    {
      Vout_Control.Reference.Reference_Set = pMSIAdata[1];
      if (Vout_Control.Reference.Reference_Set > 3500) Vout_Control.Reference.Reference_Set = 3500;

      pMSIAdata[0] = 0;
      pMSIAdata[1] = 0;
    }
    else if (command_id == 0xDC) // change offset reference
    {
      Adaptive_Gain_Factor = pMSIAdata[1]; //<<< FTx>>>
    }
  }
  
}

/*******************************************************************************
 * @fn         SendMSIFIFOMessage
 * @brief      
 * @param      none
 * @return     none
 * @details    FIFO based write from sec to prim
 *             
 *              
 *             
 *******************************************************************************/

void SendMSIFIFOMessage(void)
{
  static uint16_t TickCounter_ = 0;

  if (++TickCounter_ > 50) // >1 is the absolut min which works; 50=5ms
  {
    FIFOData_SendToPrim();
    TickCounter_ = 0;
  }
}

/*******************************************************************************
 * @fn         FIFOData_SendToPrim
 * @brief      
 * @param      none
 * @return     none
 * @details    write data to FIFO regs SWMRFDATA 
 *             (Slave Write Master Read Fifo Data Register)
 *              
 *             
 *******************************************************************************/

void __inline__ FIFOData_SendToPrim(void)
{
  static uint16_t i_;
  
  // duty cycle values for each phase
  // word 0 (CAN word 7)
  SWMRFDATA = Phase_Values_PH1.Controller_Values.Duty_Cycle_Set_Value;  
  // word 1 
  SWMRFDATA = Phase_Values_PH2.Controller_Values.Duty_Cycle_Set_Value; 
  // word 2  
//  SWMRFDATA = Phase_Values_PH3.Controller_Values.Duty_Cycle_Set_Value;  
  SWMRFDATA = Adaptive_Gain_Factor;

  // secondary core revision data
  // word 3
  SWMRFDATA = (uint16_t)((MAJOR << 8) + MINOR); 
  // word 4  
  SWMRFDATA = (uint16_t)((PATCH << 8));                    
  // word 5 
  // current loop reference for each phase
  SWMRFDATA = Phase_Values_PH1.Controller_Values.IAC_Reference;      
  // word 6 
//  SWMRFDATA = Phase_Values_PH1.Phase_Current.Rectified_Shift;
  SWMRFDATA = Vout_Control.Vout_MidPoint.Raw; //Vout_Control.Vout_MidPoint.Offset;
  // word 7 
//  SWMRFDATA = Phase_Values_PH3.Controller_Values.IAC_Reference;         
  SWMRFDATA = Vout_Control.Reference.Voltage_Loop_Output;//Vout_Control.VBode;
  // word 8     
  // state variable (de-coded for power board visualizer GUI)
  SWMRFDATA = (1 << ((uint16_t)pwr_ctrl_state-1));                         
  // word 9 
  // status flags
  SWMRFDATA = PFC_Flags.value;                                          
  // word 10
  // voltage loop reference
  SWMRFDATA = Vout_Control.Reference.Reference_Internal;                
  // word 11  
  // measured output voltage (filtered)
  SWMRFDATA = Vout_Control.Vout.Filtered;                               
  
  // FIFO is 32 words wide, all words needs to be filled up to initiate transmission to primary core
  for(i_= 12; i_< 32; i_++)
  {    
    SWMRFDATA = i_;
  }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

/**
 End of File
 */

