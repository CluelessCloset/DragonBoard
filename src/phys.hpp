
#ifndef PHYS_H
#define PHYS_H

#include <stdint.h>

//pins 
#define PIN_CS0         23
#define PIN_CS1         24
#define PIN_CS2         25

#define PIN_CMD0        26
#define PIN_CMD1        27

#define PIN_PRESENT     31 //pin 28 is reserved.
#define PIN_ACK         29

#define PIN_CONTROL     30

#define PIN_LED         32


#define HIGH            1
#define LOW             0


//function constants
#define PING            0x00
#define LED_ON          0x01
#define LED_OFF         0x02
#define ADC_READ        0x03

void phys_init();

void phys_led_write(int value);

bool phys_bus_ping(uint8_t address);
bool phys_bus_led_on(uint8_t address);
bool phys_bus_led_off(uint8_t address);
bool phys_bus_adc_read(uint8_t address);


#endif
