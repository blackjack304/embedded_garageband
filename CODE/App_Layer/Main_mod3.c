/***********************************************************************************
  Filename:     Main_mod3.c

  Description:  Main.c file for Hardware Module 3.

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include <hal_led.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "uart_intfc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_spi.h"
#include "TimerManager.h"
#include <mmc2.h>
#include "App_RF.h"


unsigned char sd_buffer[512];
char tx_buf[15];
uint8 Rf_Buf[20];
unsigned int rec_size;

static basicRfCfg_t basicRfConfig;
static unsigned char App_State = 0;
static unsigned int note_count;
static unsigned int Capture_Value = 0;
static unsigned int Delay_Value = 0;
bool Timer_Expired = 0;
bool Playback_Timer_Expired;
static unsigned char SD_Sector = 0xFF;


const char Record_0[] = "Record_0";
const char Record_1[] = "Record_1";
const char Record_2[] = "Record_2";
const char Record_3[] = "Record_3";
const char Play_0[] = "Play_0";
const char Play_1[] = "Play_1";
const char Play_2[] = "Play_2";
const char Play_3[] = "Play_3";

   //--------------------------------------------------------------------------
   //  Timers:
   //--------------------------------------------------------------------------
   TimerId timerID1 = TimerId_INVALID;
   TimerId timerID2 = TimerId_INVALID;


/***********************************************************************************
* CONSTANTS
*/


/***********************************************************************************
* LOCAL VARIABLES
*/


/***********************************************************************************
* LOCAL FUNCTIONS
*/
void MMC_Test();
void App_ISR1(TimerId timerId);
void App_ISR2(TimerId timerId);
void Receive_Notes();
void PlayBack(unsigned char Track_No);



/***********************************************************************************
* @fn      Main
*
* @brief   Module 3 Main Function.
*
* @param   none
*
* @return  none
*/
void main(void)
{
    // Initalise board peripherals
    halBoardInit();    
    uart_intfc_init();   
    tx1_send("Hello World, MMC\n\r", 18);
    
    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;

      // Initialize BasicRF
    basicRfConfig.myAddr = 0x5050;
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
    
    basicRfReceiveOn();  
    memset(sd_buffer, 0, 512);
    TimerInterface_Initialize();
    
    while(SD_init() != 0);
    tx1_send("Init Complete\n\r", 15);
    
    App_State = 0;
    
    while(1)
    {
      if(App_State == 0)
      {
        while(!basicRfPacketIsReady());
        rec_size = basicRfReceive(Rf_Buf, 255, NULL);
        tx1_send(Rf_Buf, rec_size);
          if(strncmp((char *)Rf_Buf, Record_0,8) == 0)
          {
            SD_Sector = 0;
            App_State = 1;
            Receive_Notes();
          }
          else if(strncmp((char *)Rf_Buf, Record_1,8) == 0)
          {
            SD_Sector = 1;
            App_State = 1;
            Receive_Notes();
          }
          else if(strncmp((char *)Rf_Buf, Record_2,8) == 0)
          {
            SD_Sector = 2;
            App_State = 1;
             Receive_Notes();
          }
          else if(strncmp((char *)Rf_Buf, Record_3,8) == 0)
          {
            SD_Sector = 3;
            App_State = 1;
            Receive_Notes();
          }  
          else if(strncmp((char *)Rf_Buf, Play_0,6) == 0)
          {
            App_State = 2;
            //tx1_send("Playback Start1\n\r", 17);
            PlayBack(0);
          }
          else if(strncmp((char *)Rf_Buf, Play_1,6) == 0)
          {
            App_State = 2;
            PlayBack(1);
          }
          else if(strncmp((char *)Rf_Buf, Play_2,6) == 0)
          {
            App_State = 2;
            PlayBack(2);
          }
          else if(strncmp((char *)Rf_Buf, Play_3,6) == 0)
          {
            App_State = 2;
            PlayBack(3);
          }
      }
    }
}


