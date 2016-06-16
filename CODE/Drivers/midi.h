/******************************************************************************
    Filename: midi.c

    This file defines Low Level I/O routines for MIDI Decoder Interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef MIDI_H_
#define MIDI_H_

#include "hal_defs.h"
#include "hal_types.h"

//banks
#define DEF_BANK 0x00
#define DRUMS1 0x78
#define DRUMS2 0x7F
#define MELODY 0x79

//instruments for bank MELODY
#define PIANO_AC 1
#define PIANO_ELG 3
#define PIANO_EL1 5
#define PIANO_EL2 6
#define HARP 7
#define MUSICBOX 11
#define XYLO 14
#define ORGAN_CH 20
#define ACCORDIAN 22
#define GUITAR_AC 26
#define GUITAR_EL 28
#define GUITAR_EL_BASS 34
#define VIOLIN 41
#define CELLO 43
#define TRUMPET 57
#define FLUTE 74
#define VOICE_LEAD 86
#define RAIN 97
#define SCIFI 104
#define SITAR 105
#define BAGPIPE 110
#define BIRD 124
#define TELEPHONE 125
#define GUNSHOT 128

#define MIDI_RESET P1_1

static unsigned char midiChannel = 0;

void midiReset(void);

void midiVolSet(unsigned char vol);
void midiBankSet(unsigned char bank);
void midiPatchSet(unsigned char instr);
void midiSilence(void);
void midiChannelSet(unsigned char chan);


void noteOn(unsigned char channel, unsigned char note, unsigned char attack_velocity);
void noteOff(unsigned char channel, unsigned char note, unsigned char release_velocity);
void talkMIDI(unsigned char cmd, unsigned char data1, unsigned char data2);

#endif /* MIDI_H_ */
