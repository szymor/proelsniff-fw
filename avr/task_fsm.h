#ifndef _H_TASK_FSM
#define _H_TASK_FSM

#define FSM_CYCLE_SIZE		(256)
#define FSM_CYCLE_MASK		(0xff)

union Num
{
	uint32_t v32;
	struct
	{
		uint16_t lsp;
		uint16_t msp;
	};
};

extern volatile union Num fsm_cbuffer[FSM_CYCLE_SIZE];
extern volatile uint8_t fsm_cbegin;
extern volatile uint8_t fsm_cend;

extern volatile uint16_t t1ovf_count;

void task_fsm_init(void);
void task_fsm_process(void);

#endif
