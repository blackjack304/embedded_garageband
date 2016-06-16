/******************************************************************************
  Filename:     hal_int.h

  Description:  HAL interrupt control header file

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/


#ifndef HAL_INT_H
#define HAL_INT_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * INCLUDES
 */

#include <hal_types.h>


/******************************************************************************
 * MACROS
 */

#if (defined __ICC430__) || defined(__MSP430__)

// Use the macros below to reduce function call overhead for common
// global interrupt control functions


#if (defined __ICC430__)
#define HAL_INT_ON(x)      st( __enable_interrupt(); )
#define HAL_INT_OFF(x)     st( __disable_interrupt(); )
#define HAL_INT_LOCK(x)    st( (x) = __get_interrupt_state(); \
                               __disable_interrupt(); )
#define HAL_INT_UNLOCK(x)  st( __set_interrupt_state(x); )
#endif

#if (defined __MSP430__)
#define HAL_INT_ON(x)      st( _enable_interrupts(); )
#define HAL_INT_OFF(x)     st( _disable_interrupts(); )
#define HAL_INT_LOCK(x)    st( (x) = _get_SR_register(); \
                               _disable_interrupts(); )
#define HAL_INT_UNLOCK(x)  st( _enable_interrupts(); /*_bis_SR_register(x);*/ )
#endif

#elif defined __ICC8051__

#define HAL_INT_ON(x)      st( EA = 1; )
#define HAL_INT_OFF(x)     st( EA = 0; )
#define HAL_INT_LOCK(x)    st( (x) = EA; EA = 0; )
#define HAL_INT_UNLOCK(x)  st( EA = (x); )

typedef unsigned short istate_t;

#elif defined WIN32

#define HAL_INT_ON()
#define HAL_INT_OFF()
#define HAL_INT_LOCK(x)    st ((x)= 1; )
#define HAL_INT_UNLOCK(x)

#elif defined __KEIL__

#define HAL_INT_ON(x)      st( EA = 1; )
#define HAL_INT_OFF(x)     st( EA = 0; )
#define HAL_INT_LOCK(x)    st( (x) = EA; EA = 0; )
#define HAL_INT_UNLOCK(x)  st( EA = (x); )



#else
#error "Unsupported compiler"
#endif


/******************************************************************************
 * GLOBAL FUNCTIONS
 */

void   halIntOn(void);
void   halIntOff(void);
uint16 halIntLock(void);
void   halIntUnlock(uint16 key);


#ifdef  __cplusplus
}
#endif

#endif
