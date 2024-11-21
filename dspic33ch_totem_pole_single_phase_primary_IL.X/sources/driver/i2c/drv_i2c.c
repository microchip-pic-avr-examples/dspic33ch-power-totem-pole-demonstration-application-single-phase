/**
 * I2C Driver Source File
 * 
 * @file      drv_i2c.c
 * 
 * @ingroup   pri-layer-I2C
 * 
 * @brief     This is the driver source file for I2C driver
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

#include "xc.h"
#include "drv_i2c.h"
#include "driver/can/drv_can.h"

// MCC header files
#include "i2c2.h"
#include "system/pins.h"

// create I2C write object, initialize each member to be in fault state
static I2C_t i2c_write = {
  .frame.bytes.aux_fault_flags.bits._5v = 1,
  .frame.bytes.aux_fault_flags.bits._12v = 1,
  .frame.bytes.aux_fault_flags.bits._24v = 1,
  .frame.bytes.sic_status_flags.bits.sic1 = 3,
  .frame.bytes.sic_status_flags.bits.sic2 = 3,
  .frame.bytes.sic_status_flags.bits.sic3 = 3,
  .frame.bytes.sic_status_flags.bits.sic4 = 3
};

static uint8_t i2c2_slaveWriteData = 0xAA; // holds byte written to slave

//------------------------------------------------------------------------------
// local function declarations
//------------------------------------------------------------------------------
static void i2c_crc_update(uint8_t data, struct I2C_s* i2c_obj);

/*******************************************************************************
 * @fn	Drv_I2C_Get_Write_Data()
 * @brief   read copy of i2c_write object
 * @param	None
 * @return  I2C comms object
 * @ingroup   pri-layer-I2C
 * @details
 * API function to allow reading "i2c_write" object
 *********************************************************************************/
I2C_t Drv_I2C_Get_Write_Data(void)
{
  return (i2c_write);
}

/*******************************************************************************
 * @fn	I2C2_StatusCallback()
 * @brief   I2C2 callback function
 * @param	I2C2_SLAVE_DRIVER_STATUS (defined by MCC drivers)
 * @return  bool (1 if pass through this function successfully)
 * @ingroup   pri-layer-I2C
 * @details
 * function declared by MCC generated code, defined here 
 *********************************************************************************/
bool I2C2_StatusCallback(I2C2_SLAVE_DRIVER_STATUS status)
{
  static uint16_t byteCount;

  switch (status)
  {
    case I2C2_SLAVE_TRANSMIT_REQUEST_DETECTED:
      // set up the slave driver buffer transmit pointer
      I2C2_ReadPointerSet(&i2c_write.frame.array[byteCount++]);
      if (byteCount >= I2C_WRITE_BUFFER_SIZE)
      {
        byteCount = 0;
      }
      break;

    case I2C2_SLAVE_RECEIVE_REQUEST_DETECTED:
      // address of slave matches address in I2C message transmitted 
      // from I2C master
      byteCount = 0;

      // clear status flags
      i2c_write.status.bits.done = 0;

      // clear timeout flag and counter
      i2c_write.status.bits.timeout = false;
      i2c_write.timeout_counter = 0;

      // set up the slave driver buffer receive pointer
      I2C2_WritePointerSet(&i2c2_slaveWriteData);

      // start CRC calculation
      // first clear CRC byte
      i2c_write.crc = 0;

      // read first byte written from I2C master
      // this should be the address, left shifted by 1 bit, plus the 
      // read/write bit in bit position 0 (read/write bit = 0) in this 
      // case as I2C master is writing to I2C slave
      volatile uint8_t i2c_rcv = (uint8_t) I2C2RCV;
      i2c_crc_update(i2c_rcv, &i2c_write);

      break;

    case I2C2_SLAVE_RECEIVED_DATA_DETECTED:

      // receiving non-address data          
      // update CRC
      i2c_crc_update(i2c2_slaveWriteData, &i2c_write);

      // check if command byte is valid 
      // (cmd byte = first byte received after address byte)
      if (byteCount == 0)
      {
        // first byte should be a valid command byte
        if (i2c2_slaveWriteData != CMD_SEND_COMBINED_STATUS_DATA)
        {
          // error, command byte is not correct
          i2c_write.status.bits.cmd_ok = 0;
        }
        else
        {
          // command byte is as expected
          i2c_write.status.bits.cmd_ok = 1;
        }
      }

      // fill buffer with received data
      if (byteCount < I2C_WRITE_BUFFER_SIZE)
      {
        i2c_write.frame.array[byteCount] = i2c2_slaveWriteData;
      }
      byteCount++;

      break;

    case I2C2_SLAVE_10BIT_RECEIVE_REQUEST_DETECTED:
      // 10-bit address is detected           
      break;

    case I2C2_SLAVE_STOP_BIT_DETECTED:

      // finished processing this message
      i2c_write.status.bits.done = 1;

      // CRC check, all going well the CRC should be zero at this point
      if (i2c_write.crc == 0)
      {
        i2c_write.status.bits.pec_ok = 1;
      }
      else
      {
        i2c_write.status.bits.pec_ok = 0;
      }

      break;

    default:
      break;

  }

  return true;
}

//------------------------------------------------------------------------------
// look up table for CRC8 with polynomial 0x07 (used for SMBus PEC)
//------------------------------------------------------------------------------
const uint8_t crc_lut[256] = {0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
  0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
  0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
  0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
  0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
  0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
  0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
  0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
  0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
  0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
  0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
  0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
  0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
  0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
  0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
  0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3};

/*******************************************************************************
 * @fn	i2c_crc_update()
 * @brief   update CRC using incoming data byte
 * @param	copy of byte, pointer to I2C data structure
 * @return  None
 * @ingroup   pri-layer-I2C 
 * @details
 * uses look up table for efficiency. local function
 *********************************************************************************/
static void i2c_crc_update(uint8_t data, struct I2C_s* i2c_obj)
{
  // xor byte with crc
  uint8_t xor_data_crc = (uint8_t) (data ^ i2c_obj->crc);

  // calculate CRC using LUT
  i2c_obj->crc = (uint8_t) (crc_lut[xor_data_crc]);
}

/*******************************************************************************
 * @fn	Drv_I2C_Timeout()
 * @brief   check if I2C timeout has occurred (no I2C message for I2C_TIMEOUT_IN_100us_TICKS seconds)
 * @param	None
 * @return  None
 * @ingroup   pri-layer-I2C 
 * @details
 * expext I2C message every 50ms
 * if none received in last I2C_TIMEOUT_IN_100us_TICKS seconds, then I2C timeout flag
 * This is displayed on Power Board Visualizer GUI
 *********************************************************************************/
void Drv_I2C_Timeout(void)
{
  i2c_write.timeout_counter++;
  if (i2c_write.timeout_counter > I2C_TIMEOUT_IN_100us_TICKS)
  {
    i2c_write.timeout_counter = 0;
    i2c_write.status.bits.timeout = true; // set timeout flag
  }
}