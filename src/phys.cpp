#include "mraa.h"
#include "phys.hpp"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

//pins 
#define PIN_CS0         23
#define PIN_CS1         24
#define PIN_CS2         25

#define PIN_CMD0        26
#define PIN_CMD1        27

#define PIN_PRESENT     31 //pin 28 is reserved.... hence the weird order
#define PIN_ACK         29

#define PIN_CONTROL     30

#define PIN_LED         32

//function constants
#define CMD_PING            0x00
#define CMD_LED_ON          0x01
#define CMD_LED_OFF         0x02
#define CMD_ADC_READ        0x03

//gpio contetxs
static mraa_gpio_context context_cs0;
static mraa_gpio_context context_cs1;
static mraa_gpio_context context_cs2;

static mraa_gpio_context context_cmd0;
static mraa_gpio_context context_cmd1;

static mraa_gpio_context context_present;
static mraa_gpio_context context_ack;

static mraa_gpio_context context_control;

static mraa_gpio_context context_led;

//helper funciton
int phys_bus_call(uint8_t address, int cmd);

//inits all gpio pins
//direction never changes
void phys_init()
{
  //init pins in order
  mraa_init();

  //cs pins
  context_cs0 = mraa_gpio_init(PIN_CS0);
  mraa_gpio_dir(context_cs0, MRAA_GPIO_OUT);
  mraa_gpio_write(context_cs0, LOW);
  context_cs1 = mraa_gpio_init(PIN_CS1);
  mraa_gpio_dir(context_cs1, MRAA_GPIO_OUT);
  mraa_gpio_write(context_cs1, LOW);
  context_cs2 = mraa_gpio_init(PIN_CS2);
  mraa_gpio_dir(context_cs2, MRAA_GPIO_OUT);
  mraa_gpio_write(context_cs2, LOW);

  //cmd pins
  context_cmd0 = mraa_gpio_init(PIN_CMD0);
  mraa_gpio_dir(context_cmd0, MRAA_GPIO_OUT);
  mraa_gpio_write(context_cmd0, LOW);
  context_cmd1 = mraa_gpio_init(PIN_CMD1);
  mraa_gpio_dir(context_cmd1, MRAA_GPIO_OUT);
  mraa_gpio_write(context_cmd1, LOW);
  
  //slave write
  context_present = mraa_gpio_init(PIN_PRESENT);
  mraa_gpio_dir(context_present, MRAA_GPIO_IN ); //default low
  context_ack = mraa_gpio_init(PIN_ACK);
  mraa_gpio_dir(context_ack, MRAA_GPIO_IN );

  //bus control
  context_control = mraa_gpio_init(PIN_CONTROL);
  mraa_gpio_dir(context_control, MRAA_GPIO_OUT);
  mraa_gpio_write(context_control, LOW);

  //led
  context_led = mraa_gpio_init(PIN_LED);
  mraa_gpio_dir(context_led, MRAA_GPIO_OUT);
  mraa_gpio_write(context_led, LOW);
}

/*
 * write value to the led pin
 */
void phys_led_write(int value)
{
  mraa_gpio_write(context_ack, value);
}

//write a normal command:
//PING, LED_ON or LED_OFF
//returns a 2 bit number (PRESENT)(ACK) on success
//-1 on fail
int phys_bus_call(uint8_t address, int cmd)
{
  mraa_result_t r = MRAA_SUCCESS;
  //sanity check inputs
  if(address > 7)
  {
    printf("Invalid address: %d\n", address);
    return -1;
  }

  if(cmd > 3)
  {
    printf("Invalid command: %d\n", cmd);
    return -1;
  }
  //phase 1 of bus
  //write cs and cmd to bus
  printf("Writing CS  %d and CMD %d to bus\n", address, cmd);
  mraa_gpio_write(context_cs0, address & 1);
  mraa_gpio_write(context_cs1, (address >> 1) & 1);
  mraa_gpio_write(context_cs2, (address >> 2) & 1);

  mraa_gpio_write(context_cmd0, cmd & 1);
  mraa_gpio_write(context_cmd1, (cmd >> 1)& 1);

  //feather the bus edge and wait 

  mraa_gpio_write(context_control, HIGH);
  usleep(100*1000); //delay 10 ms
  mraa_gpio_write(context_control, LOW);
  usleep(100*1000); //delay 10 ms

  printf("Flipped bus. Reading ACK and present\n");
  //arduino reads and reacts
  //phase 2 of bus
  mraa_gpio_write(context_control, HIGH);
  //read in ACK and present
  int read_ack = mraa_gpio_read(context_ack);
  int read_pres = mraa_gpio_read(context_present);

  usleep(100*1000); //delay 10 ms
  mraa_gpio_write(context_control, LOW);
  usleep(100*1000); //delay 10 ms

  printf("Bus done: ACK %d and present %d\n", read_ack, read_pres);
  //bus done.

  //process the output
  if(read_ack < 0 || read_pres <0)
  {
    printf("Fatal error in reading the bus\n");
    return -1;
  }

  //hAcKs 2bItS iNtO oNe InTeGeR
  int ret = 0;
  ret += read_ack & 1;
  ret += (read_pres & 1) << 1;

  printf("Bus done: final return %d\n", ret);
  return ret;
}

//interface for calling commands
bool phys_bus_ping(uint8_t address)
{
  int r = phys_bus_call(address, CMD_PING);
  if(r < 0)
    return false;

  //return true if the slave ACK'd 
  if(r & 1)
    return true;
  return false;
}

bool phys_bus_led_on(uint8_t address)
{
  int r = phys_bus_call(address, CMD_LED_ON);
  if(r < 0)
    return false;

  //return true if the slave ACK'd 
  if(r & 1)
    return true;
  return false;
}
bool phys_bus_led_off(uint8_t address)
{
  int r = phys_bus_call(address, CMD_LED_OFF);
  if(r < 0)
    return false;

  //return true if the slave ACK'd 
  if(r & 1)
    return true;
  return false;
}

bool phys_bus_adc_read(uint8_t address)
{
  int r = phys_bus_call(address, CMD_ADC_READ);
  if(r < 0)
    return false;

  //return true if the slave asserted PRESENT
  if((r >> 1) & 1)
    return true;
  return false;
}

