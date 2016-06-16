/***********************************************************************************
  Filename:     key_map.h

  Description:  Specific Mappings of X-Y coordinates related to Touchscreen based Drums, Piano and Guitar.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/
#include <hal_led.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "key_map.h"
#include "uart_intfc.h"
#include "utilMisc.h"


/***********************************************************************************
* @fn      piano_key_match
*
* @brief   It tries to match the input X-Y Co-ordinates with specific note/piano key
*
* @param   int x
*			 TouchScreen X-Coordinate
*		   int y
*			 TouchScreen Y-Coordinate
*
* @return  Piano Note
*/
char piano_key_match(int x, int y)
{
	volatile unsigned char BLACK_KEY = TRUE;
	volatile unsigned char WHITE_KEY = TRUE;
	char key_note=0;
	char center_note = 0x3C;
	
	if(ISBETWEEN(xmin,x,xmax) && ISBETWEEN(ymin,y,ymax))	//check if it is in logical range of lcd
	{
		if(ISBETWEEN(black_key_ymin ,y ,black_key_ymax))	//check if it is a black key only
		{			
			if(ISBETWEEN(keyA_xmin ,x ,keyA_xmax))			//check is key A is pressed
			{
				key_note=center_note - 16;
			}
			else if(ISBETWEEN(keyB_xmin ,x ,keyB_xmax))		//check is key B is pressed
			{
				key_note=center_note - 10;
			}
			else if(ISBETWEEN(keyC_xmin ,x ,keyC_xmax))		//check is key C is pressed
			{
				key_note=center_note - 4;
			}
			else if(ISBETWEEN(keyD_xmin ,x ,keyD_xmax))		//check is key D is pressed
			{
				key_note=center_note + 2;
			}
			else if(ISBETWEEN(keyE_xmin ,x ,keyE_xmax))		//check is key E is pressed
			{
				key_note=center_note + 8;
			}
			else if(ISBETWEEN(keyF_xmin ,x ,keyF_xmax))		//check is key F is pressed
			{
				key_note=center_note + 14;
			}
			else
			{
				BLACK_KEY = FALSE;
				key_note=0;
				//printf("The key pressed is not a black key\n\r");
			}
		}
		else
		{
                      BLACK_KEY = FALSE;
                      key_note=0;
			//printf("It is definately not a black Key. Check for white key\n\r");
		}
		
		if(BLACK_KEY==FALSE)	//see if black key is already found. WHy take pains to check for white key also.
		{
			if(ISBETWEEN(key1_xmin ,x ,key1_xmax))	//check if white key1 is pressed
			{
				key_note=center_note - 18;
			}
			else if(ISBETWEEN(key2_xmin ,x ,key2_xmax))	//check if white key2 is pressed
			{
				key_note=center_note - 14;
			}
			else if(ISBETWEEN(key3_xmin ,x ,key3_xmax))	//check if white key3 is pressed
			{
				key_note=center_note - 12;
			}
			else if(ISBETWEEN(key4_xmin ,x ,key4_xmax))	//check if white key4 is pressed
			{
				key_note=center_note - 8;
			}
			else if(ISBETWEEN(key5_xmin ,x ,key5_xmax))	//check if white key5 is pressed
			{
				key_note=center_note - 6;
			}
			else if(ISBETWEEN(key6_xmin ,x ,key6_xmax))	//check if white key6 is pressed
			{
				key_note=center_note - 2;
			}
			else if(ISBETWEEN(key7_xmin ,x ,key7_xmax))	//check if white key7 is pressed
			{
				key_note=center_note;
			}
			else if(ISBETWEEN(key8_xmin ,x ,key8_xmax))	//check if white key8 is pressed
			{
				key_note=center_note + 4;
			}
			else if(ISBETWEEN(key9_xmin ,x ,key9_xmax))	//check if white key9 is pressed
			{
				key_note=center_note + 6;
			}
			else if(ISBETWEEN(key10_xmin ,x ,key10_xmax))	//check if white key10 is pressed
			{
				key_note=center_note + 10;
			}
			else if(ISBETWEEN(key11_xmin ,x ,key11_xmax))	//check if white key11 is pressed
			{
				key_note=center_note + 12;
			}
			else if(ISBETWEEN(key12_xmin ,x ,key12_xmax))	//check if white key12 is pressed
			{
				key_note=center_note + 16;
			}
			else if(ISBETWEEN(key13_xmin ,x ,key13_xmax))	//check if white key13 is pressed
			{
				key_note=center_note + 18;
			}
			else 	//touched within bounds. NO white key. No black key. SO what the hell is detected???
			{
				WHITE_KEY=FALSE;
				key_note=0;
			}
		}
		else	//white key check ELSE
		{
			WHITE_KEY=FALSE;
			//printf("Not checking for white key. Black key already detected in the previous check\n\r");
		}		//END of white key check
	}	
	else	//bound check else
	{
		BLACK_KEY=FALSE;
		WHITE_KEY=FALSE;
		//printf("Data is out of Bounds. X = %d   Y = %d\n\r\n\r",x,y);
		key_note=0;
	}//end of bound check

        //tx1_send(&key_note, 1);
	return key_note;
}



