/***********************************************************************************

  Filename:     hal_board.h

  Description:  SmartRF04 board with CC2430EM/CC2510EM/CC1110EM/CC2530EM.
  Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#ifndef HAL_BOARD_H
#define HAL_BOARD_H
#include "ioCC2430.h"
#include "ioCC2431.h"
#include "hal_cc8051.h"
#include "hal_defs.h"
#include "hal_mcu.h"

/***********************************************************************************
 * CONSTANTS
 */

// Board properties
#define BOARD_NAME                          "SRF04EB"
#define NUM_LEDS                            2
#define NUM_BUTTONS                         1
#define NUM_JSTKS                           1
#define NUM_POTS                            2

// Clock
#define BSP_CONFIG_CLOCK_MHZ                32


// LEDs
#define HAL_BOARD_IO_LED_1_PORT             1   // Green
#define HAL_BOARD_IO_LED_1_PIN              0
#define HAL_BOARD_IO_LED_3_PORT             1   // Yellow
#define HAL_BOARD_IO_LED_3_PIN              1
// LED2 and LED4 are not connected to the CC2430


// Buttons
#define HAL_BOARD_IO_BTN_1_PORT             0   // Button S1
#define HAL_BOARD_IO_BTN_1_PIN              1


#define SPI_SIMO          0x08
#define SPI_SOMI          0x04
#define SPI_UCLK          0x20

// Chip Select
#define MMC_CS_PORT       0
#define MMC_CS_PIN        4

// Card Detect
#define MMC_CD_PORT       0
#define MMC_CD_PIN        0
#define MMC_CD            0x01

// Keyboard konnections
#define KDATA       P2_1
#define KPORT        2
#define KCLOCK_PIN   2
#define KDATA_PIN    1



#define CS_LOW()    MCU_IO_OUTPUT(MMC_CS_PORT,MMC_CS_PIN,0)               // Card Select
#define CS_HIGH()   while(!halSPITXDONE); MCU_IO_OUTPUT(MMC_CS_PORT,MMC_CS_PIN,1)  // Card Deselect

// Debounce
#define HAL_DEBOUNCE(expr)    { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }

/***********************************************************************************
 * MACROS
 */

// LEDs
#define HAL_LED_SET_1() MCU_IO_SET_LOW(HAL_BOARD_IO_LED_1_PORT, \
    HAL_BOARD_IO_LED_1_PIN)
#define HAL_LED_SET_3() MCU_IO_SET_LOW(HAL_BOARD_IO_LED_3_PORT, \
    HAL_BOARD_IO_LED_3_PIN)

#define HAL_LED_CLR_1() MCU_IO_SET_HIGH(HAL_BOARD_IO_LED_1_PORT, \
    HAL_BOARD_IO_LED_1_PIN)
#define HAL_LED_CLR_3() MCU_IO_SET_HIGH(HAL_BOARD_IO_LED_3_PORT, \
    HAL_BOARD_IO_LED_3_PIN)

#define HAL_LED_TGL_1() MCU_IO_TGL(HAL_BOARD_IO_LED_1_PORT, \
    HAL_BOARD_IO_LED_1_PIN)
#define HAL_LED_TGL_3() MCU_IO_TGL(HAL_BOARD_IO_LED_3_PORT, \
    HAL_BOARD_IO_LED_3_PIN)


// Buttons
#define HAL_BUTTON_1_PUSHED() (!MCU_IO_GET(HAL_BOARD_IO_BTN_1_PORT, \
    HAL_BOARD_IO_BTN_1_PIN))

// UART RTS
#define HAL_RTS_SET()       MCU_IO_SET_HIGH(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN)
#define HAL_RTS_CLR()       MCU_IO_SET_LOW(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN)
#define HAL_RTS_DIR_OUT()   MCU_IO_OUTPUT(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN, 1)

// Macro for getting the clock division factor
#define CLKSPD  ( CLKCON & 0x01 )


// Macro for getting the timer tick division factor
#define TICKSPD ((CLKCON & 0x38) >> 3)

// Macro for checking status of the crystal oscillator
#define XOSC_STABLE (SLEEP & 0x40)

// Macro for checking status of the high frequency RC oscillator.
#define HIGH_FREQUENCY_RC_OSC_STABLE    (SLEEP & 0x20)


// HAL processing not required for this board
#define HAL_PROCESS()




/***********************************************************************************
 * FUNCTION PROTOTYPES
 */
void halBoardInit(void);

#endif
