#include <asf.h>
#include <conf_usart_example.h>
#include "user_uart.h"
#include "config.h"


void Uart_Send(unsigned char *send_buff,uint16_t len )
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		USART_SendData(send_buff[i]);
	}
}

void USART_SendData(uint8_t date)
{
	usart_put(USART_SERIAL_EXAMPLE, date);
	while (!usart_tx_is_complete(USART_SERIAL_EXAMPLE)) {}
	usart_clear_tx_complete(USART_SERIAL_EXAMPLE);

}

void Uart_Init(void)
{
		/* USART options. */
		static usart_xmegae_rs232_options_t USART_SERIAL_OPTIONS = {
			.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
			.charlength = USART_SERIAL_CHAR_LENGTH,
			.paritytype = USART_SERIAL_PARITY,
			.stopbits = USART_SERIAL_STOP_BIT,
			.start_frame_detection = true,
			.one_wire = false,
			.pec_length = 0,
			.encoding_type = USART_DECTYPE_DATA_gc,
			.encoding_stream = USART_LUTACT_OFF_gc,
		};
		const usart_serial_options_t usart_serial_options = {
			
			.baudrate   = USART_SERIAL_EXAMPLE_BAUDRATE,
			.charlength = USART_SERIAL_CHAR_LENGTH,
			.paritytype = USART_SERIAL_PARITY,
			.stopbits   = USART_SERIAL_STOP_BIT,
		};

		/* Initialize usart driver in RS232 mode */
		usart_xmegae_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);
		usart_set_rx_interrupt_level(USART_SERIAL_EXAMPLE, USART_INT_LVL_LO);
		stdio_serial_init(USART_SERIAL_EXAMPLE, &usart_serial_options);
		
}

#if UART_PRINTF
void UART_puts(char *pch)
{
	while(*pch != '\0')
	{
		USART_SendData(*pch);
		pch++;
	}
}

void printn(unsigned int n, unsigned int b)
{
	unsigned char ntab[17] = "0123456789ABCDEF";
	unsigned char tab[11];
	signed char i=0;
	if(n==0) USART_SendData(ntab[0]);
	while (n)
	{
		i++;
		if(i==11)i=0;
		tab[i]=ntab[n%b];
		n = n / b;

	}
	while(i>0)
	{							   
		USART_SendData(tab[i]);
		i--;
	}

}


void UART_printf(char *fmt, ...)
{
    char c;
    unsigned int *adx = (unsigned int*)(void*)&fmt + 1;
_loop:
    while((c = *fmt++) != '%')
		{
        if (c == '\0') return;
        USART_SendData(c);
    }
    c = *fmt++;
    if (c == 'd' || c == 'l')				   
		{
        printn(*adx, 10);
    }
    if (c == 'o' || c == 'x')
		{
        printn(*adx, c=='o'? 8:16 );
    }
    if (c == 's')
		{
			UART_puts((char*)*adx);
    }
    adx++;
    goto _loop;
}
#endif
