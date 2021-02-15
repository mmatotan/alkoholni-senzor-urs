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

	lcd_gotoxy(0, 1);
	lcd_puts(adcStr);
	
	return;
}

void init_LCD(){
	DDRB = _BV(4);
		
	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 32;
		
	lcd_init(LCD_DISP_ON);
	
	return;
}

void init_ADC(){
	ADMUX = _BV(REFS0);
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
	
	return;
}

void init_usart(uint8_t ubrr)
{
	UBRRH = 0;
	UBRRL = (uint8_t) ubrr;
	
	UCSRB = _BV(RXEN) | _BV(TXEN) | _BV(RXCIE);
	UCSRC = _BV(URSEL) | _BV(UCSZ0) | _BV(UCSZ1) | _BV(UPM1) |_BV(USBS);
	
	return;
}

void init_sensor(){
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts("Alcohol sensor");
	lcd_gotoxy(0, 1);
	lcd_puts("warming up.");
	_delay_ms(20000);
	
	lcd_clrscr();
	lcd_gotoxy(0, 0);
	lcd_puts("Alcohol sensor");
	lcd_gotoxy(0, 1);
	lcd_puts("ready for use.");
	_delay_ms(5000);
	
	return;
}

// Transmit UART
void send_data(char * adcString){
	for(uint8_t i = 0; i < strlen(adcString); i++){
		while ( !(UCSRA & _BV(UDRE)));
		UDR = adcString[i];
	}
	
	return;
}

int main(void)
{
	init_LCD();
	init_ADC();
	init_usart(UBRR);	
	init_sensor();
	
	char adcString[] = "";

	while (1) {
		ADCSRA |= _BV(ADSC);
		while(!(ADCSRA & _BV(ADIF)));
		uint16_t adc = ADC;
		
		itoa(adc, adcString, 10);
		strcat(adcString, "\r\n");
		
		send_data(adcString);
		writeLCD(adc);
		
		// If put in a separate function, it doesn't show up
		lcd_gotoxy(0, 0);
		lcd_puts("Measured:");

		_delay_ms(500);
	}
	
	return 0;
}
