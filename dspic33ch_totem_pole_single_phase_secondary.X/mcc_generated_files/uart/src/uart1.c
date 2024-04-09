/**
 * UART1 Generated Driver Source File
 * 
 * @file      uart1.c
 *            
 * @ingroup   uartdriver
 *            
 * @brief     This is the generated driver source file for the UART1 driver.
 *            
 * @skipline @version   Firmware Driver Version 1.7.0
 *
 * @skipline @version   PLIB Version 1.5.0
 *            
 * @skipline  Device : dsPIC33CH512MP506S1
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

// Section: Included Files
#include <stdint.h>
#include <stddef.h>
#include <xc.h>
#include <stddef.h>
#include "../uart1.h"

// Section: Macro Definitions
#define UART1_CLOCK 100000000U
#define UART1_BAUD_TO_BRG_WITH_FRACTIONAL(x) (UART1_CLOCK/(x))
#define UART1_BAUD_TO_BRG_WITH_BRGH_1(x) (UART1_CLOCK/(4U*(x))-1U)
#define UART1_BAUD_TO_BRG_WITH_BRGH_0(x) (UART1_CLOCK/(16U*(x))-1U)
#define UART1_BRG_TO_BAUD_WITH_FRACTIONAL(x) (UART1_CLOCK/(x))
#define UART1_BRG_TO_BAUD_WITH_BRGH_1(x) (UART1_CLOCK/(4U*((x)+1U)))
#define UART1_BRG_TO_BAUD_WITH_BRGH_0(x) (UART1_CLOCK/(16U*((x)+1U)))

#define UART1_MIN_ACHIEVABLE_BAUD_WITH_FRACTIONAL 95U
#define UART1_MIN_ACHIEVABLE_BAUD_WITH_BRGH_1 24U

// Section: Driver Interface

const struct UART_INTERFACE UART1_Drv = {
    .Initialize = &UART1_Initialize,
    .Deinitialize = &UART1_Deinitialize,
    .Read = &UART1_Read,
    .Write = &UART1_Write,
    .IsRxReady = &UART1_IsRxReady,
    .IsTxReady = &UART1_IsTxReady,
    .IsTxDone = &UART1_IsTxDone,
    .TransmitEnable = &UART1_TransmitEnable,
    .TransmitDisable = &UART1_TransmitDisable,
    .TransmitInterruptEnable = NULL,
    .TransmitInterruptDisable = NULL,
    .AutoBaudSet = &UART1_AutoBaudSet,
    .AutoBaudQuery = &UART1_AutoBaudQuery,
    .AutoBaudEventEnableGet = &UART1_AutoBaudEventEnableGet,
    .BRGCountSet = &UART1_BRGCountSet,
    .BRGCountGet = &UART1_BRGCountGet,
    .BaudRateSet = &UART1_BaudRateSet,
    .BaudRateGet = &UART1_BaudRateGet,
    .ErrorGet = &UART1_ErrorGet,
    .RxCompleteCallbackRegister = NULL,
    .TxCompleteCallbackRegister = NULL,
    .TxCollisionCallbackRegister = NULL,
    .FramingErrorCallbackRegister = NULL,
    .OverrunErrorCallbackRegister = NULL,
    .ParityErrorCallbackRegister = NULL,
};

// Section: Private Variable Definitions
static union
{
    struct
    {
        uint16_t frammingError :1;
        uint16_t parityError :1;
        uint16_t overrunError :1;
        uint16_t txCollisionError :1;
        uint16_t autoBaudOverflow :1;
        uint16_t reserved :11;
    };
    size_t status;
} uartError;

// Section: UART1 APIs

void UART1_Initialize(void)
{
/*    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
  
//TODO: melody settings below  
  
    // URXEN ; RXBIMD ; UARTEN disabled; MOD Asynchronous 8-bit UART; UTXBRK ; BRKOVR ; UTXEN ; USIDL ; WAKE ; ABAUD ; BRGH ; 
//    U1MODE = 0x0;
//    // STSEL 1 Stop bit sent, 1 checked at RX; BCLKMOD enabled; SLPEN ; FLO ; BCLKSEL FOSC/2; C0EN ; RUNOVF ; UTXINV ; URXINV ; HALFDPLX ; 
//    U1MODEH = 0x800;
//    // OERIE ; RXBKIF ; RXBKIE ; ABDOVF ; OERR ; TXCIE ; TXCIF ; FERIE ; TXMTIE ; ABDOVE ; CERIE ; CERIF ; PERIE ; 
//    U1STA = 0x80;
//    // URXISEL ; UTXBE ; UTXISEL ; URXBE ; STPMD ; TXWRE ; 
//    U1STAH = 0x2E;
//    // BaudRate 115207.37; Frequency 100000000 Hz; BRG 868; 
//    U1BRG = 0x364;
//    // BRG 0; 
//    U1BRGH = 0x0;
//    
//    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
//    U1MODEbits.UTXEN = 1;
//    U1MODEbits.URXEN = 1;
  
// TODO: melody settings above
  
// TODO: MCC settings below
  
/**    
     Set the UART1 module to the options selected in the user interface.
     Make sure to set LAT bit corresponding to TxPin as high before UART initialization
*/
    // URXEN disabled; RXBIMD RXBKIF flag when Break makes low-to-high transition after being low for at least 23/11 bit periods; UARTEN enabled; MOD Asynchronous 8-bit UART; UTXBRK disabled; BRKOVR TX line driven by shifter; UTXEN disabled; USIDL disabled; WAKE disabled; ABAUD disabled; BRGH enabled; 
    // Data Bits = 8; Parity = None; Stop Bits = 1 Stop bit sent, 1 checked at RX;
    U1MODE = (0x8080 & ~(1<<15));  // disabling UARTEN bit
    // STSEL 1 Stop bit sent, 1 checked at RX; BCLKMOD disabled; SLPEN disabled; FLO Off; BCLKSEL FOSC/2; C0EN disabled; RUNOVF disabled; UTXINV disabled; URXINV disabled; HALFDPLX disabled; 
    U1MODEH = 0x00;
    // OERIE disabled; RXBKIF disabled; RXBKIE disabled; ABDOVF disabled; OERR disabled; TXCIE disabled; TXCIF disabled; FERIE disabled; TXMTIE disabled; ABDOVE disabled; CERIE disabled; CERIF disabled; PERIE disabled; 
    U1STA = 0x00;
    // URXISEL RX_ONE_WORD; UTXBE enabled; UTXISEL TX_BUF_EMPTY; URXBE enabled; STPMD disabled; TXWRE disabled; 
    U1STAH = 0x22;
    // BaudRate = 115200; Frequency = 100000000 Hz; BRG 216; 
    U1BRG = 0xD8;
    // BRG 0; 
    U1BRGH = 0x00;
    // P1 0; 
    U1P1 = 0x00;
    // P2 0; 
    U1P2 = 0x00;
    // P3 0; 
    U1P3 = 0x00;
    // P3H 0; 
    U1P3H = 0x00;
    // TXCHK 0; 
    U1TXCHK = 0x00;
    // RXCHK 0; 
    U1RXCHK = 0x00;
    // T0PD 1 ETU; PTRCL disabled; TXRPT Retransmit the error byte once; CONV Direct logic; 
    U1SCCON = 0x00;
    // TXRPTIF disabled; TXRPTIE disabled; WTCIF disabled; WTCIE disabled; BTCIE disabled; BTCIF disabled; GTCIF disabled; GTCIE disabled; RXRPTIE disabled; RXRPTIF disabled; 
    U1SCINT = 0x00;
    // ABDIF disabled; WUIF disabled; ABDIE disabled; 
    U1INT = 0x00;
    
    U1MODEbits.UARTEN = 1;   // enabling UART ON bit
    U1MODEbits.UTXEN = 1;
    U1MODEbits.URXEN = 1;    
