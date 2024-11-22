/**
 * DMA Driver Source File
 * 
 * @file      drv_dma.c
 * 
 * @ingroup   pri-layer-DMA
 * 
 * @brief     This is the driver source file for DMA driver
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

// from MCC
#include <string.h>

#include "system/pins.h"
#include "secondary_core/sec_core_interface.h"

// from elsewhere
#include "driver/spi/drv_spi.h"
#include "driver/can/drv_can.h"
#include "driver/vac_monitor/vac_monitor.h"

#include "dma/dma.h"
#include "dma/dma_types.h"

#include "secondary_core/sec_core1.h"

#include "../mcc_generated_files/cmp/cmp1.h"


// local typedefs
typedef enum {STATE_WAIT_VALID_VIN_SENSE_OFFSET = 0, STATE_ONLINE} DRV_DMA_STATES_t;

struct DRV_DMA_DATA_s 
{
  VACM_t phase1;
  VACM_t phase2;
  VACM_t phase3;

  uint16_t vinAdcBuf1;
  uint16_t vinAdcBuf2;
  uint16_t vinAdcBuf3;

  int16_t vacOpAmpBias;

  uint16_t coreBuffers[10];
  
  uint16_t* vinAdcRaw;   // pointer to SPI data
  
  DRV_DMA_STATES_t state;
};

typedef struct DRV_DMA_DATA_s DRV_DMA_DATA_t;
static DRV_DMA_DATA_t selfDma;

/*******************************************************************************
 * @fn	DRV_DMA_init()
 * @brief   initialize the local DMA structure
 * @param	None
 * @return  None
 * @ingroup   pri-layer-DMA  
 * @details
 * This is an API function
 *********************************************************************************/
void DRV_DMA_init(void)
{
  memset((void*) &selfDma, 0x00U, sizeof (selfDma));
  
  // initialize, for each phase monitor object
  // - pointer to ADC result
  // - counter reset value for Vin average calculation
  // - pointer to ADC reading of AC sense offset on isolated voltage acquisition board
  
  selfDma.phase1.vin.ptr_adcbuf = (uint16_t *) (&selfDma.vinAdcBuf1);
  
  // avgcalc_buffer: measure period over half cycle, assume nominal 50Hz, for counter reset is based on number of 10us ticks in 10ms
  selfDma.phase1.avgcalc_buffer.counter_reset = (uint16_t) (FCALL_AC_MONITOR / 50.0 / 2.0); 
  selfDma.phase1.vin.ptr_offset = (int16_t*) (&selfDma.vacOpAmpBias);

  selfDma.phase2.vin.ptr_adcbuf = (uint16_t *) (&selfDma.vinAdcBuf2);
  selfDma.phase2.avgcalc_buffer.counter_reset = (uint16_t) (FCALL_AC_MONITOR / 50.0 / 2.0);
  selfDma.phase2.vin.ptr_offset = (int16_t*) (&selfDma.vacOpAmpBias);

  selfDma.phase3.vin.ptr_adcbuf = (uint16_t *) (&selfDma.vinAdcBuf3);
  selfDma.phase3.avgcalc_buffer.counter_reset = (uint16_t) (FCALL_AC_MONITOR / 50.0 / 2.0);
  selfDma.phase3.vin.ptr_offset = (int16_t*) (&selfDma.vacOpAmpBias);
  
  // initialize pointer to raw ADC data, for this project it coming from DMA via SPI
  // from this SPI data, we determine selfDma.vinAdcBuf1, selfDma.vinAdcBuf2 and selfDma.vinAdcBuf3
  selfDma.vinAdcRaw = Drv_SPI_get_Adr_Data_Obj();  
  
  // initialize state so that first we read the measured value of AC sense offset
  selfDma.state = STATE_WAIT_VALID_VIN_SENSE_OFFSET;
  
}

/*******************************************************************************
 * @fn	DMA_Channel_updateAdcBuffers()
 * @brief   unpack the ADC measurements of phase voltages and place in buffers
 * @param	None
 * @return  None
 * @ingroup   pri-layer-DMA  
 * @details
 * 3 phase voltages are measured on isolated voltage acquisition board.
 * This data is packed into 4 16-bit words with the AC sense offset and a checksum
 * Here this data is unpacked and placed in 3 ADC buffers, so it can be used
 * by the AC monitor
 *********************************************************************************/
static void __attribute__((always_inline)) DMA_Channel_updateAdcBuffers(void)
{
  // update data received via SPI
  selfDma.vinAdcBuf1 = selfDma.vinAdcRaw[0] & 0x0FFF;
  selfDma.vinAdcBuf2 = ((selfDma.vinAdcRaw[0] & 0xF000) >> 12) + ((selfDma.vinAdcRaw[1] & 0x00FF) << 4);
  selfDma.vinAdcBuf3 = ((selfDma.vinAdcRaw[1] & 0xFF00) >> 8) + ((selfDma.vinAdcRaw[2] & 0x000F) << 8);
}

