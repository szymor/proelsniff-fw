#include "main.h"
#include "task_comm.h"
#include <stdio.h>

uint8_t flat_cbuffer[FLAT_CYCLE_SIZE];
uint8_t flat_cbegin = 0;
uint8_t flat_cend = 0;

void task_comm_init(void)
{
}

void task_comm_process(void)
{
	if (flat_cbegin != flat_cend)
	{
		esp_power_up();

		while (flat_cbegin != flat_cend)
		{
			printf("m:publish(topic..\"flat\", %d, 1, 0)\r\n", flat_cbuffer[flat_cbegin]);
			cbuffer_next(flat_cbegin, FLAT_CYCLE_MASK);
		}

		esp_power_down();
	}
}
