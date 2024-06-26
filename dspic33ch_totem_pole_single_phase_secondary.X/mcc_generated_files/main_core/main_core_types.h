/**
 * MAIN_CORE Generated Driver Types Header File
 * 
 * @file      main_core_types.h
 *            
 * @ingroup   main_coredriver
 *            
 * @brief     This is the generated driver types header file for the MAIN_CORE driver
 *            
 * @skipline @version   Firmware Driver Version 1.0.0
 *
 * @skipline @version   PLIB Version 1.0.0
 *            
 * @skipline  Device : dsPIC33CH128MP508S1
*/

/*disclaimer*/

#ifndef MAIN_CORE_TYPES_H
#define MAIN_CORE_TYPES_H

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_SYSTEM_STATUS
 @brief     Defines the status enumeration for main_core.
*/
enum MAIN_CORE_SYSTEM_STATUS
{
    MAIN_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE = 0,   /**< main_core status is in Low power mode */
    MAIN_CORE_SYSTEM_STATUS_SLEEP_MODE            = 1,   /**< main_core status is in Sleep mode */
    MAIN_CORE_SYSTEM_STATUS_IDLE_MODE             = 2,   /**< main_core status is in Idle mode */
    MAIN_CORE_SYSTEM_STATUS_RUNNING_STATE         = 3,   /**< main_core status is in Running state */
    MAIN_CORE_SYSTEM_STATUS_IN_RESET_STATE        = 4    /**< main_core status is in Reset state */
};

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_PROTOCOLS
 @brief     Defines the list of Protocols configured for MAIN_CORE_driver. 
 @note      These Protocols are required to create the buffer by the application
*/
enum MAIN_CORE_PROTOCOLS
{
    MSI1_ProtocolA,      /**< Custom-name of ProtocolA for read operation only */
    MSI1_ProtocolB,      /**< Custom-name of ProtocolB for read operation only */
};

/**
 @ingroup   main_coredriver
 @enum      MAIN_CORE_PROTOCOL_SIZE
 @brief     Defines the size for Protocols.
 @note      Size not required for the driver but will be required for application to create the buffers
*/
enum MAIN_CORE_PROTOCOL_SIZE
{
    MSI1_ProtocolA_SIZE = 5,     /**< Size of ProtocolA in 16-bit word*/
    MSI1_ProtocolB_SIZE = 10,     /**< Size of ProtocolB in 16-bit word*/
};

#endif    /* MAIN_CORE_TYPES_H */