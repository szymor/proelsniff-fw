#include "main.h"
#include "task_fsm.h"
#include "fsm.h"
#include "task_comm.h"

volatile union Num fsm_cbuffer[FSM_CYCLE_SIZE];
volatile uint8_t fsm_cbegin = 0;
volatile uint8_t fsm_cend = 0;
volatile uint16_t t1ovf_count = 0;

static void flat_callback(uint8_t flatno)
{
	flat_cbuffer[flat_cend] = flatno;
	cbuffer_next(flat_cend, FLAT_CYCLE_MASK);
}

void task_fsm_init(void)
{
	fsm_set_cb(flat_callback);
	fsm_reset();
}

void task_fsm_process(void)
{
	// process the cyclic buffer containing rising/falling edge timestamps
	while (fsm_cbegin != fsm_cend)
	{
		fsm_push_event(fsm_cbuffer[fsm_cbegin].lsp & 1, ((fsm_cbuffer[fsm_cbegin].v32 >> 1) * 889) >> 10);
		cbuffer_next(fsm_cbegin, FSM_CYCLE_MASK);
	}
}
