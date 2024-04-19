/*
© [2023] Microchip Technology Inc. and its subsidiaries.

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
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/secondary_core/sec_core1.h"
#include "driver/can/drv_can.h"

#include "dma/dma.h"
#include "driver/dma/drv_dma.h"
#include "driver/spi/drv_spi.h"

#include "main.h"
#include "spi_client/spi1.h"
#include "sources/driver/i2c/i2c2.h"


/*
    Main application
*/

static __inline__ void SPI1_InitializeCustom(void);

int main(void)
{
  SYSTEM_Initialize();
  
  // using non-melody drivers for I2C, so initialize separately
  I2C2_Initialize();      
  
  // ensure that DMA is disabled until we have sync'd with SPI bus
  // not using SPI CS pin, so need to enable SPI and DMA modules when 
  // SPI bus is idle
  DMA_ChannelDisable(0);
  
  // melody based SPI initialization doesn't work, so replace
  // with custom initialization for now
//  SPI1_Deinitialize(); 
//  SPI1_InitializeCustom();
  
  CAN_TX_Obj_Init();

  DRV_DMA_init();
  
   //<<< initialize secondary core
  SEC_CORE1_Program();
  SEC_CORE1_Start();
  
  //<<< initialize FIFO that is used to send data from secondary core to primary core
  MSI1FIFOCSbits.RFEN = 1;
  MSI1FIFOCSbits.WFEN = 0;


  Drv_SPI_Reset_Timeout_Counter(SPI_TIMEOUT_IN_100us_TICKS);

  while(1)
  {
    Tasks_Background();

    // simple 100us task driven by timer 1 (setup in MCC to expire after 100us)
    // this process is polled, not interrupt driven
    if (IFS0bits.T1IF)
    {               
      Tasks_100us();
      IFS0bits.T1IF = 0; // clear timer 1 flag 
    }
  }
}


/**
 @brief     custom (non-Melody supported) initialization of the SPI peripheral      
            note that this function should be invoked after the Melody 
            based peripheral initialization function SPI1_Initialize()
*/

static __inline__ void SPI1_InitializeCustom(void)
{
  SPI1CON1H = 0x00;
  // WLENGTH 0; 
  SPI1CON2L = 0x00;
  // SPIROV disabled; FRMERR disabled; 
  SPI1STATL = 0x00;
  // SPI1BRGL 0; 
  SPI1BRGL = 0x00;
  // SPITBFEN disabled; SPITUREN disabled; FRMERREN disabled; SRMTEN disabled; SPIRBEN disabled; BUSYEN disabled; SPITBEN disabled; SPIROVEN disabled; SPIRBFEN enabled; 
  SPI1IMSKL = 0x01;
  // RXMSK 4; TXWIEN disabled; TXMSK 0; RXWIEN disabled; 
  SPI1IMSKH = 0x400;
  // SPI1URDTL 0; 
  SPI1URDTL = 0x00;
  // SPI1URDTH 0; 
  SPI1URDTH = 0x00;
  // SPIEN disabled; DISSDO disabled; MCLKEN FOSC/2; CKP Idle:High, Active:Low; SSEN disabled; MSTEN Slave; MODE16 enabled; SMP Middle; DISSCK disabled; SPIFE Frame Sync pulse precedes; CKE Idle to Active; MODE32 disabled; SPISIDL disabled; ENHBUF disabled; DISSDI disabled; 
  SPI1CON1L = 0x440;
}

//------------------------------------------------------------------------------
// notes: differences between Melody and MCC
//------------------------------------------------------------------------------
// SPI
// differences 
// SPI1IMSKL: MCC = 0x01, MELODY = default. 
// MCC: SPIRBFEN = 1 (enable interrupts events via SPIRBF bit)

// SPI1IMSKH: MCC = 0x400, MELODY = default. 
// MCC: RXMSK = 4. Works with RXWIEN bit (Receive watermark interrupt enable bit). Not sure if this is required

// SPI1CON1L: MCC = 0x440, MELODY = 0x81
// MCC: MODE16 = 1, CKP = 1
// MELODY: SSEN = 1, ENHBUF = 1





