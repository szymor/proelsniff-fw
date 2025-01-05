#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "main.h"

uint8_t esp_power_up(int16_t timeout_ms)
{
	// power up
	PORTB |= _BV(PDCTRL_PIN);

	// wait for boot
	_delay_ms(500);

	// TODO: implement cooperative scheduling
	for (uint8_t timeout_disabled = timeout_ms <= 0;
		(timeout_disabled || timeout_ms > 0) && (PINB & _BV(ESP2_PIN));
		timeout_ms -= 100)
	{
		_delay_ms(100);
	}

	if (timeout_ms > 0)
	{
		// esp powered up successfully
		return 0;
	}
	else
	{
		// timeout reached, power down esp forcefully
		PORTB &= ~_BV(PDCTRL_PIN);
		return 1;
	}
}

uint8_t esp_power_down(int16_t timeout_ms)
{
	printf("m:close()\r\n");

	// TODO: implement cooperative scheduling
	for (uint8_t timeout_disabled = timeout_ms <= 0;
		(timeout_disabled || timeout_ms > 0) && !(PINB & _BV(ESP2_PIN));
		timeout_ms -= 100)
	{
		_delay_ms(100);
	}

	// power down anyway
	PORTB &= ~_BV(PDCTRL_PIN);

	if (timeout_ms > 0)
	{
		// esp powered down successfully
		return 0;
	}
	else
	{
		// esp powered down forcefully
		return 1;
	}
}
