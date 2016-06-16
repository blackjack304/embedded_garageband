/***********************************************************************************
  Filename:     App_RF.h

  Description:  Application layer RF Comm. APIs

	Developed by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
***********************************************************************************/
#ifndef APP_RF_H
#define APP_RF_H

#include "hal_defs.h"
#include "hal_types.h"



// Application parameters
#define RF_CHANNEL                25      // 2.4 GHz RF channel

// BasicRF address definitions
#define PAN_ID                0x2007
#define APP_PAYLOAD_LENGTH        4
#define LIGHT_TOGGLE_CMD          0

#define M1ADDR					      0xBEEF
#define M2ADDR         				0x2520
#define M3ADDR         				0x5050
#define LOGGER_ADDR           0x1234

#define RF_TX  1
#define RF_RX  2


enum {
   PingReq                  = 1,
   PingRsp                  = 2,
   AckRsp                   = 3,
   Toggle_LED_Req           = 4,
   Ref_Data                 = 5,
   Loc_Data                 = 6
   }Packet_Type;


typedef struct {
   uint8    X_Coord;
   uint8    Y_Coord;
   uint8    Ref_Node;
}Ref_DataPayload;


typedef struct {
   uint8    X_Coord;
   uint8    Y_Coord;
   uint8    Blind_Node;
}Loc_DataPayload;


void Init_AppRF(void);
bool RF_Send(unsigned char *bufptr, unsigned int len);
bool RF_Peek();
uint8 RF_Receive(unsigned char *bufptr);
uint8 RF_gets_blk(unsigned char *bufptr);
uint8 RF_gets_nblk(unsigned char *bufptr);
bool RF_puts(unsigned char *bufptr);
bool RF_puts_SD (unsigned char *bufptr, unsigned int len);

#endif