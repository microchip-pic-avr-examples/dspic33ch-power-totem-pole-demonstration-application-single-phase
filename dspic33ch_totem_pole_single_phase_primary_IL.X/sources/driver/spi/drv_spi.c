/**
 * SPI Driver Source File
 * 
 * @file      drv_spi.c
 * 
 * @ingroup   pri-layer-SPI
 * 
 * @brief     This is the driver header file for spi builds upon mcc
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
#include <p33CH512MP506.h>
#include <libpic30.h>

#include "drv_spi.h"
#include "driver/can/drv_can.h"

// MCC header files
#include "system/pins.h"
#include "spi_client/spi1.h"
#include "dma/dma.h"
#include "timer/timer_interface.h"
#include "timer/sccp1.h"
#include "timer/sccp2.h"

#include "system/pins.h"

//------------------------------------------------------------------------------
// local typedefs
//------------------------------------------------------------------------------
typedef struct
{
  bool      connected;
  uint16_t  data[4];
  bool      checksum_ok;
  int16_t   timeout_counter;
} DRV_SPI_t;


//------------------------------------------------------------------------------
// local variable definitions
//------------------------------------------------------------------------------
static DRV_SPI_t selfSpi __attribute__((address(0x1800))); // set fixed address for DMA

//------------------------------------------------------------------------------
// local function declarations
//------------------------------------------------------------------------------
static void SPI_Synchronize(void); // local

//------------------------------------------------------------------------------
// API function definitions
//------------------------------------------------------------------------------

/*******************************************************************************
 * @fn	Drv_SPI_Reset_Timeout_Counter()
 * @brief   reset the timeout counter
 * @param value to set counter to
 * @return  None
 * @ingroup   pri-layer-SPI
 * @details
 * This is an API function
 *********************************************************************************/
void Drv_SPI_Reset_Timeout_Counter(uint16_t count)
{
  selfSpi.timeout_counter = count; 
}

/*******************************************************************************
 * @fn	Drv_SPI_get_Connected_Flag()
 * @brief   retrieve the value of the flag "selfSpi.connected"
 * @param	None
 * @return  1 if SPI is already connected
 * @return  0 if SPI not connected
 * @ingroup   pri-layer-SPI
 * @details
 * This is an API function
 *********************************************************************************/
bool Drv_SPI_get_Connected_Flag(void)
{
  return (selfSpi.connected);
}

/*******************************************************************************
 * @fn	Drv_SPI_get_ChecksumOK_Flag()
 * @brief   retrieve the value of the flag "selfSpi.checksum_ok"
 * @param	None
 * @return  1 if last checksum test passed
 * @return  0 if last checksum test failed
 * @ingroup   pri-layer-SPI 
 * @details
 * This is an API function
 *********************************************************************************/
bool Drv_SPI_get_ChecksumOK_Flag(void) // API
{
  return (selfSpi.checksum_ok);
}

/*******************************************************************************
 * @fn	Drv_SPI_get_Adr_Data_Obj()
 * @brief   get address of the SPI data buffer
 * @param	none
 * @return  address of SPI data buffer
 * @ingroup   pri-layer-SPI
 * @details
 * exposes the SPI data buffer to other modules
 *********************************************************************************/
uint16_t* Drv_SPI_get_Adr_Data_Obj(void)
{
  return (selfSpi.data);
}

/*******************************************************************************
 * @fn	void Drv_SPI_Timeout(void)
 * @brief   check for SPI timeout
 * @param	None
 * @return  None
 * @ingroup   pri-layer-SPI
 * @details
 * If counter times out, SPI is deemed "not connected" and we attempt to
 * reconnect using function SPI_Synchronize()
 *********************************************************************************/
void Drv_SPI_Timeout(void) // API
{
  selfSpi.timeout_counter--;
  if (selfSpi.timeout_counter <= 0)
  {
    // this will only get executed if there
    // has been no SPI comms for 1ms
    selfSpi.timeout_counter = 10; // 1ms (10*100us)            
    selfSpi.connected = false; // SPI not connected
    selfSpi.checksum_ok = false; // checksum invalid
    SPI_Synchronize(); // try to sync to SPI bus 
  }
}

/*******************************************************************************
 * @fn	bool Drv_SPI_Checksum(void)
 * @brief   check if computed and transmitted checksum match
 * @param	None
 * @return  None
 * @ingroup   pri-layer-SPI
 * @details
 * 4 x 16 bit words received over SPI
 * calculate additive checksum as sum of words 0 to 2
 * SPI host transmits checksum in word 3, compare calculated and transmitted
 * checksum is preset with 0x0055 to prevent all zeros passing checksum test
 *********************************************************************************/
