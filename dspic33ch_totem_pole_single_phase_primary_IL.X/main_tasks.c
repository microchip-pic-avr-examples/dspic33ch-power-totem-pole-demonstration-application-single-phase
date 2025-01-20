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

#include "system/pins.h"
#include "sources/driver/spi/drv_spi.h"
#include "driver/can/drv_can.h"
#include "driver/i2c/i2c2.h"

/*******************************************************************************
 * @fn	Tasks_Background()
 * @brief background tasks
 * @param	None
 * @return None
 *  
 * @details
 * Background tasks. Only I2C handler run here at present. 
 *********************************************************************************/
void Tasks_Background(void)
{
  // I2C handler, poll interrupt flag
  // note that I2C handler is not interrupt driven              
  if (IFS2bits.SI2C2IF)
  {
    I2C2_SlaveTasks();
    IFS2bits.SI2C2IF = 0;
  }
  
  // clear Watchdog
//  WATCHDOG_TimerClear();
}

/*******************************************************************************
 * @fn	Tasks_100us()
 * @brief tasks run every 100us
 * @param	None
 * @return None
 *  
 * @details
 * 100us tasks. Currently based on polling timer 1. More details in comments
 * in body of this function. 
 *********************************************************************************/
void Tasks_100us(void)
{
  // the DMA is triggered when an SPI message is received
  // an SPI message should be received every 10us
  // so this function will trigger an SPI timeout after 1ms 
  // and will keep doing this until a reliable SPI comms channel 
  // has been established
  Drv_SPI_Timeout();

  // timeout for I2C handler
  // expect I2C message from PIC16 on power board every 50ms. 
  // If has not being received within 60ms of last message, then set timeout error flag
//  Drv_I2C_Timeout();

  // check for CAN data from GUI
  CAN_Receive_from_GUI();

  // send data to GUI if FIFO from secondary core is full
  CAN_Transmit_to_GUI();

  // red LED blinking @ 1Hz as heartbeat
  static uint16_t led_blink_counter = 0;
  if (++led_blink_counter > 5000)
  {
    LD2_RD15_Toggle();
    led_blink_counter = 0;
  }
}
/**
 End of File
 */

