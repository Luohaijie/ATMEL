#ifndef __TIMER_H_
#define __TIMER_H_
/*
extern unsigned int PWM_UP,PWM_LOW ;	
extern unsigned int PWM_UP_H,PWM_UP_L,PWM_LOW_H,PWM_LOW_L;
extern unsigned int PWM_flag ;	
*/

void Timer2_Init(void);
void Timer1_Init(void);
void Timer0_Init(void);
void PWM_Init(unsigned int u);
void Pwm_Load(unsigned int pwm_high);
void delay_ms(unsigned int i);
void Delay_100us(unsigned char time);

#endif