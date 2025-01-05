#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "fsm.h"
#include "task_fsm.h"
#include "task_comm.h"

// start, mod 8 prescaler (2.304 MHz)
#define timer_start() do { TCCR1B = _BV(CS11); } while (0);
#define timer_stop() do { TCCR1B = 0; } while (0);

void main(void)
{
	gpio_init();
	adc_init();
	serial_init();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	task_fsm_init();
	task_comm_init();

	// enable interrupts
	sei();

	// enable timer 1 overflow interrupt
	TIMSK1 = _BV(TOIE1);
	// normal timer 1 operation
	TCCR1A = 0x00;
	timer_start();

	// configure INT0 as falling edge, INT1 as rising edge
	EICRA = _BV(ISC01) | _BV(ISC11) | _BV(ISC10);
	// enable external interrupt
	EIMSK = _BV(INT0) | _BV(INT1);

	while (1)
	{
		if (!flat_send_debug &&
			fsm_get_debug_property(DP_STATE) != STATE_IDLE &&
			t1ovf_count >= 0x0080 /* ~4 sec */)
		{
			flat_send_debug = 1;
		}

		if (FSM_CYCLE_EMPTY && FLAT_CYCLE_EMPTY && !flat_send_debug && t1ovf_count >= 0x0180 /* ~12 sec */)
		{
			// green off
			PORTB |= _BV(GREEN_PIN);

			t1ovf_count = 0;
			fsm_reset();
			sleep_mode();
		}

		// green is on if not idle
		PORTB &= ~_BV(GREEN_PIN);

		task_fsm_process();
		task_comm_process();
	}
}

// interrupt service routines used for FSM

ISR(TIMER1_OVF_vect)
{
	++t1ovf_count;
}

ISR(INT0_vect)
{
	fsm_cbuffer[fsm_cend].lsp = TCNT1;
	TCNT1 = 0;
	fsm_cbuffer[fsm_cend].msp = t1ovf_count;
	t1ovf_count = 0;
	fsm_cbuffer[fsm_cend].lsp &= 0xfffe;
	cbuffer_next(fsm_cend, FSM_CYCLE_MASK);
}

ISR(INT1_vect)
{
	fsm_cbuffer[fsm_cend].lsp = TCNT1;
	TCNT1 = 0;
	fsm_cbuffer[fsm_cend].msp = t1ovf_count;
	t1ovf_count = 0;
	fsm_cbuffer[fsm_cend].lsp |= 0x0001;
	cbuffer_next(fsm_cend, FSM_CYCLE_MASK);
}
