
/***********************************************************************************
  Filename:     hal_assert.h

  Description:  Assert functionality header file

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#ifndef HAL_ASSERT_H
#define HAL_ASSERT_H

#include "hal_defs.h"

/***********************************************************************************
* MACROS
*/
#define HAL_ASSERT(expr)               st( if (!( expr )) halAssertHandler(); )
/***********************************************************************************
* FUNCTIONS
*/
void halAssertHandler(void);

#endif
