/***********************************************************************************
  Filename:     utilMisc.h

  Description:  Supporting functions related to UART, String Conversion.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/
#ifndef UTIL_MISC_H
#define UTIL_MISC_H

#include "hal_defs.h"
#include "hal_types.h"

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define ISBETWEEN(B, A, C) ( ((A-B) >= 0) && ((A-C) <= 0) )

void printf_lcd(char *buf);
void send_midi(char *buf);
void printf_pc(char *buf);
void printf_pc_code(char __code *buf);
void putchar_pc(char buf);
char getchar_pc(void);
int getstr (char *s);
char getchar_pc_nb(void);
#endif