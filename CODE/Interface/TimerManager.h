/******************************************************************************
    Filename: TimerManager.h

    This file defines Low/Mid level Timer driver and interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/
#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

//=============================================================================
//     INCLUDES AND PUBLIC DATA DECLARATIONS
//=============================================================================


//-----------------------------------------------------------------------------
//     Defines
//-----------------------------------------------------------------------------
#define MAX_TIMERS  4
#define TimerId_INVALID 0xFFFF


//-----------------------------------------------------------------------------
//     Defines
//-----------------------------------------------------------------------------



/*-------------------------------------------------------------------
   POINTERS
-------------------------------------------------------------------*/
typedef void (*VoidFuncPtrType)();


//-----------------------------------------------------------------------------
//     Include Files
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//     Type Declarations
//-----------------------------------------------------------------------------
typedef unsigned int TimerId;
typedef void (*TimerExpiredCallback)(TimerId);
typedef void (*BeaconFrame_TimerExpiredCallback)(void);

//-----------------------------------------------------------------------------
//     Global Constant Declarations
//-----------------------------------------------------------------------------


//=============================================================================
//     FUNCTION DECLARATIONS
//=============================================================================



/*============================================================================= 
  FUNCTION:     TimerInterface_Initialize
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
bool TimerInterface_Initialize();




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
);


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
);


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
  unsigned int  Old_Period_Sec,
  unsigned int  New_Period_Sec
);





/*============================================================================= 
  FUNCTION:     One_Sec_ISR
 
  DESCRIPTION:
   Fill out this section.
 
RETURN VALUE: 
   Fill out this section.
 
SIDE EFFECTS: 
   Fill out this section.
=============================================================================*/
void One_Sec_ISR();

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
);


#endif /* end ifndef SRRINTERRUPTSERVICEENGINE_H */