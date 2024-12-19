#include <avr/io.h>
#include <stdio.h>

static int sendc(char c, FILE *f);
static FILE defstream = FDEV_SETUP_STREAM(sendc, NULL, _FDEV_SETUP_RW);

void serial_init(void)
{
#define BAUD_TOL 0
#define BAUD 115200
#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
#if USE_2X
	UCSR0A |= (1 << U2X0);
#else
	UCSR0A &= ~(1 << U2X0);
#endif
#undef BAUD

	stdout = stdin = &defstream;
}

static int sendc(char c, FILE *f)
{
	// wait for empty transmit buffer
	while ( !( UCSR0A & _BV(UDRE0) ) );
	UDR0 = c;
	return 0;
}
