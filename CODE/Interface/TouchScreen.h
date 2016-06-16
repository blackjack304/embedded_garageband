/******************************************************************************
    Filename: TouchScreen.h

    This file defines basic driver for 4-wire Resistive TouchScrren Interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include "hal_defs.h"
#include "hal_types.h"

// Pin Definitions
// Port 1
#define XN         0x02
#define XP         0x04
#define YN         0x08
#define YP         0x10

#define XN_C         2
#define XP_C         3
#define YN_C         4
#define YP_C         5


// ADC Channels
#define CHAN_XN    ADC_AIN2
#define CHAN_XP    ADC_AIN3
#define CHAN_YN    ADC_AIN4
#define CHAN_YP    ADC_AIN5

// ADC Averaging Values
#define NUMSAMPLES  6
#define SAMPLESHIFT 2

// Global Variables
char touched;                               // Flag for touch status
unsigned int samples[NUMSAMPLES];           // ADC sample results
unsigned int x,y;                     // X,Y touch position variables


// System Routines
void getTsXY(unsigned int *xCoord, unsigned int *yCoord);
void TouchScreen(void);
void Initialize_TouchScreen(void);          // Configure modules & control Registers
void waitForTouch(void);                    // Waits for a touch on the screen
unsigned int getSamples(unsigned int);      // Get samples from ADC

void readXY();           // Read X,Y coordinates
void setXDrive(void);                       // Drive X wires
void setYDrive(void);                       // Drive Y wires
void clearDrives(void);                     // Stop driving X and Y
void setTouchDrives(void);                  // Drive touch test wires
void sendData(unsigned int xCoord, unsigned int yCoord); // Send touch coordinates to host
void sendByte(char);                        // Transmits byte using Timer_A

#endif
