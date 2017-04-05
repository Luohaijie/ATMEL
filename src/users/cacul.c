#include <asf.h>
#include "cacul.h"
#include "user_adc.h"
#include "function.h"
/*******************************************************
输入：no
输出：K,B
功能：计算Y=KX+B
********************************************************/
/*
void Calc_K_B_Tenbit(void)
{
	unsigned int vcp500,vcp1500;
	unsigned int i;
	unsigned int temp_buff[10];

	
	PWM_Init(300);
	delay_ms(500);
	
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect()/2;
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
    vcp500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	PWM_Init(800);
	delay_ms(500);				
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect()/2;
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
	vcp1500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	K = 500.00/(vcp1500-vcp500); 
	Badd =300-(int)(K*vcp500);
	

}
void Calc_K_B_Elebit(void)
{
	unsigned int vcp500,vcp1500;
	unsigned int i;
	unsigned int temp_buff[10];

	
	Pwm_Load(500);
	delay_ms(500);
	
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect();
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
    vcp500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	Pwm_Load(1500);
	delay_ms(500);				
	for(i=0;i<10;i++)
	{
		temp_buff[i]=Vcp_Collect();
		delay_ms(1);
	}
    Insert_Sort(temp_buff,10);
	vcp1500 = (temp_buff[4]+temp_buff[5]+temp_buff[6]+temp_buff[7])>>2;
	K = 1000.00/(vcp1500-vcp500); 
	Badd =500-(int)(K*vcp500);
}
*/
void Calc_K_B(void)
{
	uint16_t temp,table[10];
	uint8_t i;
/*
	#if TEN_BIT
		Calc_K_B_Tenbit();
	#else
		Calc_K_B_Elebit();
	#endif
*/
	K=1.0;
	DAC_Output(2000);
	for(i=0;i<10;i++){
		table[i] = Vcp_Collect();
		delay_ms(100);		 
	}
	Insert_Sort(table,10);
	temp = (table[3]+table[4]+table[5]+table[6])/4;
	Badd = (int)(2000-temp);

}

/***************************************
输入：数据和大小
输出：无
功能：冒泡排序
****************************************/
void Insert_Sort (unsigned int *Data,unsigned short size) 
{ 
	 int16_t temp;
	 int16_t j;
	 int16_t i;
	
	for (i = 0; i < size-1; i++) 
	{  		
		for (j=0; j<size-i-1; j++)
		{
			if(Data[j]>Data[j+1])
			{
				temp = Data[j+1];
		      	Data[j+1] = Data[j];
		    	Data[j] = temp;				
			}
		}
		
	}
} 
/********************************************************************
函数名：
输入：
输出：
作用：拟合曲线
********************************************************************/
/*
void Line_Fitting(unsigned int databuf[],unsigned char first_flag,unsigned char length)
{
	
	unsigned int first_val,last_val;
	float temp_val;
	unsigned char i;
	first_val = databuf[first_flag];
	last_val = databuf[length];
	if(last_val>first_val)
	{
		temp_val = (last_val-first_val)/(length-first_flag);
		for(i=first_flag;i<=length;i++)
		databuf[i+1]=(int)(databuf[first_flag]+i*temp_val);
	}
	else 
	{
		temp_val = (first_val-last_val)/(length-first_flag);
		for(i=first_flag;i<length;i++)
		databuf[i+1]=(int)(databuf[first_flag]-i*temp_val);
	}

} 		*/