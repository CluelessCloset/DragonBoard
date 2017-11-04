
#ifndef PHYS_H
#define PHYS_H

#include <stdint.h>

//bus values

#define HIGH            1
#define LOW             0

void phys_init();

void phys_led_write(int value);

//interface for calling commands
bool phys_bus_ping(uint8_t address);
bool phys_bus_led_on(uint8_t address);
bool phys_bus_led_off(uint8_t address);
bool phys_bus_adc_read(uint8_t address);


#endif
