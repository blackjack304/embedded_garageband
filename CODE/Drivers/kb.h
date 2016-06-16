/******************************************************************************
    Filename: kb.h

    This file defines Low Level I/O routines for PS/2 Keyboard.
	
    Code leveraged from www.8052.com
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#ifndef __KB_INCLUDED
#define __KB_INCLUDED

#include "hal_types.h"
#include "hal_defs.h"

#define ISC00 0
#define ISC01 1

void init_kb(void);
void decode(unsigned char sc);
void put_kbbuff(unsigned char c);
char kb_getchar();
#endif

