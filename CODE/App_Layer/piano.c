/***********************************************************************************
  Filename:     piano.c

  Description:  Application/Driver layer for playing Piano/Guitar using TouchScrren.

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
#include "piano.h"
#include "key_map.h"
#include "App_RF.h"
#include "TouchScreen.h"


/***********************************************************************************
* @fn      playVPiano
*
* @brief   Main function for Piano. Initializes Piano, monitors for any key hit through TouchScreen and plays corresponding note to VS1053 MIDI Decoder.
*
* @param   none
*
* @return  none
*/
void playVPiano() {
	
	unsigned char usrExit[] = {0};
	char keyNote = 0;
	unsigned int xCoord = 0;
	unsigned int yCoord = 0;
	unsigned char sendBuf[3] = {0};
	
	while(1) {
		if (RF_gets_nblk(usrExit) != 0) {
			if (strncmp (usrExit, "STOP", 4) == 0) {
				break;
			}
		}
		else {
			getTsXY(&xCoord, &yCoord);
			keyNote = piano_key_match(xCoord, yCoord);
			halMcuWaitMs(20);
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

void playRPiano() {
	
}


/***********************************************************************************
* @fn      playVGuitar
*
* @brief   Main function for Guitar. Initializes Guitar, monitors for anystring hit through TouchScreen and plays corresponding note to VS1053 MIDI Decoder.
*
* @param   none
*
* @return  none
*/
void playVGuitar() {
	
	unsigned char usrExit[] = {0};
	char keyNote = 0;
	unsigned int xCoord = 0;
	unsigned int yCoord = 0;
	unsigned char sendBuf[3] = {0};
	
	while(1) {
		if (RF_gets_nblk(usrExit) != 0) {
			if (strncmp (usrExit, "STOP", 4) == 0) {
				break;
			}
		}
		else {
			getTsXY(&xCoord, &yCoord);
			keyNote = guitar_key_match(xCoord, yCoord);
			halMcuWaitMs(20);
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