
#ifndef PHYS_H
#define PHYS_H

#include <stdint.h>

#define LED_PIN         23
#define HIGH            1
#define LOW             0
#define I2C_BUFFER_MAX  10
#define I2C_BUS         0 


//function constants
#define PING            0x00
#define LED_ON          0x01
#define LED_OFF         0x02
#define ADC_READ        0x03

#define ACK             0xEE
#define ERR             0xAA

//ADC constants
#define ADC_THRESH      30 
void phys_init();

void phys_write_led(int value);


uint16_t phys_i2c_read_force(uint8_t address);
void phys_i2c_write_led(uint8_t address, int value);
bool phys_i2c_ping(uint8_t address);


#endif