// TODO: MCC settings above
}

void UART1_Deinitialize(void)
{
    U1MODE = 0x0;
    U1MODEH = 0x0;
    U1STA = 0x80;
    U1STAH = 0x2E;
    U1BRG = 0x0;
    U1BRGH = 0x0;
}

uint8_t UART1_Read(void)
{
    while((U1STAHbits.URXBE == 1))
    {
        
    }

    if ((U1STAbits.OERR == 1))
    {
        U1STAbits.OERR = 0;
    }
    
    return U1RXREG;
}

void UART1_Write(uint8_t txData)
{
    while(U1STAHbits.UTXBF == 1)
    {
        
    }

    U1TXREG = txData;    // Write the data byte to the USART.
}

bool UART1_IsRxReady(void)
{
    return (U1STAHbits.URXBE == 0);
}

bool UART1_IsTxReady(void)
{
    return ((!U1STAHbits.UTXBF) && U1MODEbits.UTXEN);
}

bool UART1_IsTxDone(void)
{
    return (bool)(U1STAbits.TRMT && U1STAHbits.UTXBE);
}

void UART1_TransmitEnable(void)
{
    U1MODEbits.UTXEN = 1;
}

void UART1_TransmitDisable(void)
{
    U1MODEbits.UTXEN = 0;
}

