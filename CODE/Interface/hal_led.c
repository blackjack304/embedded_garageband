/***********************************************************************************
    Filename: hal_led.c

  Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#include "hal_types.h"
#include "hal_led.h"
#include "hal_board.h"

/***********************************************************************************
* @fn      halLedSet
*
* @brief   Turn LED on.
*
* @param   uint8 id - id of LED
*
* @return  none
*/
void halLedSet(uint8 id)
{
    if (id==1)
        HAL_LED_SET_1();
    if (id==3)
        HAL_LED_SET_3();
}

/***********************************************************************************
* @fn      halLedClear
*
* @brief   Turn LED off.
*
* @param   uint8 id - id of LED
*
* @return  none
*/
void halLedClear(uint8 id)
{
    if (id==1)
        HAL_LED_CLR_1();
    if (id==3)
        HAL_LED_CLR_3();
}

/***********************************************************************************
* @fn      halLedToggle
*
* @brief   Change state of LED. If on, turn it off. Else turn on.
*
* @param   uint8 id - id of LED
*
* @return  none
*/
void halLedToggle(uint8 id)
{
    if (id==1)
        HAL_LED_TGL_1();
    if (id==3)
        HAL_LED_TGL_3();
}

