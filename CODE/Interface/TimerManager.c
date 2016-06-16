/******************************************************************************
    Filename: TimerManager.c

    This file defines Low/Mid level Timer driver and interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

//=============================================================================
//     INCLUDES AND VARIABLE DEFINITIONS
//=============================================================================
#include "hal_types.h"
#include "hal_defs.h"
#include "clock.h"
#include "hal_board.h"
#include "hal_int.h"
#include "hal_assert.h"
#include "ioCC2430.h"
#include "uart.h"
#include "TimerManager.h"

//-----------------------------------------------------------------------------
//     Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//     Include Files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//     Type Declarations
//-----------------------------------------------------------------------------
typedef struct
{
    uint16          Required_Period_Sec;
    TimerExpiredCallback  Callback;
} TimerInst;


//-----------------------------------------------------------------------------
//     Global Constant Definitions
//-----------------------------------------------------------------------------
static const uint16                     PERIOD_INVALID = 0xFFFF;
;

//-----------------------------------------------------------------------------
//     Global Data Definitions
//-----------------------------------------------------------------------------
TimerInst               Timers[ MAX_TIMERS ];

//-----------------------------------------------------------------------------
//     Static Variable Definitions
//-----------------------------------------------------------------------------
//static bool                             InTimerISR = false;

//-----------------------------------------------------------------------------
//     Function Prototypes
//-----------------------------------------------------------------------------





//=============================================================================
//     FUNCTION DEFINITIONS
//=============================================================================


/*============================================================================= 
  FUNCTION:     TimerInterface_Initialize

  DESCRIPTION:
   Initializes basic Timer to give 1mSec Interrupt.

RETURN VALUE: 
   Fill out this section.

SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
bool TimerInterface_Initialize()
{
    for( int i=0; i<MAX_TIMERS; i++ )
    {
        Timers[ i ].Required_Period_Sec = PERIOD_INVALID;
    }
    
    T1CCTL0 = 0;    /* Make sure interrupts are disabled */
    T1CCTL1 = 0;    /* Make sure interrupts are disabled */
    T1CCTL2 = 0;    /* Make sure interrupts are disabled */
    T1CTL   = 0;
    
    // clear interrupt pending flag, disable interrupt
    T1IE  =  0; // IEN1.T1EN = 0
    
    CLKCON &= ~TICKSPD_DIV_1;
    CLKCON |= TICKSPD_DIV_1;
    
    // Set prescaler divider value to 128
    T1CTL |= 0x0C;
    
    T1CC0L =  240;     // Reset counter value
    T1CC0H =  0;     // Reset counter value
    
    T1IE  =  1; // IEN1.T1EN = 0
    OVFIM = 1;
    
    // Start timer in up/down mode
    T1CTL |= 0x02; // T1CTL.MODE = 01  
    
    return true;
}


/*============================================================================= 
  FUNCTION:     SetTimerReq
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
TimerId SetTimerReq
(
   TimerExpiredCallback callbackPtr,
   unsigned int Period_Sec
)
{
    int criticalSectionSave;
    //find the 1st available timer
    unsigned int foundID = TimerId_INVALID;
    for( unsigned int i=0; i<MAX_TIMERS; i++ )
    {
        if( Timers[ i ].Required_Period_Sec == PERIOD_INVALID && foundID == TimerId_INVALID)
        {
            foundID = i;
            break;
        }
    }
    BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );

    //set callback and time
    Timers[ foundID ].Required_Period_Sec = Period_Sec;
    Timers[ foundID ].Callback = callbackPtr;
    BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );

    return (TimerId)foundID;
}


/*============================================================================= 
  FUNCTION:     CancelTimerReq
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
bool CancelTimerReq
(
   TimerId timerId
)
{
    unsigned int criticalSectionSave;
    if( timerId == TimerId_INVALID || timerId > MAX_TIMERS || Timers[ timerId ].Required_Period_Sec == PERIOD_INVALID)
    {
        return false;
    }
    BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );
    Timers[ timerId ].Required_Period_Sec = PERIOD_INVALID;
    Timers[ timerId ].Callback = 0;
    BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );
    return true;
}


/*============================================================================= 
  FUNCTION:     CancelTimerReq
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
unsigned int Capture_TimerReq
(
   TimerId timerId
)
{
    //word16 criticalSectionSave;
    unsigned int CapturValue;
    //BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );
    CapturValue = Timers[ timerId ].Required_Period_Sec;
    //BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );
    return CapturValue;
}


/*============================================================================= 
  FUNCTION:     Reschedule_TimerReq
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
bool Reschedule_TimerReq
(
  TimerId timerId, 
  unsigned int Old_Period_Sec,
  unsigned int New_Period_Sec
)
{
    unsigned int criticalSectionSave;
    int elapsed_time;
    
    if( timerId == TimerId_INVALID || timerId > MAX_TIMERS || Timers[ timerId ].Required_Period_Sec == PERIOD_INVALID)
    {
        return false;
    }
    
    
    elapsed_time = Old_Period_Sec - Timers[ timerId ].Required_Period_Sec; //Timer is already running. Calculate how much time is elapsed
    
    BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );
    if(elapsed_time >= New_Period_Sec)
      Timers[ timerId ].Required_Period_Sec = 1; //Already got the desired delay, Call ISR immediately in the next iteration.
    else 
      Timers[ timerId ].Required_Period_Sec = New_Period_Sec - elapsed_time;  //compensate for already elapsed time delay
    BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );
    
    return true;
}


/*============================================================================= 
  FUNCTION:     One_Sec_ISR
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
void One_Sec_ISR()
{
  unsigned int criticalSectionSave;
  
     BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );
    //call all the expired timer callbacks
    for( int i=0; i<MAX_TIMERS; i++ )
    {
      if( Timers[ i ].Required_Period_Sec != PERIOD_INVALID )
      {
        Timers[ i ].Required_Period_Sec--;
        if( Timers[ i ].Required_Period_Sec == 0 )
        {
            if( Timers[ i ].Callback != 0 )
            {
                TimerExpiredCallback callback = *(Timers[ i ].Callback);
                Timers[ i ].Required_Period_Sec = PERIOD_INVALID;
                Timers[ i ].Callback = 0;
                callback((TimerId)i );
            }
        }
      }
    }
    BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );
}


/***********************************************************************************
* @fn      timer32k0_ISR
*
* @brief   ISR framework for the 32KHz timer component
*
* @param   none
*
* @return  none
*/

HAL_ISR_FUNCTION(T1_ISR,T1_VECTOR)
{
  One_Sec_ISR();
}