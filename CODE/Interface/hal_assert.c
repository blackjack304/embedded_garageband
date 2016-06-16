
/***********************************************************************************
  Filename:     hal_assert.c
    
  Description:  Assert functionality
  
  Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma    
***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include "hal_assert.h"
#include "hal_led.h"
#include "hal_mcu.h"
#include "hal_defs.h"


/***********************************************************************************
* GLOBAL FUNCTIONS
*/

/***********************************************************************************
* @fn          halAssertHandler
*
* @brief       Logic to handle an assert.
*
* @param       none
*
* @return      none
***********************************************************************************
*/
void halAssertHandler(void)
{
    // execute code that handles asserts 
    // blink all leds
    while(TRUE){
        halLedToggle(1);
        halLedToggle(2);
        halLedToggle(3);
        halLedToggle(4);
        halMcuWaitMs(50);
    }
}

