#include "user_adc.h"
#include "config.h"
#include "cacul.h"


//enum chanl{ch_vp,ch_cp,ch_ts,ch_vdd18};
	
uint16_t buf[20];
/***************************************
输入：无
输出：采集后的10位温度
功能：采集来自晶体的温度对应的电压值,得到数字量
****************************************/
#if MCU_temperture
unsigned int MCU_Temp(void)
{
    uint16_t adcVal=0;  
	adc_tempsense_select();
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adcVal = adc_get_unsigned_result(&ADCA, ADC_CH0);
	return	adcVal;

}
#else
unsigned int  KDS_Temp(void)
{
    uint16_t adcVal=0;
	return adcVal;
}

#endif
unsigned int  Temp_ADC(void)
{
    #if MCU_temperture
    	return MCU_Temp();
   	#else
   		return KDS_Temp();
   	#endif
}

unsigned int Temp_Collect(void)
{
	uint8_t i,j;
	uint16_t Data;
	long Sum=0;
	for(j=0;j<8;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Temp_ADC();

		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>3;
	return(Data);
}

/*****************************************
输入：无
输出：采集10位cp电压
功能：采集使晶体工作在中心频率的压控电压,得到数字量
******************************************/
unsigned int Vcp_ADC(void)
{
	uint16_t adcVal=0;
	    
	adc_adcch3_select();
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adcVal = adc_get_unsigned_result(&ADCA, ADC_CH0);
	
	return	adcVal;
}


unsigned int Vcp_Collect(void)
{
    uint8_t i,j;
    uint16_t Data;
    long Sum=0;
	for(j=0;j<8;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Vcp_ADC();

		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>3;
	return(Data);
}

/**************************************
输入：无
输出：采集后的10位拉偏电压值
功能：采集来自晶体的拉偏电压值，得到数字量
**************************************/
unsigned int Vpull_ADC(void)
{	
	uint16_t adcVal=0;
    
	adc_adcch5_select();
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adcVal = adc_get_unsigned_result(&ADCA, ADC_CH0);
	return	adcVal;
}

unsigned int Vpull_Collect(void)
{
    uint8_t i,j;
    uint16_t Data;
    long Sum=0;
	for(j=0;j<8;j++)
	{
		for(i=0;i<20;i++)
		{
			buf[i]=Vpull_ADC();
		}
		Insert_Sort(buf,20);
		Data = (buf[6]+buf[7]+buf[8]+buf[9]+buf[10]+buf[11]+buf[12]+buf[13])>>3;
		Sum += Data;
	}
	Data = Sum>>3;
	return(Data);
}
void DAC_Output(uint16_t Dacval)
{
	main_dac_output(Dacval);
}
