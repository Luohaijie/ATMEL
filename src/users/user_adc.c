#include "user_adc.h"
#include "config.h"
#include "cacul.h"


//enum chanl{ch_vp,ch_cp,ch_ts,ch_vdd18};
	
uint16_t buf[20];
/***************************************
���룺��
������ɼ����10λ�¶�
���ܣ��ɼ����Ծ�����¶ȶ�Ӧ�ĵ�ѹֵ,�õ�������
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
���룺��
������ɼ�10λcp��ѹ
���ܣ��ɼ�ʹ���幤��������Ƶ�ʵ�ѹ�ص�ѹ,�õ�������
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
���룺��
������ɼ����10λ��ƫ��ѹֵ
���ܣ��ɼ����Ծ������ƫ��ѹֵ���õ�������
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
