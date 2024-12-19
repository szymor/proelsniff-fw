# Proelsniff firmware

Proelsniff is a sniffer of flat-related events for Proel&trade; doorphone systems. It is designed to handle Wi-fi connectivity and protocol-specific data parsing.

## Disclaimer

This project is not endorsed by Proel&trade; company.

## ESP8266 connectivity module

The `esp` folder contains:

- **NodeMCU Binary**: A precompiled binary for the NodeMCU firmware.
- **`init.lua`**: A Lua script to be flashed onto the ESP8266. This script configures the ESP8266 as a connectivity module, connecting to a specified MQTT server.

### Archived files

- **`ppfsm.lua`**: Contains a finite state machine implementation for parsing protocol-specific data. This file is currently not in use and is kept for archival purposes.
- **`unittest.lua`**: Contains unit tests for the finite state machine. This file is also archived and not actively used.

## ATmega328PB microcontroller

The `avr` folder contains:

- **C Source Code and Unit Tests**: The `avr` directory contains the firmware source code for the ATmega328PB microcontroller, as well as unit tests for the protocol-specific parser. This parser is used to collect information about events on the bus, such as ringing the doorbell, unlocking the door with a code, or using a keyfob.

## Getting started

To get started with this project, flash the appropriate firmware onto the ESP8266 and ATmega328PB devices. Ensure that the ESP8266 is configured to connect to the correct MQTT server as specified in the `init.lua` script.
