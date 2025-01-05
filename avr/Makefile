.PHONY: all clean flash

TARGET=atmega328pb
PROJECT=main
PROJECT_TEST=test

all: $(PROJECT) $(PROJECT).hex $(PROJECT_TEST)

clean:
	-rm -rf $(PROJECT) $(PROJECT).hex $(PROJECT_TEST)

flash:
	avrdude -c usbasp -p m328pb -U flash:w:main.hex -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0xfc:m

$(PROJECT_TEST): test.c fsm.c
	gcc -g -o $@ $^

$(PROJECT): main.c fsm.c gpio.c serial.c adc.c esp.c task_fsm.c task_comm.c
	avr-gcc -o $@ -mmcu=$(TARGET) -O2 -DF_CPU=18432000UL $^
	avr-size $(PROJECT)

%.hex: %
	avr-objcopy -F elf32-avr $< $@
