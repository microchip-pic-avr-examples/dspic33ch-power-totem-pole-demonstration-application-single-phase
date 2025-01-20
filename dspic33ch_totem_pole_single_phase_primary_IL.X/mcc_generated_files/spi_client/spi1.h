
/**
 * SPI1 Generated Driver Header File
 * 
 * @file        spi1.h
 * 
 * @ingroup     spiclientdriver
 * 
 * @brief       This is the generated driver header file for the SPI1 driver
 *
 * @skipline @version     Firmware Driver Version 1.1.3
 *
 * @skipline @version     PLIB Version 1.2.0
 *
 * @skipline    Device : dsPIC33CH512MP506
*/

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

#ifndef SPI1_H
#define SPI1_H

// Section: Included Files
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "spi_client_interface.h"

// Section: Data Type Definitions

/**
 @ingroup   spiclientdriver
 @brief     Structure object of type SPI_CLIENT_INTERFACE assigned with name 
            displayed in the Melody Driver User interface. A structure pointer can be used to achieve portability
            across the SPI CLIENT having same interface structure.
*/

extern const struct SPI_CLIENT_INTERFACE SPI1_Client;
      
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_Initialize API
 */
#define SPI1_Client_Initialize SPI1_Initialize
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_Deinitialize API
 */
#define SPI1_Client_Deinitialize SPI1_Deinitialize
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_Close API
 */
#define SPI1_Client_Close SPI1_Close
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_Open API
 */
#define SPI1_Client_Open SPI1_Open
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_ByteExchange API
 */
#define SPI1_Client_ByteExchange SPI1_ByteExchange
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_ByteWrite API
 */
#define SPI1_Client_ByteWrite SPI1_ByteWrite
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_ByteRead API
 */
#define SPI1_Client_ByteRead SPI1_ByteRead
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_IsRxReady API
 */
#define SPI1_Client_IsRxReady SPI1_IsRxReady
/**
 * @ingroup  spidriver
 * @brief    This macro defines the Custom Name for \ref SPI1_IsTxReady API
 */
#define SPI1_Client_IsTxReady SPI1_IsTxReady

/**
@ingroup spiclientdriver
@enum SPI1_CLIENT_USER_CONFIG
@brief Enum list is the Custom names for the SPI1 CLIENT multiple configuration, 
       configured by user in MCC Melody user interface. 
@note The enum list in the Help document might be just a reference to illustrate multiple configuration.
      Generated enum list is based on the configuration done by user in the MCC Melody user interface
*/

enum SPI1_CLIENT_USER_CONFIG{
     /**Custom name for configuration setting:
    SPI Mode : Mode 3**/
    CLIENT_CONFIG,
    
};

// Section: Driver Interface Function


/**
 * @ingroup spiclientdriver
 * @brief   Initializes SPI1 module, using the given initialization data. 
 *          This function must be called before any other SPI1 function is called.
 * @param   none
 * @return  none  
 */
void SPI1_Initialize (void);


/**
 * @ingroup spiclientdriver
 * @brief   Deinitializes the SPI1 to POR values.
 * @param   none
 * @return  none  
 */
void SPI1_Deinitialize(void);


/**
 * @ingroup spiclientdriver
 * @brief   Disables the SPI1 module. 
 * @pre     The \ref SPI1_Open function must be called before calling this function.
 * @param   none
 * @return  none  
 */
void SPI1_Close (void);


/**
 * @ingroup     spiclientdriver
 * @brief       Configures SPI1 module with user defined unique configuration. 
 * @param[in]   spiConfigIndex    - \ref SPI1_USER_CONFIG Enum value 
 * @return bool true   -   SPI1 is configured successfully.
 * @return bool false  -   SPI1 is not configured successfully.  
 */
bool SPI1_Open(uint8_t spiConfigIndex);


/**
 * @ingroup     spiclientdriver
 * @brief       Exchanges one byte of data. This is a blocking function.
 * @pre         The \ref SPI1_IsTxReady function must be called before calling this function.
 * @param[in]   data- Data byte to be exchanged.  
 * @return      none  
 */
uint8_t SPI1_ByteExchange(uint8_t byteData);


/**
 * @ingroup     spiclientdriver
 * @brief       Writes one byte of data.
 * @pre         The \ref SPI1_IsTxReady function must be called before calling this function.
 *              Also note that every byte write has to be followed by a byte read. 
 * @param[in]   byteData         - Data to be written.  
 * @return      none  
 */
void SPI1_ByteWrite(uint8_t byteData);


/**
 * @ingroup     spiclientdriver
 * @brief       Reads one byte of data from SPI1
 * @pre         The \ref SPI1_IsRxReady function must be called before calling this function.
 *              Also note that byte read must be after byte write sequence.
 * @param       none
 * @return      Data read from SPI1.  
 */
uint8_t SPI1_ByteRead(void);


/**
 * @ingroup spiclientdriver
 * @brief   Returns a boolean value if data is available to read.
 * @pre     The \ref SPI1_Open function must be called before calling this function.
 * @param   none
 * @return  true    - Data available to read
 * @return  false   - Data not available to read  
 */
bool SPI1_IsRxReady(void);


/**
 * @ingroup spiclientdriver
 * @brief   Returns a boolean value if data can be written.
 * @pre     The \ref SPI1_Open function must be called before calling this function.
 * @param   none
 * @return  true    - Data can be written
 * @return  false   - Data buffer is full  
 */
bool SPI1_IsTxReady(void);

#endif //SPI1_H



