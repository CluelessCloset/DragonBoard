#include "mraa.h"
#include "phys.h"


mraa_gpio_context led_gpio;

void phys_init()
{
  mraa_init();
  led_gpio = mraa_gpio_init(LED_PIN);
  mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT);
}

void phys_write_led(int pin)
{
  mraa_gpio_write(led_gpio, value);
}