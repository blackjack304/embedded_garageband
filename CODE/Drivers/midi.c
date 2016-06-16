/******************************************************************************
    Filename: midi.c

    This file defines Low Level I/O routines for MIDI Decoder Interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#include <ioCC2430.h>
#include "uart_intfc.h"
#include "hal_led.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"
#include "midi.h"


/******************************************************************************
* @fn     midiReset
*
* @brief  Reset the Internal State of VS1053 MIDI Decoder.
*
* @param  void
*
* @return void
*
******************************************************************************/
void midiReset() {

  	//MIDI_RESET = 1;
	MCU_IO_OUTPUT(1,1,1);
	//delay_millisec(100);
	halMcuWaitMs(100);

  	MCU_IO_OUTPUT(1,1,0);
	//MIDI_RESET = 0;
	//delay_millisec(100);
	halMcuWaitMs(100);
	
	//MIDI_RESET = 1;
	MCU_IO_OUTPUT(1,1,1);
	//delay_millisec(100);
	halMcuWaitMs(200);
}



/******************************************************************************
* @fn     noteOn
*
* @brief  Notifies VS1053 MIDI Decoder of start of Note.
*
* @param  unsigned char channel
*			Channel Number to start the note.
*		  unsigned char note
*			Specific Note Number
*		  unsigned char attack_velocity
*			Velocity of the note to be played
*
* @return void
*
******************************************************************************/
void noteOn(unsigned char channel, unsigned char note, unsigned char attack_velocity) {
	talkMIDI( (0x90 | channel), note, attack_velocity);
}


/******************************************************************************
* @fn     noteOff
*
* @brief  Notifies VS1053 MIDI Decoder of start of Note. 
*		  Send a MIDI note-off message.  Like releasing a piano key
*
* @param  unsigned char channel
*			Channel Number to stop the note.
*		  unsigned char note
*			Specific Note Number
*		  unsigned char release_velocity
*			Velocity of the note to be released
*
* @return void
*
******************************************************************************/
void noteOff(unsigned char channel, unsigned char note, unsigned char release_velocity) {
	talkMIDI( (0x80 | channel), note, release_velocity);
}


/******************************************************************************
* @fn     talkMIDI
*
* @brief  Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
*
* @param  unsigned char cmd
*			Command to be send to MIDI
*		  unsigned char data1
*			Command Specific Lower Byte Data
*		  unsigned char data2
*			Command Specific Higher Byte Data
*
* @return void
*
******************************************************************************/
void talkMIDI(unsigned char cmd, unsigned char data1, unsigned char data2) {

	//bit_bang_tx(cmd);
	//putchar (cmd);
	tx0_send(&cmd, 1);
	tx1_send(&cmd, 1);

	//bit_bang_tx(data1);
	//putchar (data1);
	tx0_send(&data1, 1);
	tx1_send(&data1, 1);
	
	//Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes
	//(sort of: http://253.ccarh.org/handout/midiprotocol/)
	//if( (cmd & 0xF0) <= 0xB0) {
	if(cmd <= 0xB0) {
		//bit_bang_tx(data2);
		//putchar (data2);
		tx0_send(&data2, 1);
		tx1_send(&data2, 1);
	}
	tx1_send("\n\r", 2);
}


/******************************************************************************
* @fn     midiVolSet
*
* @brief  Set VS1053 MIDI Decoder Volume.
*
* @param  unsigned char vol
*			Volume to be set.
*
* @return void
*
******************************************************************************/
void midiVolSet(unsigned char vol) {
	talkMIDI(0xB0, 0x07, vol);
}


/******************************************************************************
* @fn     midiBankSet
*
* @brief  Set VS1053 MIDI Instrument bank.
*
* @param  unsigned char bank
*			Bank No. to be set.
*
* @return void
*
******************************************************************************/
void midiBankSet(unsigned char bank) {
	talkMIDI(0xB0, 0, bank);
}


/******************************************************************************
* @fn     midiPatchSet
*
* @brief  Set VS1053 MIDI Instrument Patch.
*
* @param  unsigned char instr
*			Instrument No. to be set.
*
* @return void
*
******************************************************************************/
void midiPatchSet(unsigned char instr) {
	talkMIDI(0xC0, instr, 0);
}


/******************************************************************************
* @fn     midiVolSet
*
* @brief  Set VS1053 MIDI Decoder to MUTE.
*
* @param  void
*
* @return void
*
******************************************************************************/
void midiSilence() {
	talkMIDI(0xB0, 0x78, 0);
}

/******************************************************************************
* @fn     midiChannelSet
*
* @brief  Set VS1053 MIDI Channel.
*
* @param  unsigned char chan
*			Channel No. to be set.
*
* @return void
*
******************************************************************************/
void midiChannelSet(unsigned char chan) {
	midiChannel = chan;
}


