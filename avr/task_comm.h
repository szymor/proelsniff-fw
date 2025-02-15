#ifndef _H_TASK_COMM
#define _H_TASK_COMM

#define FLAT_CYCLE_SIZE		(16)
#define FLAT_CYCLE_MASK		(0x0f)

#define FLAT_CYCLE_EMPTY	(flat_cbegin == flat_cend)

extern uint8_t flat_cbuffer[FLAT_CYCLE_SIZE];
extern uint8_t flat_cbegin;
extern uint8_t flat_cend;

extern uint8_t flat_send_debug;

void task_comm_init(void);
void task_comm_process(void);

#endif
