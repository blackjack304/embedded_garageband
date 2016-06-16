/***********************************************************************************
  Filename:     hal_mcu.c

  Description:  hal mcu library

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/


#include "hal_types.h"
#include "hal_mcu.h"
#include <ioCC2431.h>
#include "clock.h"


/***********************************************************************************
* @fn          halMcuInit
*
* @brief       Set system clock
*
* @param       none
*
* @return      none
*/
void halMcuInit(void)
{
    clockSetMainSrc(CLOCK_SRC_XOSC);
}


/***********************************************************************************
* @fn          halMcuWaitUs
*
* @brief       Busy wait function. Waits the specified number of microseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions. This function assumes a 32 MHz clock.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 usec - number of microseconds delays
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitUs(uint16 usec)
{
    usec>>= 1;

    while(usec-->0)
    {
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
}

/***********************************************************************************
* @fn          halMcuWaitMs
*
* @brief       Busy wait function. Waits the specified number of milliseconds. Use
*              assumptions about number of clock cycles needed for the various
*              instructions.
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint16 millisec - number of milliseconds delay
*
* @return      none
*/
#pragma optimize=none
void halMcuWaitMs(uint16 msec)
{
    while(msec--)
        halMcuWaitUs(1000);
}

/***********************************************************************************
* @fn          halMcuSetLowPowerMode
*
* @brief      Sets the MCU in a low power mode. Will turn global interrupts on at
*             the same time as entering the LPM mode. The MCU must be waken from
*             an interrupt (status register on stack must be modified).
*
*              NB! This function is highly dependent on architecture and compiler!
*
* @param       uint8 mode - power mode
*
* @return      none
*/
void halMcuSetLowPowerMode(uint8 mode)
{
  // comment: not yet implemented
  //HAL_ASSERT(FALSE);
}


/******************************************************************************
* @fn  halMcuReset
*
* @brief
* Resets the MCU. This utilize the watchdog timer as there is no other way
* for a software reset. The reset will not occur until ~2 ms.
* NB: The function will not return! (hangs until reset)
*
* Parameters:
*
* @param  void
*
* @return void
*
******************************************************************************/
void halMcuReset(void)
{
    const uint8 WDT_INTERVAL_MSEC_2=   0x03;   // after ~2 ms

    WDCTL = ((WDCTL & 0xFC) | (WDT_INTERVAL_MSEC_2 & 0x03));
    // Start watchdog
    WDCTL &= ~0x04;     // Select watchdog mode
    WDCTL |= 0x08;      // Enable timer
    while(1);                                   // Halt here until reset
}