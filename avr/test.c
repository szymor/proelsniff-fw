// unit tests for the Proel Protocol Finite State Machine

#include <stdio.h>

#include "fsm.h"

static int expected[16];
static int exnum;
static int cnt;

void check_cb(uint8_t flatno)
{
	if (flatno == expected[cnt])
	{
		printf("  [PASSED %d/%d] Flat %d\n", cnt + 1, exnum, flatno);
	}
	else
	{
		printf("  [FAILED %d/%d] Flat %d, expected %d\n", cnt + 1, exnum, flatno, expected[cnt]);
	}

	++cnt;
}

void run_test(char *title, char *csv_path)
{
	printf(":: TEST %s\n", title);
	fsm_reset();
	fsm_set_cb(check_cb);
	FILE *csv = fopen(csv_path, "r");
	double last_ts = 0;
	while (!feof(csv))
	{
		double timestamp;
		int value;
		if (2 == fscanf(csv, "%lf,%d", &timestamp, &value))
		{
			uint32_t period = (timestamp - last_ts) * 1000000;
			last_ts = timestamp;

			fsm_push_event(value, period);
		}
	}
	fclose(csv);

	printf("  [DEBUG] Reset period: %d\n", fsm_get_debug_property(DP_RESET_PERIOD));
	printf("  [DEBUG] End period: %d\n", fsm_get_debug_property(DP_END_PERIOD));
	printf("  [DEBUG] Flat low period: %d\n", fsm_get_debug_property(DP_FLAT_LOW_PERIOD));
	printf("  [DEBUG] Flat high period: %d\n", fsm_get_debug_property(DP_FLAT_HIGH_PERIOD));
	if (fsm_get_debug_property(DP_STATE) != STATE_IDLE)
	{
		printf("  [WARNING] FSM ended in a non-idle state.\n");
	}

	if (exnum != cnt)
	{
		printf("  [FAILED] %d entrance(s) missing!\n", exnum - cnt);
	}
}

int main(int argc, char *argv[])
{
	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("basic waveform", "testdata/basic.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("double ringbell", "testdata/double_ringbell.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("basic with noise", "testdata/basic_with_noise.csv");

	cnt = 0;
	exnum = 3;
	expected[0] = 44;
	expected[1] = 38;
	expected[2] = 44;
	run_test("three in a row", "testdata/three-in-a-row.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds0", "testdata/ds0.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds1", "testdata/ds1.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds2", "testdata/ds2.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds3", "testdata/ds3.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds4", "testdata/ds4.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 44;
	run_test("ds5", "testdata/ds5.csv");

	cnt = 0;
	exnum = 1;
	expected[0] = 38;
	run_test("ds6", "testdata/ds6.csv");

	cnt = 0;
	exnum = 2;
	expected[0] = 44;
	expected[1] = 44;
	run_test("mix05", "testdata/mix05.csv");

	cnt = 0;
	exnum = 4;
	expected[0] = 44;
	expected[1] = 44;
	expected[2] = 38;
	expected[3] = 44;
	run_test("mix1360", "testdata/mix1360.csv");

	cnt = 0;
	exnum = 4;
	expected[0] = 44;
	expected[1] = 44;
	expected[2] = 44;
	expected[3] = 44;
	run_test("mix2525", "testdata/mix2525.csv");

	return 0;
}
