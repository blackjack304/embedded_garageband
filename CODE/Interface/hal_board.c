
/***********************************************************************************
  Filename:     hal_board.c
    
  Description:  Initalize board peripherals
  Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma    
***********************************************************************************/

#include "hal_board.h"
#include "hal_mcu.h"
#include "hal_int.h"

void halBoardInit(void)
{
    halMcuInit();

    // LED
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_1_PORT, HAL_BOARD_IO_LED_1_PIN, 0);
    MCU_IO_OUTPUT(HAL_BOARD_IO_LED_3_PORT, HAL_BOARD_IO_LED_3_PIN, 0);

    HAL_LED_CLR_1();
    HAL_LED_CLR_3();

    // Button
   // MCU_IO_INPUT(0, 1, MCU_IO_TRISTATE);

    halIntOn();
}
