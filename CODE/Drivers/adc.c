/******************************************************************************
    Filename: adc.c

    These functions/macros simplifies usage of the ADC.

    Code leveraged from Texas Instruments, Inc.
	Modified by:	Niket Shah
					Maulik Kapuria
					Vishal Verma
******************************************************************************/

#include "adc.h"
#include <ioCC2431.h>


/*!
* @fn  adcSampleSingle
*
* @brief
*      This function makes the adc sample the given channel at the given
*      resolution with the given reference.
*
* @param uint8 reference
*          The reference to compare the channel to be sampled.
*        uint8 resolution
*          The resolution to use during the sample (8, 10, 12 or 14 bit)
*        uint8 input
*          The channel to be sampled.
*
* @return int16
*          The conversion result
 */
int16 adcSampleSingle(uint8 reference, uint8 resolution, uint8 channel)
{
    int16 value;

    ADC_ENABLE_CHANNEL(channel);

    ADCIF = 0;

    ADC_SINGLE_CONVERSION(reference | resolution | channel);
    while(!ADCIF);

    value  = (ADCH << 8) & 0xff00;
    value |= ADCL;

    ADC_DISABLE_CHANNEL(channel);

    //  The variable 'value' contains 16 bits where
    //     bit 15 is a sign bit
    //     bit [14 .. 0] contain the absolute sample value
    //     Only the r upper bits are significant, where r is the resolution
    //     Resolution:
    //        12   -> [14 .. 3] (bitmask 0x7FF8)
    //        10   -> [14 .. 5] (bitmask 0x7FE0)
    //         9   -> [14 .. 6] (bitmask 0x7FC0)
    //         7   -> [14 .. 8] (bitmask 0x7F00)
    
    value = (value & 0x7FF8) >> 3;

    return value;
}