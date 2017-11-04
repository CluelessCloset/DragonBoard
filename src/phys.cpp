#include "mraa.hpp"
#include "phys.hpp"


class Phys {
  mraa::Gpio led_gpio;
public:
  Phys();
  phys_write_led();
}

Phys() : led_gpio(LED_PIN)
{ 
  led_gpio.dir(mraa::DIR_OUT);
}

void Phys::phys_write_led(int value)
{
  led_gpio.write(value);
}