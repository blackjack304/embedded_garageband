/******************************************************************************
    Filename: hal_spi.h

    This file defines Low Level I/O routines for the SPI Interface of cc2430.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef _SPILIB_H
#define _SPILIB_H

#include "hal_board.h"

//----------------------------------------------------------------------------
//  These constants are used to identify the chosen SPI and UART
//  interfaces.
//----------------------------------------------------------------------------
#define SER_INTF_NULL    0
#define SER_INTF_USART0  1
#define SER_INTF_BITBANG 8


//----------------------------------------------------------------------------
// SPI/UART port selections.  Select which port will be used for the interface 
//----------------------------------------------------------------------------
#define SPI_SER_INTF      SER_INTF_USART0  // Interface to MMC
//#define SPI_SER_INTF      SER_INTF_BITBANG  // Interface to MMC




#define DUMMY_CHAR 0xFF



#if SPI_SER_INTF == SER_INTF_USART0
 #define U0CSR_TX_BYTE 0x02
 #define halSPIRXBUF  U0DBUF
 #define halSPI_SEND(x) U0DBUF=x
 #define halSPITXREADY  (U0CSR&0x01)         /* Wait for TX to be ready */
 #define halSPITXDONE  ((U0CSR&0x02)>>1)          /* Wait for TX to finish */
 #define halSPIRXREADY (URX0IF)          /* Wait for TX to be ready */
 //#define halSPI_PxIN  SPI_USART0_PxIN
 //#define halSPI_SOMI  SPI_USART0_SOMI

#elif SPI_SER_INTF == SER_INTF_BITBANG
 #define halSPIRXBUF  spi_bitbang_in_data
 #define halSPI_SEND(x) spi_bitbang_inout(x)
 #define halSPITXREADY  0
 #define halSPITXDONE   1
 #define halSPIRXREADY  1
 #define halSPIRXFG_CLR
 #define halSPI_PxIN  SPI_BITBANG_PxIN
 #define halSPI_SOMI  SPI_BITBANG_SOMI
#endif


// Varialbes


// Function Prototypes
void halSPISetup (void);
unsigned char spiSendByte(const unsigned char data);
unsigned char spiReadFrame(unsigned char* pBuffer, unsigned int size);
unsigned char spiSendFrame(unsigned char* pBuffer, unsigned int size);
unsigned char SPI_transmit(const unsigned char data);


/******************************************************************************
*******************  USART-UART specific functions/macros   *******************
******************************************************************************/
// The macros in this section simplify UART operation.
#define BAUD_E(baud, clkDivPow) (     \
    (baud==2400)   ?  6  +clkDivPow : \
    (baud==4800)   ?  7  +clkDivPow : \
    (baud==9600)   ?  8  +clkDivPow : \
    (baud==14400)  ?  8  +clkDivPow : \
    (baud==19200)  ?  9  +clkDivPow : \
    (baud==28800)  ?  9  +clkDivPow : \
    (baud==38400)  ?  10 +clkDivPow : \
    (baud==57600)  ?  10 +clkDivPow : \
    (baud==76800)  ?  11 +clkDivPow : \
    (baud==115200) ?  11 +clkDivPow : \
    (baud==153600) ?  12 +clkDivPow : \
    (baud==230400) ?  12 +clkDivPow : \
    (baud==307200) ?  13 +clkDivPow : \
    0  )


#define BAUD_M(baud) (      \
    (baud==2400)   ?  59  : \
    (baud==4800)   ?  59  : \
    (baud==9600)   ?  59  : \
    (baud==14400)  ?  216 : \
    (baud==19200)  ?  59  : \
    (baud==28800)  ?  216 : \
    (baud==38400)  ?  59  : \
    (baud==57600)  ?  216 : \
    (baud==76800)  ?  59  : \
    (baud==115200) ?  216 : \
    (baud==153600) ?  59  : \
    (baud==230400) ?  216 : \
    (baud==307200) ?  59  : \
  0)


// Options for the SPI_SETUP macro.
#define SPI_SLAVE              0x01
#define SPI_MASTER             0x00
#define SPI_CLOCK_POL_LO       0x00
#define SPI_CLOCK_POL_HI       0x80
#define SPI_CLOCK_PHA_0        0x00
#define SPI_CLOCK_PHA_1        0x40
#define SPI_TRANSFER_MSB_FIRST 0x20
#define SPI_TRANSFER_MSB_LAST  0x00


#endif /* _SPILIB_H */
