#define F_CPU 7372800
#define UBRR 51
#include <stdlib.h>
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"

void writeLCD(uint16_t adc){
	lcd_clrscr();
	
	char adcStr[16];
	itoa(adc, adcStr, 10);
	lcd_gotoxy(0, 0);
	lcd_puts("Measured:");
	lcd_gotoxy(0, 1);
	lcd_puts(adcStr);
	
	return;
}

void init_usart(uint8_t);

int main(void)
{
	DDRB = _BV(4);
	
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 64;
	
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	
	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
	
	init_usart(UBRR);
	char adcString[] = "";
	uint8_t i;

	while (1) {
		ADCSRA |= _BV(ADSC);
		
		while(!(ADCSRA & _BV(ADIF)));
		itoa(ADC, adcString, 10);
		strcat(adcString, "\r\n");
		writeLCD(ADC);
		for(i = 0; i < strlen(adcString); i++){
			while ( !(UCSRA & _BV(UDRE)) );
			UDR = adcString[i];
		}
		_delay_ms(500);
	}
}

void init_usart(uint8_t ubrr)
{
	UBRRH = 0;
	UBRRL = (uint8_t) ubrr;
	
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	UCSRC = _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1) | _BV(UPM1) |_BV(USBS);
}
