#include "mraa.hpp"
#include "phys.hpp"


static mraa::Gpio led_gpio;

void phys_init()
{
  mraa_init();
  led_gpio = led_gpio(LED_PIN);
  mraa_gpio_dir(led_gpio, MRAA_GPIO_OUT);
}

void phys_write_led(int value)
{
  mraa_gpio_write(led_gpio, value);
}