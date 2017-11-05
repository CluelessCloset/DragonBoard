#include "mraa.h"
#include "phys.hpp"
#include <stdint.h>

static mraa_gpio_context led_gpio;
static mraa_i2c_context i2c0;

static uint8_t rx_tx_buffer[I2C_BUFFER_MAX];


//local functions
mraa_result_t phys_i2c_write(uint8_t address, uint8_t data);
uint8_t phys_i2c_read_1(uint8_t address);
uint16_t phys_i2c_read_2(uint8_t address);


void phys_init()
{
  //init LED
  mraa_init();
  led_gpio = mraa_gpio_init(LED_PIN);
  mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT);

  //init i2C bus
  i2c0 = mraa_i2c_init(I2C_BUS);
  printf("i2c bus + led interface initialized");
}

//write to the red LED on the dragon
void phys_write_led(int value)
{
  mraa_gpio_write(led_gpio, value);
}


//app-level functions
//probe the address for its force value and return it
uint16_t phys_i2c_read_force(uint8_t address)
{
  phys_i2c_write(address, ADC_READ);
  return phys_i2c_read_2(address);
}

//write  a value to the device LED at address
void phys_i2c_write_led(uint8_t address, int value)
{
  int cmd;
  if(value >= 1)
    cmd = LED_ON;
  else
    cmd = LED_OFF;

  phys_i2c_write(address, cmd);
}

//ping the device at address.
//true if device is there... false otherwise
bool phys_i2c_ping(uint8_t address)
{
  mraa_result_t r;
  uint8_t rd;
  r = phys_i2c_write(address, PING);
  if(r != MRAA_SUCCESS)
    return false;
  rd = phys_i2c_read_1(address);

  if(rd == ACK)
    return true;

  return false;
}

//Local functions below
//write 1 bytes to the bus
mraa_result_t phys_i2c_write(uint8_t address, uint8_t data)
{
  mraa_i2c_address(i2c0, address);
  rx_tx_buffer[0] =  data;
  return mraa_i2c_write(i2c0, rx_tx_buffer, 1);
}

//read 1 byte from the bus
uint8_t phys_i2c_read_1(uint8_t address)
{
  mraa_i2c_address(i2c0, address);
  return (uint8_t) mraa_i2c_read_byte(i2c0);
}

//read 2 bytes from the bus (MSB first)
uint16_t phys_i2c_read_2(uint8_t address)
{
  int r;
  uint16_t result;
  printf("Reading 2 bytes: ");
  mraa_i2c_address(i2c0, address);
  r = mraa_i2c_read(i2c0, rx_tx_buffer, 2);
  if(r == 2)
    result = (rx_tx_buffer[0] << 8) + rx_tx_buffer[1];
  else
    result = ERR; //make this more visible

  printf("Results = %d\n", result);
  return result;
}