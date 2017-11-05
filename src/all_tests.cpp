#include "all_tests.hpp"
#include "net.hpp"
#include "dispatcher.hpp"
#include "phys.hpp"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define TEST_PHYS_ADDR  0
#define TEST_BAD_ADDR   6

void run_all_tests()
{
  //test_dispatcher();
  test_phys();
}

void test_dispatcher()
{
  packet p1;
  packet p2;
  packet p3;
  packet r;

  p1.packet_type = 1;
  p2.packet_type = 2;
  p3.packet_type = 3;

  dispatcher_init();
  push_job(&p1);

  r = pull_job();
  assert(r.packet_type == p1.packet_type);

  push_job(&p1);
  push_job(&p2);
  push_job(&p3);

  r = pull_job();
  assert(r.packet_type == p1.packet_type);
  r = pull_job();
  assert(r.packet_type == p2.packet_type);
  r = pull_job();
  assert(r.packet_type == p3.packet_type);

  for(int i = 0; i < BUFFER_LEN; i++)
    push_job(&p1);

  r = pull_job();
  assert(r.packet_type == p1.packet_type);

  printf("Dispatcher passed tests\n");
}

void test_phys()
{
  phys_init();
  phys_led_write(HIGH);
  sleep(1);
  phys_led_write(LOW);
  printf("LED should have blinked\n");
  sleep(1);

  //now test the bus interface
  //ping
  bool result;
  result = phys_bus_ping(TEST_PHYS_ADDR);
  assert(result == true);
  result = phys_bus_ping(TEST_BAD_ADDR);
  assert(result == false);

  result = phys_bus_led_on(TEST_PHYS_ADDR);
  assert(result == true);
  sleep(1);
  result = phys_bus_led_off(TEST_PHYS_ADDR);
  assert(result == true);
  sleep(1);
  printf("Arduino Light should have blinked\n");

  result = phys_bus_adc_read(TEST_PHYS_ADDR);
  printf("Here's whether there's clohtes or not: %d", (int) result);

}

void test_net()
{
  // nothin yet :D
}