/***********************************************************************************
* @fn      ps2_key_match
*
* @brief   It tries to match the input PS/2 Keyboard ASCII key with specific note/piano key
*
* @param   char scancode
*			 Input ASCII Key by PS/2 KB
*
* @return  Piano Note
*/
char ps2_key_match(char scancode)
{
	char i=0;
	char keynote=0;
	char base_keynote = 0x3C-18;
	
	for(i=0;i<19;i++)
	{
		if(scancode == valid_scancode[i])
		{
			keynote = base_keynote + (i*2);
			break;
		}
	}
	return keynote;
}



//DRUMS STUFF------------------------------

/***********************************************************************************
* @fn      drum_key_match
*
* @brief   It tries to match the input X-Y Co-ordinates with specific note/drum key
*
* @param   int x
*			 TouchScreen X-Coordinate
*		   int y
*			 TouchScreen Y-Coordinate
*
* @return  Drum Note
*/
char drum_key_match(int x, int y)
{
	char key_note = 0;
	
	if(ISBETWEEN(xmin,x,xmax) && ISBETWEEN(ymin,y,ymax))	//check if it is in logical range of lcd
	{
		if(ISBETWEEN(square1_xmin, x, square1_xmax) && ISBETWEEN(square1_ymin, y, square1_ymax))
				key_note = 40;		//Drum 1 SNARE DRUM
		else if(ISBETWEEN(square2_xmin, x, square2_xmax) && ISBETWEEN(square2_ymin, y, square2_ymax))
				key_note = 36;		//Drum 2 Bass drum	
		else if(ISBETWEEN(square3_xmin, x, square3_xmax) && ISBETWEEN(square3_ymin, y, square3_ymax))
				key_note = 48;		//drum 3 Tom Tom drum
		else if(ISBETWEEN(square4_xmin, x, square4_xmax) && ISBETWEEN(square4_ymin, y, square4_ymax))
				key_note = 41;		//Drum 4 Low Floor tom
		else if(ISBETWEEN(square5_xmin, x, square5_xmax) && ISBETWEEN(square5_ymin, y, square5_ymax))
				key_note = 51;		//Cymbal ride cymbal 1
		else
			key_note = 0;

	}
		else	//bound check else
	{
		key_note=0;
	}//end of bound check

    //tx1_send(&key_note, 1);
	return key_note;
}

/***********************************************************************************
* @fn      guitar_key_match
*
* @brief   It tries to match the input X-Y Co-ordinates with specific note/Guitar string
*
* @param   int x
*			 TouchScreen X-Coordinate
*		   int y
*			 TouchScreen Y-Coordinate
*
* @return  Guitar Note
*/
char guitar_key_match(int x, int y)
{
	char key_note=0;
	
	if(ISBETWEEN(xmin,x,xmax) && ISBETWEEN(ymin,y,ymax))	//check if it is in logical range of lcd
	{
		if(ISBETWEEN(string1_ymin, y, string1_ymax))	//String 1 Thickest string
			key_note = 52;
		else if(ISBETWEEN(string2_ymin, y, string2_ymax))	//String 2
			key_note = 57;
		else if(ISBETWEEN(string3_ymin, y, string3_ymax))	//String 3
			key_note = 62;
		else if(ISBETWEEN(string4_ymin, y, string4_ymax))	//String 4
			key_note = 67;
		else if(ISBETWEEN(string5_ymin, y, string5_ymax))	//String 5
			key_note = 71;
		else if(ISBETWEEN(string6_ymin, y, string6_ymax))	//String 6
			key_note = 76;
		else
			key_note=0;
	
	}	
	else	//bound check else
	{
		key_note=0;
	}//end of bound check

	return key_note;
}