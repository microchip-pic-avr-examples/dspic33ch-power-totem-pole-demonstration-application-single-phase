/**
 * CAN Driver Source File
 * 
 * @file      drv_can.c
 * 
 * @ingroup   pri-layer-CAN
 * 
 * @brief     This is the driver source file for CAN driver
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

// MCC header files
//#include "slave1.h"
#include "driver/dma/drv_dma.h"

// other header files
#include "drv_can.h"
#include "driver/vac_monitor/vac_monitor.h"
#include "driver/spi/drv_spi.h"
#include "driver/i2c/drv_i2c.h"
#include "../../../revision.h"
#include "can/can_interface.h"
#include "system/pins.h"
#include "can/can1.h"

#include "secondary_core/sec_core1.h"

#include "../sources_common/Totempole_Application_Setup.h"

//------------------------------------------------------------------------------
// local typedefs
//------------------------------------------------------------------------------
struct HOUSEKEEPING_STATUS_s {

  union 
  {
    struct 
    {
      volatile uint8_t sic1_5v_ok : 1; ///< Bit #0
      volatile uint8_t sic2_5v_ok : 1; ///< Bit #1
      volatile uint8_t sic3_5v_ok : 1; ///< Bit #2
      volatile uint8_t sic4_5v_ok : 1; ///< Bit #3
      volatile uint8_t sic1_comms_ok : 1; ///< Bit #4
      volatile uint8_t sic2_comms_ok : 1; ///< Bit #5
      volatile uint8_t sic3_comms_ok : 1; ///< Bit #6
      volatile uint8_t sic4_comms_ok : 1; ///< Bit #7
      volatile uint8_t aux_5v_ok : 1; ///< Bit #8
      volatile uint8_t aux_12v_ok : 1; ///< Bit #9
      volatile uint8_t aux_24v_ok : 1; ///< Bit #10
      volatile uint8_t aux_comms_ok : 1; ///< Bit #11
      volatile uint8_t spi_checksum_ok : 1; ///< Bit #12
      volatile uint8_t spi_connection_ok : 1; ///< Bit #13                                    
      volatile unsigned : 1; ///< Bit #14: unused            
      volatile unsigned : 1; ///< Bit #15: unused                      
    } __attribute__((packed)) bits; // data structure for single bit addressing operations
    volatile uint16_t value; // buffer for 16-bit word read/write operations
  };
};

// TODO: create module specific structure wrapping all of these variables
typedef struct HOUSEKEEPING_STATUS_s HOUSEKEEPING_STATUS_t;

static HOUSEKEEPING_STATUS_t housekeeping_status;

static uint16_t MSIFIFOrec[32]; //intermediate buffer for S->M FIFO data
static uint8_t can_sendbuffer[64]; //buffer for send bytes via CAN interface

static uint16_t protocolAData[5]; 

typedef struct CAN_MSG_OBJ CAN_MSG_OBJ_t; // this is necessary!

static CAN_MSG_OBJ_t CANBRIDGE_MSGOBJ_TX; //struct from CAN-MCC
static CAN_MSG_OBJ_t CANBRIDGE_MSGOBJ_RX; //struct from CAN-MCC

static CAN_MSG_OBJ_t CANBRIDGE_FIRMWARE_VERSION_TX;

static uint8_t firmware_version[32] = {'p', 'r', 'i',':',' ', 
                                       REV_MAJOR,'.',REV_MINOR,'.',REV_PATCH,
                                       ',',' ','s','e','c',':',' ','.','.','.','.','.','.','.'};

/*******************************************************************************
 * @fn	CAN_TX_Obj_Init(void)
 * @brief   Initialize CAN transmit object
 * on main power board 
 * @param	None
 * @return  None
 * @ingroup   pri-layer-CAN
 * @details
 *********************************************************************************/
