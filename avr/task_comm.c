#include "main.h"
#include "fsm.h"
#include "task_comm.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

uint8_t flat_cbuffer[FLAT_CYCLE_SIZE];
uint8_t flat_cbegin = 0;
uint8_t flat_cend = 0;

uint8_t flat_send_debug = 0;

void task_comm_init(void)
{
	// test run of esp8266, allows to set Wi-fi if not configured
	_delay_ms(1000);
	esp_power_up(0);
	printf("m:publish(topic..\"idle_voltage\", %d, 1, 0)\r\n", ((uint32_t)adc_read() * 12100) >> 10);
	esp_power_down(2000);
}

void task_comm_process(void)
{
	if (flat_send_debug || flat_cbegin != flat_cend)
	{
		if (0 == esp_power_up(20000))
		{
			// clear the error if any
			PORTB |= _BV(RED_PIN);

			while (flat_cbegin != flat_cend)
			{
				printf("m:publish(topic..\"flat\", %d, 1, 0)\r\n", flat_cbuffer[flat_cbegin]);
				cbuffer_next(flat_cbegin, FLAT_CYCLE_MASK);
			}

			if (flat_send_debug)
			{
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
				printf("m:publish(topic..\"debug/last_state\", \"%s\", 1, 0)\r\n", state2string[fsm_get_debug_property(DP_STATE)]);
				printf("m:publish(topic..\"debug/reset\", %lu, 1, 0)\r\n", fsm_get_debug_property(DP_RESET_PERIOD));
				printf("m:publish(topic..\"debug/end\", %lu, 1, 0)\r\n", fsm_get_debug_property(DP_END_PERIOD));
				printf("m:publish(topic..\"debug/flat_low\", %lu, 1, 0)\r\n", fsm_get_debug_property(DP_FLAT_LOW_PERIOD));
				printf("m:publish(topic..\"debug/flat_high\", %lu, 1, 0)\r\n", fsm_get_debug_property(DP_FLAT_HIGH_PERIOD));
			}

			esp_power_down(5000);
		}
		else
		{
			// inform about the error
			PORTB &= ~_BV(RED_PIN);
			// ignore the registered flat number
			cbuffer_next(flat_cbegin, FLAT_CYCLE_MASK);
		}
	}
}
