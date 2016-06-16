/******************************************************************************
    Filename: graphics_lcd.h

    This file defines Serial Graphics LCD related functions for the User Interface 
	Level Menu Design.
	
	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef GRAPHICS_LCD_H_
#define GRAPHICS_LCD_H_

#include "hal_defs.h"
#include "hal_types.h"

void pixel(char S_R, char x, char y);
void line(char S_R, char x1, char y1, char x2, char y2);
void circle(char S_R, int x, int y, int r);
void set_text_position(char x, char y);
void small_delay(int x);

void clear_screen_lcd(void);

void draw_piano(void);
void draw_drums(void);
void draw_guitar(void);


#endif /* GRAPHICS_LCD_H_ */
