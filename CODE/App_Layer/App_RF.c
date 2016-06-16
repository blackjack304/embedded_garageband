/***********************************************************************************
  Filename:     App_RF.c

  Description:  Application layer RF Comm. APIs

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
#include "string.h"
#include "basic_rf.h"
#include "uart_intfc.h"

#include "App_RF.h"

static basicRfCfg_t basicRfConfig;

/***********************************************************************************
* @fn      Init_AppRF
*
* @brief   Initialize Radio Core
*
* @param   none
*
* @return  none
*/
void Init_AppRF()
{
    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
		basicRfConfig.myAddr = M2ADDR;
		
		// Initialize BasicRF
		if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }
		
		basicRfReceiveOn();
}


/***********************************************************************************
* @fn      RF_Send
*
* @brief   Send RF Data to specified Device.
*
* @param   
*
* @return  
*/
bool RF_Send(unsigned char *bufptr, unsigned int len)
{
	basicRfReceiveOff();
  for(uint8 i = 0; i<5; i++) {
    if(basicRfSendPacket(M1ADDR, bufptr, len) == SUCCESS)  {
			basicRfReceiveOn();
			return true;
		}
	}
	basicRfReceiveOn();
  return false;
}


/***********************************************************************************
* @fn      RF_Peek
*
* @brief   Check if we received any packets.
*
* @param   
*
* @return  1 if any packets received
*/
bool RF_Peek()
{
  return basicRfPacketIsReady();
}


/***********************************************************************************
* @fn      RF_Receive
*
* @brief   Receive any buffered packet.
*
* @param   
*
* @return  
*/
uint8 RF_Receive(unsigned char *bufptr)
{
  return basicRfReceive(bufptr, 255, NULL);
}


uint8 RF_gets_blk(unsigned char *bufptr) {

  while (!RF_Peek());	//block
  return RF_Receive(bufptr);	
}


uint8 RF_gets_nblk(unsigned char *bufptr) {

  if (RF_Peek()) {
  	return RF_Receive(bufptr);	
	}
	else return 0;
}


bool RF_puts(unsigned char *bufptr) {
  return RF_Send(bufptr, strlen((const char*)bufptr));
}


bool RF_puts_SD (unsigned char *bufptr, unsigned int len) {
	basicRfReceiveOff();
  for(uint8 i = 0; i<5; i++) {
    if(basicRfSendPacket(M3ADDR, bufptr, len) == SUCCESS)  {
			basicRfReceiveOn();
			return true;
		}
	}
	basicRfReceiveOn();
  return false;
}