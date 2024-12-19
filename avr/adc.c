#include <avr/io.h>
#include <stdio.h>

void adc_init(void)
{
	// internal 1.1V bandgap reference, left-adjusted, channel 0
	ADMUX = _BV(REFS1) | _BV(REFS0);
	// disable digital input for channel 0
	DIDR0 = _BV(ADC0D);
	// enable ADC, prescaler = 128
	//PRR0 = _BV(PRADC);
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

uint16_t adc_read(void)
{
	// start conversion
	ADCSRA |= _BV(ADSC);
	// wait...
	while (!(ADCSRA & _BV(ADIF)));
	// clear interrupt flag
	ADCSRA |= ADIF;

	return ADCW;
}
