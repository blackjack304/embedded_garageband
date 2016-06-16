/******************************************************************************
    Filename: kb.c

    This file defines Low Level I/O routines for PS/2 Keyboard.
	
    Code leveraged from www.8052.com
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#include "kb.h"
#include "hal_board.h"
#include "hal_cc8051.h"
#include "hal_mcu.h"
#include "hal_led.h"
#include "hal_types.h"
#include "hal_defs.h"
#include "hal_int.h"
#include "hal_assert.h"
#include "ioCC2430.h"

#include "scancodes.h"
#include "uart_intfc.h"

#define BUFF_SIZE 64

unsigned char edge, bitcount;                // 0 = neg.  1 = pos.

unsigned char kb_buffer[BUFF_SIZE];
unsigned char *inpt, *outpt;
unsigned char buffcnt;
unsigned int spd;
unsigned char is_up, shift, mode,spec ;
bool data_ready;
bool start_read;


/******************************************************************************
* @fn     init_kb
*
* @brief  Initialize PS/2 Keyboard Interface.
*
* @param  void
*
* @return void
*
******************************************************************************/
void init_kb()
{
  data_ready=FALSE;
  is_up=0;
  spec=0;
  shift=0;
  mode=0;

  inpt = kb_buffer;// Initialize buffer
  outpt = kb_buffer;
  buffcnt = 0;
  edge = 0;
  bitcount = 11;

  halLedSet(3);

  MCU_IO_OUTPUT(KPORT, KDATA_PIN, 0);
  MCU_IO_OUTPUT(1, 0, 0);

  MCU_IO_INPUT(1, 0, MCU_IO_TRISTATE);
  MCU_IO_INPUT(KPORT, KCLOCK_PIN, MCU_IO_TRISTATE);

  P1IF = 0;
  PICTL |= 0x02;  //IT0 = 1;  				// Ext INT0 falling edge triggered
  IEN2 |= 0x10;         //IEN2.P2IE = 1;
  P1IEN = 0x01;

  bitcount = 11;
}


/******************************************************************************
* @fn     port1_ISR
*
* @brief  ISR function for port 1 GPIO.
*
* @param  void
*
* @return void
*
******************************************************************************/
HAL_ISR_FUNCTION(port1_ISR,P1INT_VECTOR)
{
  static unsigned char kbdata;// Holds the received scan code
  P1IF = 0;
  halLedClear(3);

  if(P1IFG & 0x01)
  {
	
    if (!edge)                                // Routine entered at falling edge
    {
        if(bitcount < 11 && bitcount > 2)    // Bit 3 to 10 is data. Parity bit,
        {                                    // start and stop bits are ignored.
            kbdata = (kbdata >> 1);
            if(P2 & 0x02)
                kbdata = kbdata | 0x80;            // Store a '1'
        }

       PICTL &= ~0x02;                            // Set interrupt on rising edge
        edge = 1;

    } else {                                // Routine entered at rising edge

        PICTL |= 0x02;                            // Set interrupt on falling edge
        edge = 0;

        if(--bitcount == 0)                    // All bits received
        {
            decode(kbdata);
            bitcount = 11;
        }
    }
	
  }
  P1IFG = 0;
  P1IF = 0;
  halLedSet(3);
}


/******************************************************************************
* @fn     decode
*
* @brief  Decode incoming data from keyboard to proper ASCII key.
*
* @param  void
*
* @return void
*
******************************************************************************/
void decode(unsigned char sc)
{
  unsigned char i;
  if (!is_up)// Last data received was the up-key identifier
  {
    switch (sc)
    {
    case 0xF0 :// The up-key identifier
      is_up = 1;
      spec = 0;
      break;
    case 0x12 :// Left SHIFT
      shift = 1;
      break;
    case 0x59 :// Right SHIFT
      shift = 1;
      break;
    case 0x05 :// F1
      /*if(mode == 0)
      mode = 1;// Enter scan code mode
      if(mode == 2)
      mode = 3;// Leave scan code mode*/
      break;
    case 0xE0 :
      //print_line("in E0");
      spec = 1;
      /*for(i = 0; special[i][0]!=sc && special[i][0]; i++);
      if (special[i][0] == sc) {
      put_kbbuff(special[i][1]);
    }
      */
      break;

    default:
      if(mode == 0 || mode == 3)// If ASCII mode
      {
        if(!shift)// If shift not pressed,
        { // do a table look-up

          if (spec)
          {for(i = 0; special[i][0]!=sc && special[i][0]; i++);
          if (special[i][0] == sc) {
            put_kbbuff(special[i][1]);
          }

          }
          else
          {
            for(i = 0; unshifted[i][0]!=sc && unshifted[i][0]; i++);
            if (unshifted[i][0] == sc) {
              put_kbbuff(unshifted[i][1]);
            }
          }
        } else {// If shift pressed
          for(i = 0; shifted[i][0]!=sc && shifted[i][0]; i++);
          if (shifted[i][0] == sc) {
            put_kbbuff(shifted[i][1]);
          }
        }
      } else{ // Scan code mode
        put_kbbuff(' ');
        put_kbbuff(' ');
      }
      break;
    }
  }
  else {
    is_up = 0;// Two 0xF0 in a row not allowed
    switch (sc)
    {
    case 0x12 :// Left SHIFT
      shift = 0;
      break;
    case 0x59 :// Right SHIFT
      shift = 0;
      break;
    case 0x05 :// F1
      /*if(mode == 1)
      mode = 2;
      if(mode == 3)
      mode = 0;*/
      break;
    case 0x06 :// F2
      break;
    case 0xE0 :
      spec = 0;
      break;

    }
  }
}


/******************************************************************************
* @fn     put_kbbuff
*
* @brief  Put decoded key into buffer.
*
* @param  unsigned char c
*
* @return void
*
******************************************************************************/
void put_kbbuff(unsigned char c)
{
  if (buffcnt<BUFF_SIZE && start_read == TRUE)// If buffer not full
  {
    *inpt = c;// Put character into buffer
    inpt++; // Increment pointer
    buffcnt++;
    data_ready=TRUE;
    if (inpt >= kb_buffer + BUFF_SIZE)// Pointer wrapping
      inpt = kb_buffer;
    //pc_kbd_scan();    	// call scanning routine
  }
}


/******************************************************************************
* @fn     kb_getchar
*
* @brief  This function can be called by Application layer to check if there is any key pressed..
*
* @param  unsigned char c
*
* @return void
*
******************************************************************************/
char kb_getchar()
{
  char byte;
  while(buffcnt == 0);// Wait for data
  byte = *outpt;// Get byte
  outpt++; // Increment pointer
  if (outpt >= kb_buffer + BUFF_SIZE)// Pointer wrapping
    outpt = kb_buffer;
  buffcnt--; // Decrement buffer count
  data_ready=FALSE;
  return byte;
}

