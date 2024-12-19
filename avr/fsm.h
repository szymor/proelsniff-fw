#ifndef _H_FSM
#define _H_FSM

#include <stdint.h>

enum State
{
	STATE_IDLE,
	STATE_RESET_START,
	STATE_RESET_END,
	STATE_FLAT_START,
	STATE_FLAT_END,
	STATE_RINGTONE_START,
	STATE_RINGTONE,
	STATE_END
};

enum DebugProperty
{
	DP_STATE,
	DP_RESET_PERIOD,
	DP_END_PERIOD,
	DP_FLAT_LOW_PERIOD,
	DP_FLAT_HIGH_PERIOD
};

typedef void (*fsm_cb)(uint8_t flatno);

void fsm_reset(void);
uint32_t fsm_get_debug_property(enum DebugProperty dp);
void fsm_set_cb(fsm_cb callback);
void fsm_push_event(uint8_t rising, uint32_t period);

#endif
