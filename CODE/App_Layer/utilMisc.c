/***********************************************************************************
  Filename:     utilMisc.c

  Description:  Supporting functions related to UART, String Conversion.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/

#include <string.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "uart_intfc.h"

#include "utilMisc.h"

void printf_lcd(char *buf) {
  tx0_send(buf, strlen(buf));
}

void send_midi(char *buf) {
  tx0_send(buf, strlen(buf));
}

void printf_pc(char *buf) {
  tx1_send(buf, strlen(buf));
}

void printf_pc_code(char __code *buf) {
  tx1_send((const char*)buf, strlen((const char*)buf));
}



void putchar_pc(char buf) {
  tx1_send((void*)&buf, 1);
}

char getchar_pc(void) {
	
	char ch = 0;
	
  while (!rx_peek());
	rx_receive((void*)&ch, 1);
	return ch;
}

int getstr (char *s) {
	
	int i = 0;
	
	do {
					s[i] = getchar_pc();
					putchar_pc(s[i]);
					i++;
	} while(s[i-1] != 0x0D);              //0x0D is <CR>
	s[i-1] = 0x00;                        //replace <CR> with NULL (0x00)
	putchar_pc('\n');
	putchar_pc('\r');
	return (i-1);
}

char getchar_pc_nb(void) {
	
	char ch = 0;
	
  if (rx_peek())
	{
		rx_receive((void*)&ch, 1);
		return ch;
	}
	else
		return 0;
}