/*
 * File:   ADC.c
 * HAL source file for TI ADS816x
 * Version 0.1
 * Author:  M Hoyt
 * Company: Black Oak Engineering
 *   blackoakeng.com - New York USA
 * Developed and tested on BlueJay v1.3 platform
 *   + custom Shield.
 * Copyright 2023 - All Rights Reserved
 * License: License_BOE_MIT.txt included herewith.
 * All information herein from TI Nov 2019 datasheet.
 * This source code is not intended for life support 
 * functions. No warranty is made as to its suitability 
 * for any particular application. 
 */

#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "Advant.h"
#include "ADS816x.h"
#include "ADC.h"
#include "Spi.h"
#include "Serial.h"
#include "Timers.h"

#define ADC_CT_2_V          3.0 / 4096       // Vref+ / 12b

// First conversion can often be grossly off, esp with floating inputs. 
// So it is always discarded, and one extra taken.
double adcMeanV(uint8_t adcChan, uint8_t numSamps, double *stdDev)
{
    uint8_t sa;
    double d, dAcc = 0, dMean; 
    uint16_t uArray[ADC_NumSampsMax + 1];  
    if (numSamps < 1 || numSamps > ADC_NumSampsMax) return(0);
    for (sa = 0; sa < numSamps + 1; sa++)
    {
        d = adcConvert(adcChan); 		// + adcOffsetCt[adcChan]; Not used here      
        uArray[sa] = d;
        dAcc += uArray[sa];
    }
    dAcc -= uArray[0];
    dMean = dAcc / numSamps;
    dAcc = 0;
    for (sa = 0; sa < numSamps + 1; sa++)
    {
        sprintf(ioBfr, "\t%u", uArray[sa]);
        putStr(ioBfr);
        d = uArray[sa] - dMean; 
        dAcc += d * d;        
    }
    dAcc /= (numSamps + 1);
    *stdDev = sqrt(dAcc);
    *stdDev *= ADC_CT_2_V;          // Convert to V
    dMean *= ADC_CT_2_V;
    return(dMean);
}

SYS_RtnCode adcConvert(uint8_t adcChan, uint16_t *convVal) // adcChan is 1-based.  There are 16 chans.
{
    uint8_t dataFrame[3] = { 0x00, 0x00, 0x00 };     // Tx first first
    uint8_t convFrame[3];
    uint8_t ads8166Chan;        // There are two 8-chan ADCs, 0-based
    SYS_RtnCode retVal = RTN_Executed;
   
    ADC1_CS_n = ADC2_CS_n = 1;    
    if (adcChan > 0 && adcChan <= 8) 
    {
        ADC1_CS_n = 0;
        ads8166Chan = adcChan - 1;
    }
    else if (adcChan > 8 && adcChan <= 16) 
    {
        ADC2_CS_n = 0;
        ads8166Chan = adcChan - 9;
    }
    else return RTN_IndexOutOfRange;
    
    dataFrame[0] = ADS8166_CMD_WR_REG << 3;     // Note, frame does not coincide with 3 bytes. See notes.
    dataFrame[1] = ADS8166_CHANNEL_ID;  
    dataFrame[2] = ads8166Chan;                 
    spi1ByteExchange(dataFrame[0]);
    spi1ByteExchange(dataFrame[1]);
    spi1ByteExchange(dataFrame[2]);      
    ADC1_CS_n = ADC2_CS_n = 1;              // Conversion starts. Theoretically 2.5 us Hold time
    delay_us(5);   
    if (adcChan > 0 && adcChan <= 8) ADC1_CS_n = 0;
    else ADC2_CS_n = 0;

    dataFrame[0] = ADS8166_CMD_NOP;     
    dataFrame[1] = 0x00;  
    dataFrame[2] = 0x00;                 
    convFrame[0] = spi1ByteExchange(dataFrame[0]);
    convFrame[1] = spi1ByteExchange(dataFrame[1]);
    convFrame[2] = spi1ByteExchange(dataFrame[2]); 
    
    sprintf(ioBfr, "[%02X %02X %02X]", convFrame[0], convFrame[1], convFrame[2]);// DEB
    putStr(ioBfr);
    
    ADC1_CS_n = ADC2_CS_n = 1;  
    *convVal = convFrame[0];
    *convVal <<= 8;
    *convVal += convFrame[1];
    return retVal;   
}

    

