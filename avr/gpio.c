#include "main.h"
#include <avr/io.h>

uint8_t gpio_get_my_flatno(void)
{
	return (PIND & 0xf0) | ((PINC >> 2) & 0x0f);
}

void gpio_init(void)
{
	// lower 4 flat bits as pull-ups
	PORTC = _BV(PINC2) | _BV(PINC3) | _BV(PINC4) | _BV(PINC5);
	// INT0, INT1 and higher 4 flat bits as pull-ups
	PORTD = _BV(PIND2) | _BV(PIND3) | _BV(PIND4) | _BV(PIND5) | _BV(PIND6) | _BV(PIND7);

	// pd control, green and red leds are output
	// esp gpio0 and esp gpio2 are input
	PORTB = _BV(RED_PIN);
	DDRB = _BV(GREEN_PIN) | _BV(RED_PIN) | _BV(PDCTRL_PIN);
}