void CAN_TX_Obj_Init(void)
{
  CANBRIDGE_MSGOBJ_TX.msgId = CANSendID;
  CANBRIDGE_MSGOBJ_TX.field.dlc = DLC_64;
  CANBRIDGE_MSGOBJ_TX.field.brs = CAN_BRS_MODE;
  CANBRIDGE_MSGOBJ_TX.field.formatType = CAN_FD_FORMAT;
  CANBRIDGE_MSGOBJ_TX.field.frameType = CAN_FRAME_DATA;
  CANBRIDGE_MSGOBJ_TX.field.idType = CAN_FRAME_STD;
  CANBRIDGE_MSGOBJ_TX.data = can_sendbuffer;
  
  // object for transmission of the firmware version number
  CANBRIDGE_FIRMWARE_VERSION_TX.msgId = 0x1000;
  CANBRIDGE_FIRMWARE_VERSION_TX.field.dlc = DLC_32;
  CANBRIDGE_FIRMWARE_VERSION_TX.field.brs = CAN_BRS_MODE;
  CANBRIDGE_FIRMWARE_VERSION_TX.field.formatType = CAN_FD_FORMAT;
  CANBRIDGE_FIRMWARE_VERSION_TX.field.frameType = CAN_FRAME_DATA;
  CANBRIDGE_FIRMWARE_VERSION_TX.field.idType = CAN_FRAME_EXT;   
}

/*******************************************************************************
 * @fn	void Housekeeping_Flags_Update(void)
 * @brief   Update housekeeping flags related to PIC16s on SiC cards and PIC16
 * on main power board 
 * @param	None
 * @return  None
 * @ingroup   pri-layer-CAN
 * @details
 * PIC16 on main power board sends I2C message to dsPIC33CH primary core
 * every 50ms. Contains information about the 5V rail on each SiC card, the comms
 * status of each SiC card, and the status of the 5V, 12V and 24V rails on the 
 * main power board
 * Each SiC card has a related "5V ok" flag and "comms" flag.
 * Each of the Aux rails (5V, 12V and 24V) has an associated flag.
 * Also, there are flags for I2C timeout and "cmd ok", which is high when the 
 * command byte of the I2C message received from the PIC16 on the main power 
 * board is correct.
 *********************************************************************************/
void Housekeeping_Flags_Update(void)
{
  // sic status byte from I2C
  // bits 0:1 -> SiC card 1
  // bits 2:3 -> SiC card 2
  // bits 4:5 -> SiC card 3
  // bits 6:7 -> SiC card 4
  //
  // if 0: comms ok, 5V ok
  // if 1 or 2: comms ok, 5V not ok
  // if 3 comms not ok, 5V not ok (assume the worst as don't know)        

  // fetch i2c write data via API function
  I2C_t i2c_write_data = Drv_I2C_Get_Write_Data();

  //----------------------------------------------------------------------------
  // SiC card 1
  //----------------------------------------------------------------------------
  if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic1 == 0)
  {
    housekeeping_status.bits.sic1_5v_ok = 1;
    housekeeping_status.bits.sic1_comms_ok = 1;
  }
  else if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic1 == 3)
  {
    housekeeping_status.bits.sic1_5v_ok = 0;
    housekeeping_status.bits.sic1_comms_ok = 0;
  }
  else
  {
    // 1 or 2
    housekeeping_status.bits.sic1_5v_ok = 0;
    housekeeping_status.bits.sic1_comms_ok = 1;
  }

  //----------------------------------------------------------------------------
  // SiC card 2
  //----------------------------------------------------------------------------
  if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic2 == 0)
  {
    housekeeping_status.bits.sic2_5v_ok = 1;
    housekeeping_status.bits.sic2_comms_ok = 1;
  }
  else if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic2 == 3)
  {
    housekeeping_status.bits.sic2_5v_ok = 0;
    housekeeping_status.bits.sic2_comms_ok = 0;
  }
  else
  {
    // 1 or 2
    housekeeping_status.bits.sic2_5v_ok = 0;
    housekeeping_status.bits.sic2_comms_ok = 1;
  }

  //----------------------------------------------------------------------------
  // SiC card 3
  //----------------------------------------------------------------------------
  if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic3 == 0)
  {
    housekeeping_status.bits.sic3_5v_ok = 1;
    housekeeping_status.bits.sic3_comms_ok = 1;
  }
  else if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic3 == 3)
  {
    housekeeping_status.bits.sic3_5v_ok = 0;
    housekeeping_status.bits.sic3_comms_ok = 0;
  }
  else
  {
    // 1 or 2
    housekeeping_status.bits.sic3_5v_ok = 0;
    housekeeping_status.bits.sic3_comms_ok = 1;
  }

  //----------------------------------------------------------------------------
  // SiC card 4
  //----------------------------------------------------------------------------
  if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic4 == 0)
  {
    housekeeping_status.bits.sic4_5v_ok = 1;
    housekeeping_status.bits.sic4_comms_ok = 1;
  }
  else if (i2c_write_data.frame.bytes.sic_status_flags.bits.sic4 == 3)
  {
    housekeeping_status.bits.sic4_5v_ok = 0;
    housekeeping_status.bits.sic4_comms_ok = 0;
  }
  else
  {
    // 1 or 2
    housekeeping_status.bits.sic4_5v_ok = 0;
    housekeeping_status.bits.sic4_comms_ok = 1;
  }

  //----------------------------------------------------------------------------
  // PIC16 on main board (5V, 12V and 24V rails)
  // this data is received over I2C 
  //----------------------------------------------------------------------------
  if (i2c_write_data.frame.bytes.aux_fault_flags.bits._5v == 0)
  {
    housekeeping_status.bits.aux_5v_ok = 1;
  }
  else
  {
    housekeeping_status.bits.aux_5v_ok = 0;
  }

  if (i2c_write_data.frame.bytes.aux_fault_flags.bits._12v == 0)
  {
    housekeeping_status.bits.aux_12v_ok = 1;
  }
  else
  {
    housekeeping_status.bits.aux_12v_ok = 0;
  }

  if (i2c_write_data.frame.bytes.aux_fault_flags.bits._24v == 0)
  {
    housekeeping_status.bits.aux_24v_ok = 1;
  }
  else
  {
    housekeeping_status.bits.aux_24v_ok = 0;
  }

  // comms check
  if ((!i2c_write_data.status.bits.cmd_ok) || (i2c_write_data.status.bits.timeout))
  {
    housekeeping_status.bits.aux_comms_ok = 0;
  }
  else
  {
    housekeeping_status.bits.aux_comms_ok = 1;
  }

  housekeeping_status.bits.spi_checksum_ok = Drv_SPI_get_ChecksumOK_Flag();
  housekeeping_status.bits.spi_connection_ok = Drv_SPI_get_Connected_Flag();

}

