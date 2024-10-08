/*
 * File:   ADS816x.h
 * Driver header file for TI ADS816x
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

#ifndef ADS8166_H
#define	ADS8166_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    // Handles for the 2 chips
#define   ADC1       1  
#define   ADC2       2    
    
    // Commands. These are the first 5 bits of a 24 bit frame.
#define  ADS8166_CMD_NOP        0b00000       // Followed by 0s
#define  ADS8166_CMD_WR_REG     0b00001       // Followed by 11b addr, then 8b datum
#define  ADS8166_CMD_RD_REG     0b00010       // Followed by 11b addr, then 0s   
#define  ADS8166_CMD_SET_BITS   0b00011       // Followed by 11b addr, then 8b unmasked bits to set
#define  ADS8166_CMD_CLR_BITS   0b00100       // Followed by 11b addr, then 8b unmasked bits to clr
    
    // Registers (only an MVP subset)
#define ADS8166_REG_ACCESS  0x00    // Enable rd/wr access to regs
#define ADS8166_REG_ACCESS_KEY  0xAA
    // Write 0xAA code to ena access to regs. Anything else disables.
    
#define ADS8166_PD_CNTL     0x04    // Ena/disa control for reference, ref bfr, REFby2, ADC
    // Selectively turn off power to modules [Unneeded]

#define ADS8166_SDI_CNTL    0x08
    // Default 0x00 for SDI_MODE[1:0] = 00: CPOL = 0 & CPHASE = 0, 'Normal SPI'

#define ADS8166_SDO_CNTL1   0x0C         
    // Defaults [00h]: Length out data set by DATA_OUT_FORMAT is DATA_CNTL reg.
    // Left aligned data. If data frame is 32 b, will have 0s appended.
    // Data bits output only on SDO-0.	

#define ADS8166_SDO_CNTL2   0x0D   
    // Output data rate, 0 = x for SDO_MODE[1:0] = 00

#define ADS8166_DATA_CNTL   0x10
    // Output data word config
    // If set bit 0 H will output A6h for debug.
    // Defaults: Output data bits only, no addr.

#define ADS8166_CHANNEL_ID  0x1D    
    // 7:3 reserved, 2:0 AIN7:0

#define ADS8166_AIN_CFG     0x24
    // Default 0x00 for Config 1 (8 single ended inputs)
    
#define ADS8166_COM_CFG     0x27
    // Default [Bit0 = 0] All chans single-ended.
    
#define ADS8166_DEVICE_CFG  0x1C
    // Default [1:0] = 00 = Manual mode

#define ADS8166_CHANNEL_ID  0x1D
    // Use [2:0] Manual mode chan select [**]


SYS_RtnCode ads8166_Init(uint8_t adcHandle);
SYS_RtnCode ads8166_TestMode(uint8_t adcHandle);

#ifdef	__cplusplus
}
#endif

#endif	/* ADS8166_H */

