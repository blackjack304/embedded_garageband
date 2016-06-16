/******************************************************************************
    Filename: hal_spi.c

    This file defines Low Level I/O routines for the SPI Interface of cc2430.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef _SPILIB_C
#define _SPILIB_C


#include "hal_SPI.h"
#include "hal_board.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"


#ifndef DUMMY_CHAR
#define DUMMY_CHAR 0xFF
#endif


/******************************************************************************
* @fn     halSPISetup
*
* @brief  Configures the assigned interface to function as a SPI port and initializes it.
*
* @param  void
*
* @return void
*
******************************************************************************/
void halSPISetup(void)
{
    U0CSR |= 0X40;				
    U0UCR = 0x80;
    U0CSR = 0x00; 
    P0SEL |= (SPI_SIMO + SPI_SOMI + SPI_UCLK);
    
    PERCFG |= 0x00;
    
    //IO_DIR_PORT_PIN(0,3,IO_OUT); // txd / MOSI
    //IO_DIR_PORT_PIN(0,2,IO_IN); // Rxd / MISO
    //IO_DIR_PORT_PIN(0,4,IO_OUT); // CTS / SS_N
    //IO_DIR_PORT_PIN(0,5,IO_OUT); // txd / SCK
    
    U0GCR = BAUD_E(115200, CLKSPD);
    U0BAUD = BAUD_M(115200);
    
     //Configure SPI Polarity, Clock Phase, and Bit Order
    U0CSR =   U0CSR & ~0x80;    // Set U0CSR bit 7 to 0 for SPI Mode
    U0CSR =   U0CSR & ~0x20;    // Set U0CSR bit 5 to 0 for Master
    U0GCR =  (U0GCR | 0x80);   // Set U0GCR bit 7   to 1 for CPOL = 1
    U0GCR =  (U0GCR | 0x20);    // Set U0GCR bit 5 to 1 for MSB first
    U0GCR =  (U0GCR & ~0x40);    // Set U0GCR bit 6 to 0 for CPHA = 0
    
    U0CSR |= 0x40;	
    
    URX0IF = 0;

    halSPI_SEND(DUMMY_CHAR);
}


/******************************************************************************
* @fn     spiSendByte
*
* @brief  Writes a single byte and returns the value read.
*
* @param  const unsigned char data
*			1-byte data to be written
*
* @return 1 Byte data read from SPI.
*
******************************************************************************/
unsigned char spiSendByte(const unsigned char data)
{
  while (halSPITXREADY);    // wait while not ready for TX
  halSPI_SEND(data);            // write
  while(!(U0CSR & U0CSR_TX_BYTE));            // wait
  U0CSR &= ~U0CSR_TX_BYTE;                       // clear flag
  while (halSPIRXREADY ==0);    // wait for RX buffer (full)
  return (halSPIRXBUF);
}



/******************************************************************************
* @fn     spiReadFrame
*
* @brief  Read a frame of bytes via SPI.
*
* @param  unsigned char* pBuffer
*			Ptr to buffer where data to be stored
*		  unsigned int size
*			Total no. of bytes to be read
*
* @return 0 if successful.
*
******************************************************************************/
unsigned char spiReadFrame(unsigned char* pBuffer, unsigned int size)
{
  unsigned long i = 0;
  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++){
    while (halSPITXREADY);   // wait while not ready for TX
    halSPI_SEND(DUMMY_CHAR);     // dummy write
    while(!(U0CSR & U0CSR_TX_BYTE));            // wait
    U0CSR &= ~U0CSR_TX_BYTE;                       // clear fl
    pBuffer[i] = halSPIRXBUF;
  }
  return(0);
}


/******************************************************************************
* @fn     spiSendFrame
*
* @brief  Send a frame of bytes via SPI.
*
* @param  unsigned char* pBuffer
*			Ptr to buffer of data to be written
*		  unsigned int size
*			Total no. of bytes to be written
*
* @return 0 if successful.
*
******************************************************************************/
unsigned char spiSendFrame(unsigned char* pBuffer, unsigned int size)
{
  unsigned long i = 0;
  // clock the actual data transfer and receive the bytes; spi_read automatically finds the Data Block
  for (i = 0; i < size; i++){
    while (halSPITXREADY);   // wait while not ready for TX
    halSPI_SEND(pBuffer[i]);     // write
    while(!(U0CSR & U0CSR_TX_BYTE));            // wait
    U0CSR &= ~U0CSR_TX_BYTE;                       // clear fl
    pBuffer[i] = halSPIRXBUF;
  }
  return(0);
}


/******************************************************************************
* @fn     SPI_transmit
*
* @brief  Writes a single byte and returns the value read.
*
* @param  const unsigned char data
*			1-byte data to be written
*
* @return 1 Byte data read from SPI.
*
******************************************************************************/
unsigned char SPI_transmit(const unsigned char data)
{
  while (halSPITXREADY);    // wait while not ready for TX
  halSPI_SEND(data);            // write
  while(!(U0CSR & U0CSR_TX_BYTE));            // wait
  U0CSR &= ~U0CSR_TX_BYTE;                       // clear flag
  while (halSPIRXREADY ==0);    // wait for RX buffer (full)
  return (halSPIRXBUF);
}

//---------------------------------------------------------------------
#endif /* _SPILIB_C */