/*******************************************************************************
 * @fn	DMA_Channel_doAcMonitor()
 * @brief   run AC monitor for each of the 3 phases
 * @param	None
 * @return  None
 * @ingroup   pri-layer-DMA  
 * @details
 * 
 *********************************************************************************/
static void __attribute__((always_inline)) DMA_Channel_doAcMonitor(void)
{
  // run the AC monitor state machines using the data received over SPI as input
  vacm_state_machine(&selfDma.phase1);
  vacm_state_machine(&selfDma.phase2);
  vacm_state_machine(&selfDma.phase3);
}

/*******************************************************************************
 * @fn	DMA_Channel_updateCoreBuffers()
 * @brief   update the buffer used to store data that will be sent to sec core
 * @param	pointer to core buffer
 * @return  None
 * @ingroup   pri-layer-DMA  
 * @details
 * mailbox used to send processed AC input voltage from primary core to secondary core
 * the buffer used to store the mailbox data is updated here 
 *********************************************************************************/
static void __attribute__((always_inline)) DMA_Channel_updateCoreBuffers(uint16_t * coreBuffer)
{
    
  //<<<< only for Debug purpose---------------
  DAC1DATH = selfDma.phase1.vin.raw;  
  //<<<< only for Debug purpose---------------
  coreBuffer[0] = selfDma.phase1.vin.rectified;
  coreBuffer[1] = selfDma.phase2.vin.rectified;
  coreBuffer[2] = selfDma.phase3.vin.rectified;
  coreBuffer[3] = selfDma.phase1.vin.vloop_ff;
  coreBuffer[4] = selfDma.phase2.vin.vloop_ff;
  coreBuffer[5] = selfDma.phase3.vin.vloop_ff;
  coreBuffer[7] = selfDma.phase1.vin.avg;
  coreBuffer[8] = selfDma.phase2.vin.avg;
  coreBuffer[9] = selfDma.phase3.vin.avg;
}

/*******************************************************************************
 * @fn	DMA_Channel_resetAcMonitor()
 * @brief reset the 3 AC monitor objects
 * @param	None
 * @return  None
 * @ingroup   pri-layer-DMA  
 * @details
 * reset all of the AC monitor objects
 *********************************************************************************/
static void __attribute__((always_inline)) DMA_Channel_resetAcMonitor(void)
{
  // set AC ok flags to zero so that secondary core will shut off PFC as soon as possible
  vacm_reset_phase_monitor_object(&selfDma.phase1);
  vacm_reset_phase_monitor_object(&selfDma.phase2);
  vacm_reset_phase_monitor_object(&selfDma.phase3);
  vacm_reset_state_machine(&selfDma.phase1);
  vacm_reset_state_machine(&selfDma.phase2);
  vacm_reset_state_machine(&selfDma.phase3);
}

/*******************************************************************************
 * @fn	DMA_Channel_updateStatusAndSendData()
 * @brief update status flags for each phase, update core buffer word, send data via mailbox B
 * @param	pointer to core buffer 
 * @return  None
 * @ingroup   pri-layer-DMA 
 * @details
 * note that the status flags are packed in such a way that execution time
 * on primary core is optimized, as all DMA interrupt tasks need to be completed
 * within 10us
 *********************************************************************************/
static void __attribute__((always_inline)) DMA_Channel_updateStatusAndSendData(uint16_t * coreBuffer)
{
  // update status flags. The bit-fields are organized in such a way so they can be packed quickly
  // into one 16-bit word for transmission via mailbox to secondary core
  uint16_t phase1_status = selfDma.phase1.status.value & 0x001F;
  uint16_t phase2_status = (selfDma.phase2.status.value & 0x001F) << 5;
  uint16_t phase3_status = (selfDma.phase3.status.value & 0x001F) << 10;
  coreBuffer[6] = phase1_status + phase2_status + phase3_status + HV_FLAG;

  // send data to secondary core via mailbox B
  SEC_CORE1_ProtocolWrite(MSI1_ProtocolB, coreBuffer);
}


/*******************************************************************************
 * @fn	DRV_DMA_getVinAverage()
 * @brief return average value of Vin for each phase
 * @param	pointer to result 
 * @return  None
 * @ingroup   pri-layer-DMA 
 * @details
 * Vin average is calculated by the AC monitor (for each phase)
 * This is an API function
 *********************************************************************************/