/*******************************************************************************
 * @fn	void CAN_Transmit_to_GUI(void)
 * @brief   send frame to GUI when the FIFO from secondary to primary has been filled
 * @param	None
 * @return  None
 * @ingroup   pri-layer-CAN  
 * @details
 * when 32 word FIFO from secondary core to primary core is full 
 * we create a 64 byte CAN frame to be transmitted to the Power Board Visualizer GUI
 *********************************************************************************/
void CAN_Transmit_to_GUI(void)
{
  if (MSI1FIFOCSbits.RFFULL) // FIFO from secondary to primary core is full
  {
    volatile uint16_t can_word_index;

    // average input voltage has been computed by AC monitor state machines
    // this data is stored in phase1.vin.avg, phase2.vin.avg and phase3.vin.avg
    
    uint16_t vin_avg[3];
    uint16_t * coreBuf = DRV_DMA_getCoreBuffer();
    
    DRV_DMA_getVinAverage(vin_avg);

    // CAN word 0, 1 and 2
    // average voltage for each phase, for display on power board visualizer GUI
    for (can_word_index = 0; can_word_index < 3; can_word_index++)
    {
      can_sendbuffer[can_word_index << 1] = (uint8_t) (vin_avg[can_word_index] >> 8);
      can_sendbuffer[(can_word_index << 1) + 1] = (uint8_t) (vin_avg[can_word_index] & 0x00FF);
    }

    // CAN words 3, 4, 5, 6
    // word 3: phase 1 voltage loop feed-forward term
    // word 4: phase 2 voltage loop feed-forward term
    // word 5: phase 3 voltage loop feed-forward term
    // word 6: Vac status word
    // this data has already been placed in the array pri_to_sec_core_buffer[]
    // for sending across to the secondary core
    for (can_word_index = 3; can_word_index < 7; can_word_index++)
    {
      can_sendbuffer[can_word_index << 1] = (uint8_t) (coreBuf[can_word_index] >> 8);
      can_sendbuffer[(can_word_index << 1) + 1] = (uint8_t) (coreBuf[can_word_index] & 0x00FF);
    }

    // next bunch of CAN bytes: fill them up with the data from the FIFO
    // channel from secondary to primary          
    uint16_t fifo_word_index = 0;

    while (!MSI1FIFOCSbits.RFEMPTY) // keep pulling words out of FIFO until it is empty
    {
      MSIFIFOrec[fifo_word_index] = MRSWFDATA; // read word from FIFO            

      // at present, only the first 12 words in the FIFO words contain valid data
      // so stop when index >= 12 
      if (fifo_word_index < 12)
      {
        // convert word to bytes into CAN send buffer
        can_sendbuffer[can_word_index << 1] = (uint8_t) (MSIFIFOrec[fifo_word_index] >> 8);
        can_sendbuffer[(can_word_index << 1) + 1] = (uint8_t) (MSIFIFOrec[fifo_word_index] & 0x00FF);
        can_word_index++;
      }

      fifo_word_index++; // keep going until FIFO is empty
    }

    // fill rest of the CAN buffer with data gathered over I2C and SPI 
    Housekeeping_Flags_Update(); // update flags for PBV GUI

    // convert words to bytes for CAN
    can_sendbuffer[can_word_index << 1] = (uint8_t) (housekeeping_status.value >> 8);
    can_sendbuffer[(can_word_index << 1) + 1] = (uint8_t) (housekeeping_status.value & 0x00FF);
    can_word_index++;

    // bytes 2 to 5 of I2C data contain SiC card temperatures
    // Power Board visualizer reads words (not bytes) so convert from bytes to words  
    I2C_t i2c_write_data = Drv_I2C_Get_Write_Data(); // fetch I2C write data via API function

    uint16_t i2c_byte_index;
    for (i2c_byte_index = 2; i2c_byte_index < 6; i2c_byte_index++)
    {
      int16_t temperature = (int16_t) ((int8_t) i2c_write_data.frame.array[i2c_byte_index] + 128); // sign extend if necessary

      // convert words to bytes
      can_sendbuffer[can_word_index << 1] = (uint8_t) (temperature >> 8);
      can_sendbuffer[(can_word_index << 1) + 1] = (uint8_t) (temperature & 0x00FF);
      can_word_index++;
    }

    static uint16_t transmit_firmware_version_counter = 100;
    transmit_firmware_version_counter--;
    if (transmit_firmware_version_counter <= 0)
    {
      // update secondary core firmware version information
      firmware_version[19] = (uint16_t)((MSIFIFOrec[3] & 0xFF00)>>8); // secondary core, major revision
      firmware_version[21] = (uint16_t)(MSIFIFOrec[3] & 0x00FF);      // secondary core, minor revision
      firmware_version[23] = (uint16_t)((MSIFIFOrec[4] & 0xFF00)>>8); // secondary core, patch revision
      firmware_version[25] = (uint16_t)(MSIFIFOrec[4] & 0x00FF);      // secondary core, high or low voltage
      CANBRIDGE_FIRMWARE_VERSION_TX.data = firmware_version;  
      CAN1_Transmit(CAN1_TXQ, &CANBRIDGE_FIRMWARE_VERSION_TX);
      transmit_firmware_version_counter = 2000;
    }
    else
    {
      // send bytes to GUI via CAN          
      CANBRIDGE_MSGOBJ_TX.data = can_sendbuffer;
      CAN1_Transmit(CAN1_TXQ, &CANBRIDGE_MSGOBJ_TX);      
    }
  }
}

