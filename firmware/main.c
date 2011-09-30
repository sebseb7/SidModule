//
//  main.c
//  SidModule
//


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "usart.h"

void nops(void);
void toggle_cs(void);
void write_reg(uint8_t,uint8_t);

#define SID_RESET_DDR DDRD
#define SID_RESET_PORT PORTD
#define SID_RESET_PIN PORTD6
#define SID_RW_DDR DDRD
#define SID_RW_PORT PORTD
#define SID_RW_PIN PORTD4
#define SID_CS_DDR DDRD
#define SID_CS_PORT PORTD
#define SID_CS_PIN PORTD5

//#define soundcheck

uint8_t sid_buf[25];


int main(void)
{
	SID_RESET_DDR |= (1<<SID_RESET_PIN);
	SID_RESET_PORT |= (1<<SID_RESET_PIN);
	SID_RW_DDR |= (1<<SID_RW_PIN);
	SID_CS_DDR |= (1<<SID_CS_PIN);
	SID_CS_PORT |= (1<<SID_CS_PIN);
	
	DDRC = 0xFF;
	DDRA = 0xFF;

    USART0_Init();
    sei();
    
    uint8_t data = 0;

#ifdef soundcheck
	for(uint8_t addr = 0; addr < 25;addr++)
	{
		write_reg(addr,0);
	}
	write_reg(1,0x50);
	write_reg(4,0x21);
	write_reg(5,0x37);
	write_reg(6,0x73);
	write_reg(24,0x08);
#endif

    
    while(1)
    {

#ifdef soundcheck
		write_reg(4,0x21);
		_delay_ms(50);
		write_reg(4,0x20);
		_delay_ms(50);
#endif


		if(USART0_Getc_nb(&data))
		{

			// jump to bootloader        
			if(data == 0x1b)
        	{
				GPIOR2=255;
	    	   	asm volatile("jmp 0xE000");
	        }
	        // flush all 25 registers to the SID
	        if(data == 0xf7)
	        {
				for(uint8_t i = 0; i<25;i++)
				{
					write_reg(i,sid_buf[i]);
				}
	        }
	        else
	        {
				uint8_t sid_addr = data;
			
				USART0_Getc(&data);
				
				uint8_t sid_data = data;

				if(sid_addr < 26)
				{
					sid_buf[sid_addr]=sid_data;
					USART0_putc(' ');
				}
				else
				{
					USART0_putc('X');
				}

	        }
	   	}
    }
}

void write_reg(uint8_t sid_addr, uint8_t sid_data)
{

			PORTC = sid_data;
			PORTA = sid_addr;
			
			toggle_cs();
			
			PORTC = 0xff;
			PORTA = 0xff;

}

void toggle_cs(void)
{
					SID_CS_PORT &= ~(1<<SID_CS_PIN);
					nops();
					SID_CS_PORT |= (1<<SID_CS_PIN);
					nops();
}

void nops(void)
{
	for(uint8_t i = 0; i<30;i++)
	{
		asm volatile("nop");
	}

}



