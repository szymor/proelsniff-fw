#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include "fsm.h"
#include "main.h"

union Num
{
	uint32_t v32;
	struct
	{
		uint16_t lsp;
		uint16_t msp;
	};
};

#define CYCLE_SIZE		(256)
#define CYCLE_MASK		(0xff)

#define GREEN_PORT		(PORTB)
#define GREEN_PIN		(0)
#define RED_PORT		(PORTB)
#define RED_PIN			(1)
#define PDCTRL_PORT		(PORTB)
#define PDCTRL_PIN		(2)

// start, mod 8 prescaler (2.304 MHz)
#define timer_start() do { TCCR1B = _BV(CS11); } while (0);
#define timer_stop() do { TCCR1B = 0; } while (0);
#define cycle_forward(x) do { x = (x + 1) & CYCLE_MASK; } while (0);

static volatile uint16_t count = 0;

static volatile union Num ts[CYCLE_SIZE];
static volatile uint8_t cbegin = 0;
static volatile uint8_t cend = 0;

static uint8_t flat_to_send = 0;

ISR(TIMER1_OVF_vect)
{
	++count;
}

ISR(INT0_vect)
{
	ts[cend].lsp = TCNT1;
	TCNT1 = 0;
	ts[cend].msp = count;
	count = 0;
	ts[cend].lsp &= 0xfffe;
	cycle_forward(cend);
}

ISR(INT1_vect)
{
	ts[cend].lsp = TCNT1;
	TCNT1 = 0;
	ts[cend].msp = count;
	count = 0;
	ts[cend].lsp |= 0x0001;
	cycle_forward(cend);
}

void flat_callback(uint8_t flatno)
{
	flat_to_send = flatno;
}

uint8_t get_my_flatno(void)
{
	return (PIND & 0xf0) | ((PINC >> 2) & 0x0f);
}

void esp_power_up(void)
{
	RED_PORT &= ~_BV(RED_PIN);
	PDCTRL_PORT |= _BV(PDCTRL_PIN);
	_delay_ms(5000);
	_delay_ms(5000);
	_delay_ms(5000);
}

void esp_power_down(void)
{
	_delay_ms(5000);
	PDCTRL_PORT &= ~_BV(PDCTRL_PIN);
	RED_PORT |= _BV(RED_PIN);
}

void main(void)
{
	adc_init();
	serial_init();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	// enable interrupts
	sei();

	// enable timer 1 overflow interrupt
	TIMSK1 = _BV(TOIE1);
	// normal timer 1 operation
	TCCR1A = 0x00;
	timer_start();

	// lower 4 flat bits as pull-ups
	PORTC = _BV(PINC2) | _BV(PINC3) | _BV(PINC4) | _BV(PINC5);
	// INT0, INT1 and higher 4 flat bits as pull-ups
	PORTD = _BV(PIND2) | _BV(PIND3) | _BV(PIND4) | _BV(PIND5) | _BV(PIND6) | _BV(PIND7);
	// configure INT0 as falling edge, INT1 as rising edge
	EICRA = _BV(ISC01) | _BV(ISC11) | _BV(ISC10);
	// enable external interrupt
	EIMSK = _BV(INT0) | _BV(INT1);

	fsm_set_cb(flat_callback);
	fsm_reset();

	// pd control (off), green and red leds are output
	PORTB = 0x03;
	DDRB = 0x07;

	while (1)
	{
		// sleep if idle for ~16s
		if (count >= 0x0200)
		{
			GREEN_PORT |= _BV(GREEN_PIN);
			count = 0;
			fsm_reset();
			sleep_mode();
		}

		// green is on if not idle
		GREEN_PORT &= ~_BV(GREEN_PIN);

		// process the cyclic buffer of events
		while (cbegin != cend)
		{
			fsm_push_event(ts[cbegin].lsp & 1, ((ts[cbegin].v32 >> 1) * 889) >> 10);
			cycle_forward(cbegin);
		}

		// send after ~4s of bus inactivity
		if ((flat_to_send || fsm_get_debug_property(DP_STATE) != STATE_IDLE) && (count >= 0x0080))
		{
			esp_power_up();

			if (flat_to_send)
			{
				printf("m:publish(topic..\"flat\", %d, 2, 0)\r\n", flat_to_send);
				flat_to_send = 0;
			}

#ifdef DEBUG
			const char state2string[][8] = {
				"idle",
				"rstart",
				"rend",
				"fstart",
				"fend",
				"rtstart",
				"rtend"
			};
			// publish debug info
			printf("m:publish(topic..\"debug/last_state\", \"%s\", 2, 0)\r\n", state2string[fsm_get_debug_property(DP_STATE)]);
			printf("m:publish(topic..\"debug/reset\", %lu, 2, 0)\r\n", fsm_get_debug_property(DP_RESET_PERIOD));
			printf("m:publish(topic..\"debug/end\", %lu, 2, 0)\r\n", fsm_get_debug_property(DP_END_PERIOD));
			printf("m:publish(topic..\"debug/flat_low\", %lu, 2, 0)\r\n", fsm_get_debug_property(DP_FLAT_LOW_PERIOD));
			printf("m:publish(topic..\"debug/flat_high\", %lu, 2, 0)\r\n", fsm_get_debug_property(DP_FLAT_HIGH_PERIOD));
#endif

			// publish the idle voltage
			printf("m:publish(topic..\"idle_voltage\", %d, 2, 0)\r\n", ((uint32_t)adc_read() * 12100) >> 10);
			// publish the flat number set on jumpers
			printf("m:publish(topic..\"my_flat\", %d, 2, 0)\r\n", get_my_flatno());

			esp_power_down();
		}
	}
}