/***********************************************************************************
* @fn      Receive_Notes
*
* @brief   Receive Wireless Notes and store in SD cards.
*
* @param   none
*
* @return  none
*/
void Receive_Notes()
{
  Capture_Value = 0;
  memset(sd_buffer, 0, 512);
  note_count = 0;
  
  basicRfReceiveOn();

  
  
  while((note_count<96) && App_State ==1 )
  {
    while(!basicRfPacketIsReady() || Timer_Expired);
    rec_size = basicRfReceive(Rf_Buf, 255, NULL);
    if(rec_size == 3)
    {
      //Cancel Timer, Capture Value
      Capture_Value = 40000 - Capture_TimerReq(timerID1);
      CancelTimerReq(timerID1);
      timerID1 = TimerId_INVALID;
      
      timerID1 = SetTimerReq(&App_ISR1, 40000);
      Timer_Expired = 0;

      
      //Reset Timer
      sd_buffer[(note_count*5)+0] = (unsigned char)(Capture_Value & 0x00ff);
      sd_buffer[(note_count*5)+1] = (unsigned char)((Capture_Value & 0xff00)>>8);
      
      memcpy(&sd_buffer[(note_count*5)+2], Rf_Buf, 3);
      
      tx1_send(&sd_buffer[(note_count*5)+0], 5);
      note_count++;
       
      if(Rf_Buf[0] == 0xff && Rf_Buf[1] == 0xff && Rf_Buf[2] == 0xff)
      {
        App_State = 0;
        break;
      }
    } 
    
  }
    tx1_send("Record Exit\n\r", 13);
  
    CancelTimerReq(timerID1);
    timerID1 = TimerId_INVALID;
    
    
    
    SD_writeSingleBlock((unsigned long)SD_Sector);
    basicRfReceiveOn();
}


/***********************************************************************************
* @fn      PlayBack
*
* @brief   Transmit recorded Notes from SD card.
*
* @param   none
*
* @return  none
*/
void PlayBack(unsigned char Track_No)
{
  tx1_send("Playback Start2\n\r", 17);
  basicRfReceiveOff();
  if(Track_No > 20)
  {
    //Fetch Play Error Sound
    Track_No = 0;
  }
  SD_readSingleBlock((unsigned long)Track_No);
  note_count = 0;
  Delay_Value = 5;
  Playback_Timer_Expired = 0;
  App_ISR2(2);
  while(App_State == 2)
  {
      while(Playback_Timer_Expired == 0);
      Playback_Timer_Expired = 0;
      basicRfSendPacket(0x2520, &sd_buffer[((note_count)*5)+2], 3);
      note_count++; 
      
      Delay_Value = (unsigned int)((sd_buffer[((note_count)*5)+1])<<8) | (unsigned int)((sd_buffer[((note_count)*5)]) & 0x00ff);
      if(Delay_Value<10)  Delay_Value = 10;
      timerID2 = SetTimerReq(&App_ISR2,Delay_Value);
      
      tx1_send(&sd_buffer[((note_count-1)*5)+0], 5);
      
      
      if(note_count > 95 || (sd_buffer[((note_count)*5)+2] == 0xff && sd_buffer[((note_count)*5)+3] == 0xff && sd_buffer[((note_count)*5)+4] == 0xff))
      {
        CancelTimerReq(timerID2);
        timerID2 = TimerId_INVALID;
        basicRfSendPacket(0x2520, &sd_buffer[((note_count)*5)+2], 3);
        App_State = 0;
        break;
      }
  }
  App_State = 0;
  basicRfReceiveOn();
}


/***********************************************************************************
* @fn      App_ISR1
*
* @brief   Timer ISR to record delay between two notes received.
*
* @param   none
*
* @return  none
*/
void App_ISR1(TimerId timerId)
{
  timerID1 = TimerId_INVALID;
  Timer_Expired = 0;
}

/***********************************************************************************
* @fn      App_ISR2
*
* @brief    Timer ISR to generate delay between two notes to be transmitted.
*
* @param   none
*
* @return  none
*/
void App_ISR2(TimerId timerId)
{
  timerID2 = TimerId_INVALID;
  Playback_Timer_Expired = 1;
}