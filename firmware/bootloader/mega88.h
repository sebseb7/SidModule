#ifndef _MEGA88_H_
#define _MEGA88_H_

/* Part-Code ISP */
#define DEVTYPE_ISP     0x76
/* Part-Code BOOT */
#define DEVTYPE_BOOT    0x77

#define SIG_BYTE1	0x1E
#define SIG_BYTE2	0x93
#define SIG_BYTE3	0x0F

#define UART_BAUD_HIGH	UBRR0H
#define UART_BAUD_LOW	UBRR0L
#define UART_STATUS	UCSR0A
#define UART_TXREADY	UDRE0
#define UART_RXREADY	RXC0
#define UART_DOUBLE	U2X0
#define UART_CTRL	UCSR0B
#define UART_CTRL_DATA	((1<<TXEN0) | (1<<RXEN0))
#define UART_CTRL2	UCSR0C
#define UART_CTRL2_DATA	(1<<UCSZ00)|(1<<UCSZ01)//((1<<URSEL0) | (1<<UCSZ10) | (1<<UCSZ00))
#define UART_DATA	UDR0

#endif // #ifndef _MEGA88_H_
