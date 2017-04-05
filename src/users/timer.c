#include <CS1521.H>

#include "timer.h"
#include "function.h"


//extern unsigned char flag_uart;
/*
void Timer0_Init(void)
{
	 NOP;
}
*/
/********************************************************************
函数名：
输入：
输出：
作用：定时器初始化
*********************************************************************/
/*
void Timer1_Init(void)
{
	   NOP;
} 

void Timer2_Init(void)
{	
	   NOP;
}

void Pwm_Load(unsigned int pwm_high)
{
	PCMPH = pwm_high>>8;
	PCMPL = pwm_high&0xff;    //duty ratio  pwm_high/3FFF
	PLOADH =0x3F;
	PLOADL =0xFF;    //cycle 0~3FFF 
	PCTRL = 0x04;    //enable pwm , Fosc/1
}
  */
/********************************************************************
函数名：delay_ms
输入：
输出：
作用：延时1毫秒
*********************************************************************/
void	Delay_100us(unsigned char time)	//8Mhz内部RC震荡延时Xus  
{
	
	uchar i;
    uchar j;
	for(i=0;i<time;i++)
	{
	    for(j=0;j<50;j++)
			{
//					IEN0|=0X40;
//					IEN1|=0X40;
			}
							
	};
		
}

void	delay_ms(unsigned int time)	//8Mhz内部RC震荡延时Xms  
{
	uint16 i;
	for(i=0;i<time;i++)
	{
	   if(flag_uart == 1)
		{
				break;
		}
		Delay_100us(10);
  
	}
		

}

/***************************************
函数名：12位PWM波形
输入：PWM_UP
输出：
功能：
****************************************/
/*
void PWM_Init(unsigned int u)
{
	PCMPH = u>>8;
	PCMPL = u&0xff;    //duty ratio  1/3FFF
	PLOADH =0x3F;
	PLOADL =0xFF;    //cycle 0~3FFF 
	PCTRL = 0x04;    //enable pwm , Fosc/1
}
  */