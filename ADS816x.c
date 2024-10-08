/*
 * File:   ADS816x.c
 * Driver source file for TI ADS816x
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
#include "Advant.h"
#include "spi.h"
#include "ADS816x.h"
#include "Timers.h"
#include "Serial.h" // DEB

SYS_RtnCode ads8166_Init(uint8_t adcHandle)
{
    uint8_t dataFrame[3] = { 0x00, 0x00, 0x00 };     // Tx first first
    SYS_RtnCode retVal = RTN_Executed;
   
    ADC1_CS_n = ADC2_CS_n = 1;    
    if (adcHandle == 1) ADC1_CS_n = 0;
    else if (adcHandle == 2) ADC2_CS_n = 0;
    else return RTN_IndexOutOfRange;
    
    dataFrame[0] = ADS8166_CMD_WR_REG << 3;     // Note, frame does not coincide with 3 bytes. See notes.
    dataFrame[1] = ADS8166_REG_ACCESS;  
    dataFrame[2] = ADS8166_REG_ACCESS_KEY;      // Unlock. We'll leave unlocked.               
    spi1ByteExchange(dataFrame[0]);
    spi1ByteExchange(dataFrame[1]);
    spi1ByteExchange(dataFrame[2]);      

    ADC1_CS_n = ADC2_CS_n = 1;  
    return retVal;   
}

SYS_RtnCode ads8166_TestMode(uint8_t adcHandle)
{
    uint8_t dataFrame[3] = { 0x00, 0x00, 0x00 };     // Tx first first
    uint8_t readFrame[3] = { 0x00, 0x00, 0x00 };     
    SYS_RtnCode retVal = RTN_Executed;
   
    ADC1_CS_n = ADC2_CS_n = 1;    
    if (adcHandle == 1) ADC1_CS_n = 0;
    else if (adcHandle == 2) ADC2_CS_n = 0;
    else return RTN_IndexOutOfRange;
    
    dataFrame[0] = ADS8166_CMD_WR_REG << 3;     // Note, frame does not coincide with 3 bytes. See notes.
    dataFrame[1] = ADS8166_DATA_CNTL;  
    dataFrame[2] = 0x01;      // Will output fake 0xA6 now              
    spi1ByteExchange(dataFrame[0]);
    spi1ByteExchange(dataFrame[1]);
    spi1ByteExchange(dataFrame[2]);      

    ADC1_CS_n = ADC2_CS_n = 1;  
    delay_us(5);
    if (adcHandle == 1) ADC1_CS_n = 0;
    else if (adcHandle == 2) ADC2_CS_n = 0;
    else return RTN_IndexOutOfRange;
    
    dataFrame[0] = ADS8166_CMD_RD_REG << 3;     // Note, frame does not coincide with 3 bytes. See notes.
    dataFrame[1] = ADS8166_DATA_CNTL;  
    dataFrame[2] = 0x00;                
    spi1ByteExchange(dataFrame[0]);
    spi1ByteExchange(dataFrame[1]);
    readFrame[0] = spi1ByteExchange(dataFrame[2]);
    readFrame[1] = spi1ByteExchange(dataFrame[1]);
    readFrame[2] = spi1ByteExchange(dataFrame[2]);
    ADC1_CS_n = ADC2_CS_n = 1;  
    
    sprintf(ioBfr, "[%02X %02X %02X]", readFrame[0], readFrame[1], readFrame[2]);   //  DEB
    putStr(ioBfr);
    return retVal;   
}