bool Drv_SPI_Checksum(void) 
{
  bool test_passed = false;

  // checksum preset with 0x0055 so that transmission of all zeros won't 
  // result in a false pass
  uint16_t i;
  uint16_t checksum = 0x0055;
  for (i = 0; i < 3; i++)
  {
    checksum = checksum + selfSpi.data[i];
  }

  // compare calculated checksum against checksum transmitted in word 3
  if (checksum == selfSpi.data[3])
  {
    // calculated and transmitted checksums match
    selfSpi.checksum_ok = true;
    test_passed = true;
  }
  else
  {
    // calculated and transmitted checksums don't match
    selfSpi.checksum_ok = false;
    selfSpi.connected = false;
    
    // reset timeout counter so as we attempt to re-sync to SPI bus 
    // as soon as possible
    // setting 1 means that the next time Drv_SPI_Timeout() is called
    // we will attempt to re-sync to SPI bus, this should be max 100us
    selfSpi.timeout_counter = 1;  
    test_passed = false;
  }
  return (test_passed);
}

/*******************************************************************************
 * @fn	void SPI_Synchronize(void)
 * @brief   synchronize to SPI bus
 * @param	None
 * @return  None
 * @ingroup   pri-layer-SPI
 * @details
 * need to synchronize to the SPI messages coming from the isolated
 * voltage acquisition board.
 * this is necessary since we don't have a SPI_CS line, so we must enable
 * the DMA between SPI messages (that is, while the SPI bus is idle)
 * when SPI bus is idle, SPI_CLK is high continuously.
 * isolated voltage acquisition board is sending messages every 10us.
 * SPI bus should be idle for at least 2us between messages.
 * we can use a timer to measure this
 * 
 * timer SCCP1 is set to expire after 1us
 * timer SCCP2 is set to expire after 40us

 * code works as follows
 * - start both timers
 * - continuously read SPI_CLK
 * - if SPI_CLK = 0: reset SCCP1
 * - if either timer expires, check which one expired
 * - if SCCP1 expired, then SPI_CLK has been high continuously for 1us: 
 *   so can conclude that SPI bus is idle
 * - if SCCP2 expired, then for some reason SPI bus has not been in idle state 
 *   over 40us, so exit and try to detect again in 1ms
 *********************************************************************************/

static void SPI_Synchronize(void) // local
{
  DMA_ChannelDisable(0); // disable DMA channel 0
  SPI1CON1Lbits.SPIEN = 0; // disable SPI
  
  SPI_Timeout_40us_Timer_Start();
  
//  SCCP2_TMR_Start(); // set to time out after 40us
  IFS1bits.CCT2IF = 0; // clear SCCP2 timer overflow flag

  // clear timer (should use MCC API functions but they don't work)
  CCP2TMRL = 0;
  CCP2TMRH = 0;
  
  SPI_BusIdle_1us_Timer_Start();

//  SCCP1_TMR_Start(); // set to time out after 1us
  IFS0bits.CCT1IF = 0; // clear timer overflow flag

  // clear timer (should use MCC API functions but they don't work)
  CCP1TMRL = 0;
  CCP1TMRH = 0;

  do
  {
    // stay here until one of the 2 timers expires
//    if (SCK1IN_GetValue() == 0) 
    if (_RC8 == 0)
    {
      // SPI_CLK is low, so SPI bus is not in idle state. Reset 1us timer
      CCP1TMRL = 0;
      CCP1TMRH = 0;
    }
  } while ((!IFS0bits.CCT1IF) && (!IFS1bits.CCT2IF));

  // check which timer expired first
  if (IFS0bits.CCT1IF)
  {
    // SCCP1 expired, meaning we detected that the SPI bus is idle
    // (SPI_CLK has been high continuously for 1us)
    // since SPI bus is idle, we can now enable the SPI and DMA peripherals
    selfSpi.connected = true;
    SPI1_Initialize();
    SPI1CON1Lbits.SPIEN = 1;

    DMA_Initialize();

    // address to which the DMA sends the incoming data (destination address)
    // is overwritten via DMADST0 register
    DMADST0 = (uint16_t) & selfSpi.data[0];
    DMA_ChannelEnable(0);
  }
  else
  {
    // SCCP2 expired, meaning we didn't detect SPI bus in idle over a 40us period 
    selfSpi.connected = false; // could not establish SPI connection, try again later
  }

  // stop timers and exit
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  SPI_BusIdle_1us_Timer_Stop();
  SPI_Timeout_40us_Timer_Stop();
}

/**
 End of File
 */