void DRV_DMA_getVinAverage(uint16_t * vinAvg)
{
  vinAvg[0] = selfDma.phase1.vin.avg;
  vinAvg[1] = selfDma.phase2.vin.avg;
  vinAvg[2] = selfDma.phase3.vin.avg;
}

/*******************************************************************************
 * @fn	DRV_DMA_getCoreBuffer()
 * @brief API function to get core buffer
 * @param	None
 * @return  pointer to core buffer
 * @ingroup   pri-layer-DMA
 * @details
 * 
 *********************************************************************************/
uint16_t* DRV_DMA_getCoreBuffer(void)
{
  return selfDma.coreBuffers;
}

/*******************************************************************************
 * @fn	DMA_Channel0_CallBack()
 * @brief DMA interrupt callback function
 * @param	None
 * @return None
 * @ingroup   pri-layer-DMA
 * @details
 * Critical function, contains most of the AC monitor functionality
 * DMA configured with SPI
 * SPI message should be received from isolated voltage acquisition board every 10us
 * 
 * First, checks if the SPI is connected
 * If so, runs checksum test on received data
 * if this passes, we run a mini-state machine
 * first state is used only to read the AC sense offset (this is transmitted over SPI)
 * Once this has been received correctly, we run AC monitor based on the 
 * ADC reading of each of the 3 phase voltage (which come from isolated voltage acquisition
 * board via SPI).
 * After AC monitor executed for each phase voltage, the results are sent to the secondary
 * core via a mailbox.
 * if SPI is not connected or checksum test fails, still send data to secondary core
 * but clear AC monitor status flags, so secondary core will shut down PFC (as the AC OK
 * for each phase will be cleared in this case)
 *********************************************************************************/
void DMA_ChannelCallback(enum DMA_CHANNEL channel)
{ 
//////  PIM_TP4_Toggle();
  if (Drv_SPI_get_Connected_Flag())
  {
    // SPI is connected
    if (Drv_SPI_Checksum()) 
    {     
      // SPI checksum test passes
      Drv_SPI_Reset_Timeout_Counter(SPI_TIMEOUT_IN_100us_TICKS);  
      switch(selfDma.state)
      {
        case STATE_WAIT_VALID_VIN_SENSE_OFFSET:
        {
          // wait for a valid reading of the op-amp offset before running AC monitor
          // on isolated voltage acquisition board, the offset is only measured at startup
          // so once we have a valid reading we exit this state
          // a valid reading of the AC offset is one that is within +/- 5% of the expected value of 1.65V
          int16_t opAmpBias = (int16_t) ((selfDma.vinAdcRaw[2] & 0xFFF0) >> 4);   // unpack ADC reading from data received over SPI
          DMA_ChannelEnable(0);  // Enable DMA Channel, this needs to be done for correct operation
          if ((opAmpBias < OPAMP_BIAS_HIGH_LIMIT) && (opAmpBias > OPAMP_BIAS_LOW_LIMIT))
          {
            selfDma.vacOpAmpBias = opAmpBias; // store reading
            selfDma.state = STATE_ONLINE;     // onto normal running state
          }          
        }
        break;
        
        case STATE_ONLINE:
        {
          // this is the normal running state
          // update ADC buffers with data received over SPI
          DMA_Channel_updateAdcBuffers();  

          // Enable DMA Channel, this needs to be done for correct operation
          // doing it here means that the SPI/DMA peripherals are ready to
          // receive new SPI data from the isolated voltage acquisition board
          // while we are still processing the current data
          DMA_ChannelEnable(0);
          
          // run AC monitor for each of the 3 phases
          DMA_Channel_doAcMonitor(); 
          
          // update buffer used to send data across to sec. core via mailbox
          DMA_Channel_updateCoreBuffers(selfDma.coreBuffers); 
        }
        break;
        
        default:
          selfDma.state = STATE_WAIT_VALID_VIN_SENSE_OFFSET;
          break;
      } // switch(selfDma.state)
    }
    else
    {
      // checksum test failed. 
      DMA_ChannelEnable(0);  // Enable DMA Channel, this needs to be done for correct operation
      
      // Reset AC monitor. In this case the secondary core should turn off PFC
      // as the AC OK flags for each phase will be cleared
      DMA_Channel_resetAcMonitor();
    }    
  }
  else
  {
      // SPI not connected
      DMA_ChannelEnable(0);  // Enable DMA Channel, this needs to be done for correct operation
      
      // Reset AC monitor. In this case the secondary core should turn off PFC
      // as the AC OK flags for each phase will be cleared
      DMA_Channel_resetAcMonitor();
  }
  
  // update status flags and send data to sec. core via mailbox B
  DMA_Channel_updateStatusAndSendData(selfDma.coreBuffers);
}
/**
 End of File
 */

