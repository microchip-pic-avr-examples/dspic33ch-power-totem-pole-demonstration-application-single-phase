/**
 * ADC Driver Header File
 * 
 * @file      drv_msi.c
 * 
 * @ingroup   sec-layer-MSI
 * 
 * @brief     This is the driver source file for MSI 
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

#include "drv_msi.h"
#include "../../../mcc_generated_files/main_core/main_core.h"
#include "../power_controller/../../devices/dev_TPBLPFC_typedef.h"
#include "../../../revision.h"
#include "../mcc_generated_files/system/pins.h" //add Masters 24

//------------------------------------------------------------------------------
// local vars and function prototype
//------------------------------------------------------------------------------

uint16_t pMSIAdata[16];

void FIFOData_SendToPrim(void);

/***********************************************************************************
 * @ingroup drv_msi
 * @fn      ReceiveMSIAMessage 
 * @param   void
 * @return  void
 * @brief   M->S protocolA mailbox, receive mailbox A
 * @details 
 *          10 byte data
 *          CAN-FD input data like start/stop, reference and other set values
 **********************************************************************************/

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
      else if (control_byte == 0x60) // Relay On, add Masters 24
      {
        RELAY_SetHigh(); //Relay ON;
      }
      pMSIAdata[0] = 0;
    }
    else if (command_id == 0xEF) // change output voltage setpoint
    {
      Vout_Control.Reference.Reference_Set = pMSIAdata[1];
      if (Vout_Control.Reference.Reference_Set > MAXIMUM_REFERENCE_SET_VALUE) 
        Vout_Control.Reference.Reference_Set = MAXIMUM_REFERENCE_SET_VALUE;

      pMSIAdata[0] = 0;
      pMSIAdata[1] = 0;
    }
    else if (command_id == 0xDC) // adaptive Gain by slider
    {
      Adaptive_Gain_Factor = pMSIAdata[1]; //<<< FTx>>>
    }
    else if (command_id == 0xCD) // Chargepump Delay time (openloop startup in normal mode))
    {
      OpenLoopStartupDuration = pMSIAdata[1];
    }
  }
  
}

/***********************************************************************************
 * @ingroup drv_msi
 * @fn      SendMSIFIFOMessage
 * @param   void
 * @return  void
 * @brief   S->M Fifo
 * @details 
 *          based on TickCounter messages from Fifo are sent
 *          that messages are part of the whole CAN-FD frame send over to GUI
 **********************************************************************************/

void SendMSIFIFOMessage(void)
{
  static uint16_t TickCounter_ = 0;

  if (++TickCounter_ > 50) // >1 is the absolut min which works; 50=5ms
  {
    FIFOData_SendToPrim();
    TickCounter_ = 0;
  }
}

/***********************************************************************************
 * @ingroup drv_msi
 * @fn      FIFOData_SendToPrim
 * @param   void
 * @return  void
 * @brief   S->M Fifo
 * @details 
 *          slave write (master read) Fifo message buffer
 *          and send to primary core for CAN send
 *          Fifo is always 32 word deep
 **********************************************************************************/

void __inline__ FIFOData_SendToPrim(void)
{
  static uint16_t i_;
  
  // duty cycle values for each phase
  // word 0 (CAN word 7)
  SWMRFDATA = Phase_Values_PH1.Controller_Values.Duty_Cycle_Set_Value;  
  // word 1 
//  SWMRFDATA = Phase_Values_PH2.Controller_Values.Duty_Cycle_Set_Value; 
  SWMRFDATA = Phase_Values_PH1.Phase_Voltage.Vin_div_Averaged2; 
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
  SWMRFDATA = Phase_Values_PH1.Phase_Current.Filtered;
  // word 7      
  SWMRFDATA = Vout_Control.Reference.Voltage_Loop_Output;
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

