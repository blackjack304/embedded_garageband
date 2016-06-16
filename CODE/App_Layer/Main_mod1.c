/***********************************************************************************
  Filename:     Main_mod1.c

  Description:  Main.c file for Hardware Module 1.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/
/***********************************************************************************
* INCLUDES
*/
#include <hal_led.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "uart_intfc.h"
#include "TouchScreen.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_spi.h"
#include "TimerManager.h"
#include "Drum.h"
#include "midi.h"
#include "graphics_lcd.h"
#include "key_map.h"
#include "App_RF.h"
#include "kb.h"
#include "utilMisc.h"
#include "piano.h"


#define SZ_RFBUF 50

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

enum rfStat {
	rfHello = 1,
	rfVPiano,
	rfVDrum,
	rfVGuitar
};

unsigned char rfBuf[SZ_RFBUF] = {0};
unsigned char Check_Flag = 1;



void midiTest(void);
void lcdTest(void);


/***********************************************************************************
* @fn      Main
*
* @brief   Module 1 Main Function.
*
* @param   none
*
* @return  none
*/
void main(void)
{
	//rf flags-----
	unsigned char rfStatus = 0;
	
	// Initalise board peripherals
  halBoardInit();
  uart_intfc_init();
  TimerInterface_Initialize();
	Init_AppRF();
	halMcuWaitMs(2000);

  printf_pc("Init done (M1)\n\r");

  while (1) {
		
		bzero(rfBuf, SZ_RFBUF);
		RF_gets_blk(rfBuf);
		
		if (NULL != strstr((const char*)rfBuf, "HELLO")) rfStatus = rfHello;
		if (NULL != strstr((const char*)rfBuf, "VPIANO")) rfStatus = rfVPiano;
		if (NULL != strstr((const char*)rfBuf, "VDRUM")) rfStatus = rfVDrum;
		if (NULL != strstr((const char*)rfBuf, "VGUITAR")) rfStatus = rfVGuitar;
		
		switch (rfStatus) {
			
			case rfHello:
				printf_pc("In rfHello\n\r");
				halMcuWaitMs(100);
				RF_puts("ACK");
				break;
				
			case rfVPiano:
				printf_pc("In rfVPiano\n\r");
				clear_screen_lcd();						//Clears LCD screen
				draw_piano();							//Draws a Piano
				RF_puts("ACK");
				playVPiano();
				break;
				
			case rfVDrum:
				printf_pc("In rfVDrum\n\r");
				clear_screen_lcd();						//Clear LCD screen
				draw_drums();									//Draws Drums
				RF_puts("ACK");
				playVDrum();
				break;
				
			case rfVGuitar:
				printf_pc("In rfVGuitar\n\r");
				clear_screen_lcd();						//Clear LCD screen
				draw_guitar();								//Draws Guitar Strings
				RF_puts("ACK");
				playVGuitar();
				break;
				
			default:
				printf_pc("In default\n\r");
				RF_puts("NACK");
				
		} //end switch
  }// end while(1)
}