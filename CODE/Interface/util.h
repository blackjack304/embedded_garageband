/***********************************************************************************

    Filename:     util.h

    Description:  Utility library header file

***********************************************************************************/

#ifndef UTIL_H
#define UTIL_H

/***********************************************************************************
* INCLUDES
*/
#include <hal_types.h>

/***********************************************************************************
* TYPEDEFS
*/

typedef struct {
  const char* szDescr;      // Textual description
  const uint8  value;        // Value
} menuItem_t;

typedef struct {
  const menuItem_t* pMenuItems;
  const uint8 nItems;
} menu_t;




/***********************************************************************************
* CONSTANTS AND DEFINES
*/

// Number of elements in an array
#define N_ITEMS(arr)                sizeof(arr)/sizeof(arr[0])

/***********************************************************************************
* GLOBAL FUNCTIONS
*/
char* convInt32ToText(int32 value);
const char* utilChipIdToStr(uint8 chipID);

#ifndef WIN32
uint8 min(uint8 v1, uint8 v2);
void utilReverseBuf(uint8* pBuf, uint8 length);
#endif

#endif
