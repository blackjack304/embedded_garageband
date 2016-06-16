/***********************************************************************************
  Filename:     Drum.h

  Description:  Application/Driver layer for playing actual Drums using Peizo Sensors connected to Port Pins.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/
#ifndef DRUM_H
#define DRUM_H

#include "hal_defs.h"
#include "hal_types.h"

// Pin Definitions
// Port 1

#define DR0         0x01
#define DR1         0x02
#define DR2         0x04
#define DR3         0x08
#define DR4         0x10
#define DR5         0x20

#define DRUM_PORT   0



#define DR0_PIN         0
#define DR1_PIN         1
#define DR2_PIN         2
#define DR3_PIN         3
#define DR4_PIN         4
#define DR5_PIN         5

void Intitialize_Drumset();

void DrumSet();
void playVDrum();


#endif