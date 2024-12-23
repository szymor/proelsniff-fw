#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "main.h"

void esp_power_up(void)
{
	PORTB &= ~_BV(RED_PIN);
	PORTB |= _BV(PDCTRL_PIN);
	// wait for boot
	_delay_ms(500);
	// TODO: wait for an established MQTT connection
	while (PINB & _BV(ESP2_PIN));
}

void esp_power_down(void)
{
	printf("m:close()\r\n");
	// TODO: wait for MQTT disconnection
	while (!(PINB & _BV(ESP2_PIN)));
	PORTB &= ~_BV(PDCTRL_PIN);
	PORTB |= _BV(RED_PIN);
}
