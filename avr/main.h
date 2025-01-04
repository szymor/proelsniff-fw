#ifndef _H_MAIN
#define _H_MAIN

#include <stdint.h>

#define GREEN_PIN		(0)
#define RED_PIN			(1)
#define PDCTRL_PIN		(2)
#define ESP0_PIN		(3)
#define ESP2_PIN		(4)

#define cbuffer_next(x, mask) do { x = (x + 1) & (mask); } while (0);

void serial_init(void);

void adc_init(void);
uint16_t adc_read(void);

void gpio_init(void);
uint8_t gpio_get_my_flatno(void);

void esp_power_up(void);
void esp_power_down(void);

void task_fsm(void);
void task_flat(void);

#endif
