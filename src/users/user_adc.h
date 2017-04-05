#ifndef	__ADC_H_
#define __ADC_H_  




unsigned int MCU_Temp(void);
unsigned int  KDS_Temp(void);
extern unsigned int  Temp_Collect(void);
extern unsigned int  Vpull_Collect(void);
extern unsigned int Vcp_Collect(void);
extern unsigned int Vcp_ADC(void);
extern unsigned int Vpull_ADC(void);
extern unsigned int  Temp_ADC(void);
extern void DAC_Output(unsigned int Dacval);

#endif
