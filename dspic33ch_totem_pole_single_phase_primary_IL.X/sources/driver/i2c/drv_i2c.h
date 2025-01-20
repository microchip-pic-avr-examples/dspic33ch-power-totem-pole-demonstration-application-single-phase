/**
 * I2C Driver HEader File
 * 
 * @file      drv_i2c.h
 * 
 * @ingroup   pri-layer-I2C
 * 
 * @brief     This is the driver header file for DMA driver
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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DRV_I2C_H
#define	DRV_I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  

#include "i2c2.h"

//------------------------------------------------------------------------------
// macro definitions
//------------------------------------------------------------------------------
// I2C_WRITE_BUFFER_SIZE: buffer to store received data from I2C master
#define I2C_WRITE_BUFFER_SIZE   (9)

// first byte transmitted from PIC16 is a "command" byte
#define CMD_SEND_COMBINED_STATUS_DATA   (0x43)

// expect I2C message every 50ms, if not within this time of last message, set error flag
#define I2C_TIMEOUT             (60.0e-3)     // in seconds

// I2C_TIMEOUT in 100us ticks
#define I2C_TIMEOUT_IN_100us_TICKS       ((uint16_t)(I2C_TIMEOUT/100.0e-6)) 

//------------------------------------------------------------------------------
// typedefs
//------------------------------------------------------------------------------

struct I2C_STATUS_s {

  union {

    struct {
      volatile bool cmd_ok : 1; ///< Bit #0: flag indicating the validity of the command byte
      volatile bool pec_ok : 1; ///< Bit #1: flag indicating PEC error (CRC error in datastream)
      volatile bool done : 1; ///< Bit #2: flag indicating that I2C module is finished processing this message
      volatile bool timeout : 1; ///< Bit #3: flag indicating that I2C message has not been received withing timeout period
      volatile unsigned : 1; ///< Bit #4: unused            
      volatile unsigned : 1; ///< Bit #5: unused  
      volatile unsigned : 1; ///< Bit #6: unused    
      volatile unsigned : 1; ///< Bit #7: unused   
    } __attribute__((packed)) bits; // data structure for single bit addressing operations

    volatile uint8_t value; // buffer for 8-bit word read/write operations
  };
};
typedef struct I2C_STATUS_s I2C_STATUS_t; ///< Status word object

struct SIC_FLAGS_s {

  union {

    struct {
      volatile uint8_t sic1 : 2; ///< Bit 0:1  
      volatile uint8_t sic2 : 2; ///< Bit 2:3  
      volatile uint8_t sic3 : 2; ///< Bit 4:5  
      volatile uint8_t sic4 : 2; ///< Bit 6:7          
    } __attribute__((packed)) bits;
    volatile uint8_t value;
  };
};
typedef struct SIC_FLAGS_s SIC_FLAGS_t;

struct AUX_FLAGS_s {

  union {

    struct {
      volatile uint8_t _5v : 2; ///< Bit 0:1 -> 0 = no fault  
      volatile uint8_t _12v : 2; ///< Bit 2:3 -> 0 = no fault 
      volatile uint8_t _24v : 2; ///< Bit 4:5 -> 0 = no fault 
      volatile uint8_t : 2; ///< Bit 6:7 -> 0 = no fault 
    } __attribute__((packed)) bits;
    volatile uint8_t value;
  };
};
typedef struct AUX_FLAGS_s AUX_FLAGS_t;

struct SIC_CLIENT_RECEIVE_FRAME_s {

  union {

    struct {
      uint8_t command_id; // byte 0
      SIC_FLAGS_t sic_status_flags; // byte 1
      uint8_t temp_sic1; // byte 2
      uint8_t temp_sic2; // byte 3
      uint8_t temp_sic3; // byte 4
      uint8_t temp_sic4; // byte 5
      AUX_FLAGS_t aux_fault_flags; // byte 6
      uint8_t fan_duty;
      // byte 7
      uint8_t crc_rx; // byte 8
    } __attribute__((packed)) bytes;
    uint8_t array[I2C_WRITE_BUFFER_SIZE];
  };
};
typedef struct SIC_CLIENT_RECEIVE_FRAME_s SIC_CLIENT_RECEIVE_FRAME_t;

struct I2C_s {
  I2C_STATUS_t status; ///< status flags
  SIC_CLIENT_RECEIVE_FRAME_t frame; ///< buffer to store I2C data
  uint8_t crc; ///< running crc
  uint16_t timeout_counter;
};

typedef struct I2C_s I2C_t;


//------------------------------------------------------------------------------
// Public data objects
//------------------------------------------------------------------------------
bool I2C2_StatusCallback(I2C2_SLAVE_DRIVER_STATUS status); 
I2C_t Drv_I2C_Get_Write_Data(void); // API function
void Drv_I2C_Timeout(void);

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */

#endif	/* DRV_I2C_H */

