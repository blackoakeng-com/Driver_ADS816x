/*
 * File:   ADC.h
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

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif 
    
#define ADC_NumSampsMax 10
      
SYS_RtnCode adcConvert(uint8_t adcChan, uint16_t *convVal); // adcChan is 1-based

double adcMeanV(uint8_t adcChan, uint8_t numSamps, double *stdDev);


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

