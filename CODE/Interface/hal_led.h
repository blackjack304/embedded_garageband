/***********************************************************************************
  Filename:     hal_led.h

  Description:  hal led library header file

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#ifndef HAL_LED_H
#define HAL_LED_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************
* INCLUDES
*/
#include "hal_types.h"

/***********************************************************************************
 * GLOBAL FUNCTIONS
 */
void halLedInit(void);
void halLedSet(uint8 led_id);
void halLedClear(uint8 led_id);
void halLedToggle(uint8 led_id);


#ifdef  __cplusplus
}
#endif

#endif
