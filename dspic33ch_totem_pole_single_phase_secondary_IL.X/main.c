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

#define FCY 100000000UL
#include <libpic30.h>

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/timer/tmr1.h"
#include "sources/devices/dev_TPBLPFC_typedef.h"
#include "sources/driver/power_controller/drv_pwrctrl_app_misc_TPBLPFC.h"
#include "main.h"
#include "PFC_frameworkSetup.h"

/***********************************************************************************
 * @ingroup main
 * @fn      main
 * @param   void
 * @return  void
 * @brief   MAIN with 100us scheduler task
 * @details 
 *          100us easy performance optimized scheduler
 *          with peripheral initialization
 *          with power controller state machine to run the PFC in different 
 *          modes based on the configuration
 **********************************************************************************/

int main(void)
{
  SYSTEM_Initialize();

  Init_pwr_ctrl();

  Drv_PwrCtrl_TPBLPFC_Init();

  //<<START>> Timer 1 for 100us task for state machine
  TMR1_Start();

  //<<<<<<<< Mode set by "Totempole_Application_Setup.h" file ------------------ 
#if defined VOLTAGE_LOOP && !defined MODE_INTERLEAVED
  PFC_Flags.bits.AC_PFC_1_PH = 1;
#endif

#if !defined VOLTAGE_LOOP && !defined MODE_GRID_TIE_INVERTER 
  PFC_Flags.bits.DC_AVGCM_no_VMC_1_PH = 1;
#endif

#if defined MODE_GRID_TIE_INVERTER  
  PFC_Flags.bits.AC_AVGCM_no_VMC_1_PH_reverse = 1;
#endif

#if defined MODE_INTERLEAVED 
  PFC_Flags.bits.AC_PFC_2PH_1_PH = 1;
#endif
  //<<<<<<<< Mode set END ------------------------------------------------------

  while (1)
  {
    //<< end 100us scheduler 
    if (IFS0bits.T1IF)
    {
      Tasks_100us();

      IFS0bits.T1IF = 0;

    }
  }    
}