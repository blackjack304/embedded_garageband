/***********************************************************************************
  Filename:     Main_mod2.c

  Description:  Main.c file for Hardware Module 2.

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
#include "utilMisc.h"
#include "user_interface.h"

#define SZ_RFBUF 50

enum rfStat {
	rfHello = 1,
	rfVPiano,
	rfVDrum,
	rfVGuitar,
	rfRPiano
};

unsigned char rfBuf[SZ_RFBUF] = {0};
unsigned char Check_Flag = 1;

extern char drum_piano_guitar;
extern char real_virtual;
extern char drum_piano_guitar_rv;
extern char piano_type;
extern char guitar_type;
extern char audio_termination;
extern char record_notes;
extern char play_recorded_notes;
extern char track;
extern char playback;

unsigned char endPacket[3] = {0xFF, 0xFF, 0xFF};
unsigned char Record_0[] = "Record_0";
unsigned char Record_1[] = "Record_1";
unsigned char Record_2[] = "Record_2";
unsigned char Record_3[] = "Record_3";
unsigned char Play_0[] = "Play_0";
unsigned char Play_1[] = "Play_1";
unsigned char Play_2[] = "Play_2";
unsigned char Play_3[] = "Play_3";


void midiTest(void);
void playRfNotes();
void playRfNotesSD();



/***********************************************************************************
* @fn      Main
*
* @brief   Module 2 Main Function.
*
* @param   none
*
* @return  none
*/
void main(void)
{	
	unsigned char cmd = 0;
	char keyNote = 0;
	//char printBuf[50] = {0};
	
	// Initalise board peripherals
  halBoardInit();
  uart_intfc_init();
  TimerInterface_Initialize();
	Init_AppRF();
	halMcuWaitMs(1000);

  printf_pc("Init done (M2)\n\r");	
	halMcuWaitMs(1000);
	
	RF_puts("HELLO");
	RF_gets_blk(rfBuf);
	//printf_pc(rfBuf);
	
	while(1) {
		bzero(rfBuf, SZ_RFBUF);
		start_my_menu();
		
		if (record_notes == TRUE) {
			switch (track) {
				case 0: RF_puts_SD(Record_0, 8); break;
				case 1: RF_puts_SD(Record_1, 8); break;
				case 2: RF_puts_SD(Record_2, 8); break;
				case 3: RF_puts_SD(Record_3, 8); break;
				default: break;
			}
		}
		
		if (play_recorded_notes == TRUE) {
			printf_pc("\n\rWill play recorded notes\n\r");	
			midiReset();
			switch (playback) {
				case 0: RF_puts_SD(Play_0, 6); break;
				case 1: RF_puts_SD(Play_1, 6); break;
				case 2: RF_puts_SD(Play_2, 6); break;
				case 3: RF_puts_SD(Play_3, 6); break;
				default: break;
			}
			playRfNotesSD();
		}
		else {
			switch (drum_piano_guitar_rv) {
				case DRUM_REAL_INSTRUMENT:
					midiInit();
					DrumSet();
					RF_puts_SD(endPacket, 3);
					break;
					
				case PIANO_REAL_INSTRUMENT:
					//printf_pc("PIANO_REAL_INSTRUMENT\n\r");
					midiInit();
					while(1) {
						cmd = getchar_pc();
						if (cmd == 27) {
							RF_puts_SD(endPacket, 3);
							break;
						}
						else {
							keyNote = ps2_key_match(cmd);
							tx1_send(&keyNote, 1);
							if (keyNote != 0) {
								noteOn(0, keyNote, 0x7F);
							}
						}
					}
					break;
					
				case DRUM_VIRTUAL_INSTRUMENT:
					//printf_pc("DRUM_VIRTUAL_INSTRUMENT\n\r");
					midiInit();
					RF_puts("VDRUM");
					RF_gets_blk(rfBuf);
					printf_pc(rfBuf);
					playRfNotes();
					RF_puts_SD(endPacket, 3);
					break;
					
				case PIANO_VIRTUAL_INSTRUMENT:
					//printf_pc("PIANO_VIRTUAL_INSTRUMENT\n\r");
					midiInit();
					RF_puts("VPIANO");
					RF_gets_blk(rfBuf);
					printf_pc(rfBuf);
					playRfNotes();
					RF_puts_SD(endPacket, 3);
					break;
					
				case GUITAR_VIRTUAL_INSTRUMENT:
					//printf_pc("GUITAR_VIRTUAL_INSTRUMENT\n\r");
					midiInit();
					RF_puts("VGUITAR");
					RF_gets_blk(rfBuf);
					printf_pc(rfBuf);
					playRfNotes();
					RF_puts_SD(endPacket, 3);
					break;
					
				default:
					printf_pc("default\n\r");
					break;
			}
		}
	}
	
//
//	//DrumSet();
//	
//	while (1) {
//		while (!RF_Peek());
//		rfLen = RF_Receive(&keyNote);	
//		noteOn(0, keyNote, 0x3D);
//		tx1_send(&keyNote, 1);
//	}
}

void playRfNotes() {
	
	char cmd = 0;
	
	while (1) {
		cmd = getchar_pc_nb();
		if (cmd == 27) {
			RF_puts("STOP");
			break;
		}
		else {
			if (RF_gets_nblk(rfBuf) != 0) {
				noteOn(rfBuf[0], rfBuf[1], rfBuf[2]);
			}
		}
	}
}


/***********************************************************************************
* @fn      Main
*
* @brief   Send Notes to VS1053 which are received by RF Packets.
*
* @param   none
*
* @return  none
*/
void playRfNotesSD() {
	
	char cmd = 0;
	
	while (1) {
		cmd = getchar_pc_nb();
		if (cmd == 27) {
			break;
		}
		else {
			if (RF_gets_nblk(rfBuf) != 0) {
				tx1_send(rfBuf, 3);
				talkMIDI(rfBuf[0], rfBuf[1], rfBuf[2]);
				if (rfBuf[0] == 0xFF && rfBuf[1] == 0xFF && rfBuf[2] == 0xFF) {
					break;
				}
			}
		}
	}//while (1)
}


/***********************************************************************************
* @fn      Main
*
* @brief   VS1053 general Test Program.
*
* @param   none
*
* @return  none
*/
void midiTest() {
	
	midiReset();
	midiVolSet(120);
	//midiBankSet(DRUMS1);
	//midiPatchSet(5);
	midiBankSet(MELODY);
	midiPatchSet(PIANO_ELG);
	halMcuWaitMs(100);

//  midiReset();
//	midiVolSet(120); //set channel volume to near max (127)
//	midiBankSet(MELODY); //0x79 = melodious instruments
//	midiPatchSet(118);
//	noteOn(0, 51, 0x3D);
//	halMcuWaitMs(500);
	
	
//	for (i=0; i<50; i++) {
//		noteOn(0, i+30, 0x48);
//		halMcuWaitMs(100);
//		//noteOff(0, i+30, 0x48);
//		halMcuWaitMs(100);
//
//	}
//	for (i=50; i>=0; i--) {
//		noteOn(0, i+30, 0x48);
//		halMcuWaitMs(100);
//		//noteOff(0, i+30, 0x48);
//		halMcuWaitMs(100);
//	}
	
	//tx1_send("Done\n\r", 6);
	while (1);
}
