#ifndef __UART_H_
#define __UART_H_

 
void Uart_Init(void);
void Uart_Send(unsigned char *send_buff,unsigned int len);
void USART_SendData(uint8_t date);
void UART_printf(char *fmt, ...);
void printn(unsigned int n, unsigned int b);
void UART_puts(char *pch);

#endif 