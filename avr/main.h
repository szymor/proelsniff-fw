#ifndef _H_MAIN
#define _H_MAIN

#define GREEN_PIN		(0)
#define RED_PIN			(1)
#define PDCTRL_PIN		(2)
#define ESP0_PIN		(3)
#define ESP2_PIN		(4)

void serial_init(void);

void adc_init(void);
uint16_t adc_read(void);

void gpio_init(void);

void esp_power_up(void);
void esp_power_down(void);

#endif