void UART1_AutoBaudSet(bool enable)
{
    U1INTbits.ABDIF = 0U;
    U1INTbits.ABDIE = enable;
    U1MODEbits.ABAUD = enable;
}

bool UART1_AutoBaudQuery(void)
{
    return U1MODEbits.ABAUD;
}

bool UART1_AutoBaudEventEnableGet(void)
{ 
    return U1INTbits.ABDIE; 
}

size_t UART1_ErrorGet(void)
{
    uartError.status = 0;
    if(U1STAbits.FERR == 1U)
    {
        uartError.status = uartError.status|UART_ERROR_FRAMING_MASK;
    }
    if(U1STAbits.PERR== 1U)
    {
        uartError.status = uartError.status|UART_ERROR_PARITY_MASK;
    }
    if(U1STAbits.OERR== 1U)
    {
        uartError.status = uartError.status|UART_ERROR_RX_OVERRUN_MASK;
        U1STAbits.OERR = 0;
    }
    if(U1STAbits.TXCIF== 1U)
    {
        uartError.status = uartError.status|UART_ERROR_TX_COLLISION_MASK;
        U1STAbits.TXCIF = 0;
    }
    if(U1STAbits.ABDOVF== 1U)
    {
        uartError.status = uartError.status|UART_ERROR_AUTOBAUD_OVERFLOW_MASK;
        U1STAbits.ABDOVF = 0;
    }
    
    return uartError.status;
}

void UART1_BRGCountSet(uint32_t brgValue)
{
    U1BRG = brgValue & 0xFFFFU;
    U1BRGH = (brgValue >>16U) & 0x000FU;
}

uint32_t UART1_BRGCountGet(void)
{
    uint32_t brgValue;
    
    brgValue = U1BRGH;
    brgValue = (brgValue << 16U) | U1BRG;
    
    return brgValue;
}

void UART1_BaudRateSet(uint32_t baudRate)
{
    uint32_t brgValue;
    
    if((baudRate >= UART1_MIN_ACHIEVABLE_BAUD_WITH_FRACTIONAL) && (baudRate != 0))
    {
        U1MODEHbits.BCLKMOD = 1;
        U1MODEbits.BRGH = 0;
        brgValue = UART1_BAUD_TO_BRG_WITH_FRACTIONAL(baudRate);
    }
    else if(baudRate >= UART1_MIN_ACHIEVABLE_BAUD_WITH_BRGH_1)
    {
        U1MODEHbits.BCLKMOD = 0;
        U1MODEbits.BRGH = 1;
        brgValue = UART1_BAUD_TO_BRG_WITH_BRGH_1(baudRate);
    }
    else
    {
        U1MODEHbits.BCLKMOD = 0;
        U1MODEbits.BRGH = 0;
        brgValue = UART1_BAUD_TO_BRG_WITH_BRGH_0(baudRate);
    }
    U1BRG = brgValue & 0xFFFFU;
    U1BRGH = (brgValue >>16U) & 0x000FU;
}

uint32_t UART1_BaudRateGet(void)
{
    uint32_t brgValue;
    uint32_t baudRate;
    
    brgValue = UART1_BRGCountGet();
    if((U1MODEHbits.BCLKMOD == 1) && (brgValue != 0))
    {
        baudRate = UART1_BRG_TO_BAUD_WITH_FRACTIONAL(brgValue);
    }
    else if(U1MODEbits.BRGH == 1)
    {
        baudRate = UART1_BRG_TO_BAUD_WITH_BRGH_1(brgValue);
    }
    else
    {
        baudRate = UART1_BRG_TO_BAUD_WITH_BRGH_0(brgValue);
    }
    return baudRate;
}
