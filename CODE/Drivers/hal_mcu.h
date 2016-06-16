/***********************************************************************************
  Filename:     hal_mcu.h

  Description:  hal mcu library header file

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef HAL_MCU_H
#define HAL_MCU_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************
* INCLUDES
*/

#include <hal_types.h>


/***********************************************************************************
 * CONSTANTS AND DEFINES
 */
enum {                  // Input values to halMcuSetLowPowerMode()
    HAL_MCU_LPM_0,
    HAL_MCU_LPM_1,
    HAL_MCU_LPM_2,
    HAL_MCU_LPM_3,
    HAL_MCU_LPM_4
};


enum {                  // Return values of halMcuGetResetCause()
    HAL_MCU_RESET_CAUSE_POR,
    HAL_MCU_RESET_CAUSE_EXTERNAL,
    HAL_MCU_RESET_CAUSE_WATCHDOG
};


/***********************************************************************************
 * GLOBAL FUNCTIONS
 */

void halMcuInit(void);
void halMcuWaitUs(uint16 usec);
void halMcuWaitMs(uint16 msec);
void halMcuSetLowPowerMode(uint8 mode);
void halMcuReset(void);
uint8 halMcuGetResetCause(void);


#ifdef  __cplusplus
}
#endif

/**********************************************************************************/
#endif
