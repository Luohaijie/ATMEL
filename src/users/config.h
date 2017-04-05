#ifndef _CONFIG_H
#define _CONFIG_H

#include <asf.h>

#define MCU_temperture    1  //MCU=true;KDS=false
//#define TEN_BIT 1
#define LOCK_SELF         0
#define CORRECT_DEBUG     0  
#define CORRECT_MODE      1
#define UART_PRINTF	      0
#define ATUO_FALLFLASH    0

#define CMOS 0x02
#define CSIN 0x03

extern void main_adc_init(void);
extern void main_adc_correction_start(void);
extern void adc_tempsense_select(void);
extern void adc_adcch3_select(void);
extern void adc_adcch5_select(void);
extern void main_dac_init(void);
extern void main_dac_output(int16_t volt);

#endif