/******************************************************************************
    Filename: TouchScreen.c

    This file defines basic driver for 4-wire Resistive TouchScrren Interface.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#include <ioCC2430.h>
#include "TouchScreen.h"
#include "adc.h"
#include "uart_intfc.h"
#include "hal_led.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"
#include "key_map.h"
#include "App_RF.h"

#include <stdio.h>
#include <string.h>



char str_buf[20];

/***********************************************************************************
* @fn      TouchScreen
*
* @brief   Main Function, called by App_Layer to get the co-ordinates (touched or not) of TouchScreen.
*
* @param   none
*
* @return  none
*/
void TouchScreen(void)
{
	Initialize_TouchScreen();               // Initialize device
	waitForTouch();                         // Wait for touch on screen
	touched = 1;                            // Screen is touched
	while(1)
	{                                       // Loop while screen is touched
		readXY();                             // Get X,Y coordinates index 0
	
		// Take 2 values and see the difference between them.
		// If value is greater than 5 steps, discard packet.
		// This code is designed to take a majority vote of 2 packets.
		if(x > 1100 && x < 4600 && y > 700 && y < 5300)
				sendData(x,y);                // Send data to host
	}
}


/***********************************************************************************
* @fn      Initialize_TouchScreen
*
* @brief  Initializing Device Interface
*
* @param   none
*
* @return  none
*/
void Initialize_TouchScreen(void)
{
	clearDrives();                        // Clear outputs
	PICTL |= 1;                             // Y+drive high to low transition int
}


/***********************************************************************************
* @fn      waitForTouch
*
* @brief  Wait for Touch on Screen
*
* @param   none
*
* @return  none
*/
void waitForTouch(void)
{
	setTouchDrives();
	if((P0 & YP)) {                       // Y+ high?
	P0IFG = 0;                             // Clear interrupt flags
	PICTL |= 1;
	while(P0IFG == 0);  //__bis_SR_register(LPM4_bits + GIE);     // Wait for interrupt
	}
	halLedToggle(1);
	clearDrives();                            // Clear drive wires
}


/***********************************************************************************
* @fn      getSamples
*
* @brief  Get ADC Samples of Specified Channel and Average
*
* @param   none
*
* @return  none
*/
unsigned int getSamples(unsigned int chan)
{
	unsigned char loop;
	unsigned int avg = 0;                     // Clear average variable
	for (loop=0;loop<NUMSAMPLES;loop++) {     // Loop
	avg = avg + adcSampleSingle(ADC_REF_AVDD,ADC_12_BIT,chan);              // Accumulate samples
	}
	avg = avg >> SAMPLESHIFT;                 // Calculate average
	return avg;                               // Return average
}


/***********************************************************************************
* @fn      readXY
*
* @brief  Read X and Y Coordinates of Touch
*
* @param   none
*
* @return  none
*/
void readXY()
{
	// Find X Coordinate
	setXDrive();                              // Set X wires to drives
	x = getSamples(CHAN_YP);          // Sample Y+ to get X coordinate
	
	// Find Y Coordinate
	setYDrive();                              // Set Y wires to drives
	y = getSamples(CHAN_XP);          // Sample X+ to get Y coordinate
	
	// Setup pins to check if screen is still touched
	setTouchDrives();                         // See if screen still touched
	
	clearDrives();                            // Clear drive wires
}


/***********************************************************************************
* @fn      setYDrive
*
* @brief  Drive X+drive High and X-drive Low, Configure ADC Pins
*
* @param   none
*
* @return  none
*/
void setXDrive(void)
{
	clearDrives();
	MCU_IO_OUTPUT(0, XP_C, 1);
	MCU_IO_OUTPUT(0, XN_C, 0);
	halMcuWaitMs(1);                                 // Delay to allow settling
}


/***********************************************************************************
* @fn      setYDrive
*
* @brief   Drive Y+drive High and Y-drive Low, Configure ADC Pins
*
* @param   none
*
* @return  none
*/
void setYDrive(void)
{
	clearDrives();
	MCU_IO_OUTPUT(0,YP_C,1);
	MCU_IO_OUTPUT(0,YN_C,0);
	halMcuWaitMs(1);                                 // Delay to allow settling
}


/***********************************************************************************
* @fn      clearDrives
*
* @brief   Stop Driving X and Y, Configure ADC Pins
*
* @param   none
*
* @return  none
*/
void clearDrives(void)
{
    MCU_IO_INPUT(0,XN_C,MCU_IO_TRISTATE);
    MCU_IO_INPUT(0,YN_C,MCU_IO_TRISTATE);
    MCU_IO_INPUT(0,XP_C,MCU_IO_TRISTATE);
    MCU_IO_INPUT(0,YP_C,MCU_IO_TRISTATE);
}



/***********************************************************************************
* @fn      setTouchDrives
*
* @brief   Set Y+drive to input with pull-up, X-drive to output low
*
* @param   none
*
* @return  none
*/
void setTouchDrives(void)
{
	clearDrives();
	MCU_IO_OUTPUT(0,YP_C,1);
	MCU_IO_INPUT(0,YP_C,MCU_IO_PULLUP);
	MCU_IO_OUTPUT(0,XN_C,0);
	halMcuWaitMs(1);                                 // Wait for pin to settle
}



/***********************************************************************************
* @fn      getTsXY
*
* @brief   Send X and Y Coordinates to Host
*
* @param   unsigned int xCoord
*			 X-Coordinate
*		   unsigned int yCoord
*			 Y-Coordinate
*
* @return  none
*/
void sendData(unsigned int xCoord, unsigned int yCoord) {
  //static   char byte[5];

//    xCoord -= 1100;
//    yCoord -= 700;
  	unsigned char keyNote = 0;

    if((signed int)xCoord < 0)  xCoord = 0;
    if((signed int)yCoord < 0)  yCoord = 0;


    sprintf(str_buf, "X: %d, Y:%d\n\r",xCoord, yCoord);
    tx1_send(str_buf,strlen(str_buf));
	
	keyNote = piano_key_match(xCoord, yCoord);
	//keyNote = drum_key_match(xCoord, yCoord);
	tx1_send(&keyNote, 1);
	RF_Send(&keyNote, 1);

}

/***********************************************************************************
* @fn      getTsXY
*
* @brief   Main Function, called by App_Layer to get the co-ordinates (touched or not) of TouchScreen.
*
* @param   unsigned int *xCoord
*			Ptr to get X-Coordinate
*		   unsigned int *yCoord
*			Ptr to get Y-Coordinate
*
* @return  none
*/
void getTsXY(unsigned int *xCoord, unsigned int *yCoord) {
	
	Initialize_TouchScreen();               // Initialize device
	readXY();                             	// Get X,Y coordinates
	*xCoord = x;
	*yCoord = y;
}