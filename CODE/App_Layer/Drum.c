/***********************************************************************************
  Filename:     Drum.c

  Description:  Application/Driver layer for playing actual Drums using Peizo Sensors connected to Port Pins.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

#include <hal_led.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "ioCC2430.h"
#include "uart_intfc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TimerManager.h"
#include "key_map.h"
#include "App_RF.h"
#include "TouchScreen.h"
#include "Drum.h"



//--------------------------------------------------------------------------
//  Timers:
//--------------------------------------------------------------------------
TimerId timerID_DR[6] = {TimerId_INVALID, TimerId_INVALID, TimerId_INVALID, TimerId_INVALID, TimerId_INVALID, TimerId_INVALID};

volatile unsigned int Capture_DR[6] = {0, 0, 0, 0, 0, 0};

char tx1_buf[5];
volatile unsigned char Print_Flag = 0;

void Drum_ISR(TimerId timerId);
volatile unsigned char tmp_port;
unsigned char inn;


/***********************************************************************************
* @fn      Intitialize_Drumset
*
* @brief   Initialize I/Os and Timers related to Drumset..
*
* @param   none
*
* @return  none
*/
void Intitialize_Drumset()
{
  P0SEL = 0;

  MCU_IO_OUTPUT(DRUM_PORT, DR0_PIN, 1);
  MCU_IO_OUTPUT(DRUM_PORT, DR1_PIN, 1);
  MCU_IO_OUTPUT(DRUM_PORT, DR2_PIN, 1);
  MCU_IO_OUTPUT(DRUM_PORT, DR3_PIN, 1);
  MCU_IO_OUTPUT(DRUM_PORT, DR4_PIN, 1);
  MCU_IO_OUTPUT(DRUM_PORT, DR5_PIN, 1);

  MCU_IO_INPUT(DRUM_PORT, DR0_PIN, MCU_IO_PULLUP);
  MCU_IO_INPUT(DRUM_PORT, DR1_PIN, MCU_IO_PULLUP);
  MCU_IO_INPUT(DRUM_PORT, DR2_PIN, MCU_IO_PULLUP);
  MCU_IO_INPUT(DRUM_PORT, DR3_PIN, MCU_IO_PULLUP);
  MCU_IO_INPUT(DRUM_PORT, DR4_PIN, MCU_IO_PULLUP);
  MCU_IO_INPUT(DRUM_PORT, DR5_PIN, MCU_IO_PULLUP);

  MCU_IO_INPUT(DRUM_PORT, 6, MCU_IO_PULLDOWN);
  MCU_IO_INPUT(DRUM_PORT, 7, MCU_IO_PULLDOWN);


  PICTL |= 0x01;   //High to Low Edge
  PICTL |= 0x18;    //Enable Interrupts P0ENH, P0ENL
  P0IF = 0;
  P0IFG = 0;
  P0IE = 1;

  halLedSet(1);
  halLedSet(3);

}


/***********************************************************************************
* @fn      DrumSet
*
* @brief   Main function for Drums.
*
* @param   none
*
* @return  none
*/
void DrumSet()
{
  Intitialize_Drumset();
  while(1)
  {
    if(P0 & 0x3F)
    {
      tmp_port = P0;
      for(inn=0;inn<6;inn++)
      {
        if((tmp_port>>inn) & 0x01)
           break;
      }
      if(inn<6 && timerID_DR[inn] == TimerId_INVALID)
      {
         timerID_DR[inn] = SetTimerReq(&Drum_ISR,200);
         halLedToggle(1);
         P0IFG &= ~(1<<inn);
         Capture_DR[inn] = 0;
      }
    }

    if(Print_Flag == 1)
    {
      tx1_send(tx1_buf,3);
      Print_Flag = 0;
    }
  }
}


/***********************************************************************************
* @fn      Drum_ISR
*
* @brief   ISR for Drum Timer
*
* @param   none
*
* @return  none
*/
void Drum_ISR(TimerId timerId)
{
  unsigned char i=0;
  for(i=0; i<6; i++)
  {
    if( timerId == timerID_DR[i])
    {
      timerID_DR[i] = TimerId_INVALID;
      Capture_DR[i] = 200 - Capture_DR[i];
      tx1_buf[0] = 0x80;
      tx1_buf[1] = i;
      tx1_buf[2] = (unsigned char)(Capture_DR[i] & 0x00ff);
      Print_Flag = 1;
      break;
    }
  }
}


/***********************************************************************************
* @fn      port0_ISR
*
* @brief   ISR framework for P0 digio interrupt
*
* @param   none
*
* @return  none
*/
HAL_ISR_FUNCTION(port0_ISR,P0INT_VECTOR)
{
  static uint8 isr_count = 0;
  static int criticalSectionSave;
  unsigned char i = 0;


  BSP_ENTER_CRITICAL_SECTION( criticalSectionSave );
  for(i=0;i<6;i++)
  {
    if((P0IFG>>i) & 0x01)
       break;
  }

  if(i<6 && timerID_DR[i] != TimerId_INVALID)
  {
    Capture_DR[i] = Capture_TimerReq(timerID_DR[i]);
  }

  P0IFG = 0;
  isr_count++;
  halLedToggle(3);
  //halMcuWaitUs(500);
  BSP_EXIT_CRITICAL_SECTION( criticalSectionSave );
  P0IF = 0;
}


/***********************************************************************************
* @fn      playVDrum
*
* @brief   Main function for Drums. Initializes DrumSet, monitors for any drum hit and plays corresponding note to VS1053 MIDI Decoder.
*
* @param   none
*
* @return  none
*/
void playVDrum() {
	
	unsigned char usrExit[] = {0};
	char keyNote = 0;
	unsigned int xCoord = 0;
	unsigned int yCoord = 0;
	unsigned char sendBuf[3] = {0};
	
	while(1) {
		//usrExit = getchar() nblocking in M2
		//In this module do a non blocking recv and chk for STOPMAGIC
		if (RF_gets_nblk(usrExit) != 0) {
			if (strncmp ((char *)usrExit, "STOP", 4) == 0) {
				break;
			}
		}
		else {
			getTsXY(&xCoord, &yCoord);
			keyNote = drum_key_match(xCoord, yCoord);
			halMcuWaitMs(30);
			if (keyNote) {
				//send the note over rf and serial
				sendBuf[0] = 0;					//Channel
				sendBuf[1] = keyNote;		//Note
				sendBuf[2] = 0x7f; 			//Attack Velocity //TODO: use pressure
				halMcuWaitMs(50);
				RF_Send(sendBuf, 3);
			}
		}
	}//while(1)
}