/*******************************************************************************
 * @fn	void CAN_Receive_from_GUI(void)
 * @brief   received message from GUI. Send the data to the secondary core using 
 * mailbox protocol A
 * @param	None
 * @return  None
 * @ingroup   pri-layer-CAN 
 * @details
 * check if message received from Power Board Visualizer GUI. If so, send 
 * data across to secondary core via mailbox protocol A
 *********************************************************************************/
void CAN_Receive_from_GUI(void)
{
  if (CAN1_ReceivedMessageCountGet() > 0)
  {
    if (CAN1_Receive(&CANBRIDGE_MSGOBJ_RX) == true) // message received over CAN from GUI
    {
      // send CAN data from primary to secondary using protocol A
      protocolAData[0] = (uint16_t) (CANBRIDGE_MSGOBJ_RX.data[0] << 8) + CANBRIDGE_MSGOBJ_RX.data[1];
      protocolAData[1] = (uint16_t) (CANBRIDGE_MSGOBJ_RX.data[2] << 8) + CANBRIDGE_MSGOBJ_RX.data[3];
      protocolAData[2] = (uint16_t) (CANBRIDGE_MSGOBJ_RX.data[4] << 8) + CANBRIDGE_MSGOBJ_RX.data[5];
      protocolAData[3] = (uint16_t) (CANBRIDGE_MSGOBJ_RX.data[6] << 8) + CANBRIDGE_MSGOBJ_RX.data[7];
      protocolAData[4] = (uint16_t) (CANBRIDGE_MSGOBJ_RX.data[8] << 8) + CANBRIDGE_MSGOBJ_RX.data[9];
      SEC_CORE1_ProtocolWrite(MSI1_ProtocolA, protocolAData);
    }
  }
}