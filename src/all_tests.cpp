#include "all_tests.hpp"
#include "net.hpp"
#include "dispatcher.hpp"
#include "phys.hpp"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define TEST_PHYS_ADDR  8
#define BAD_PHYS_ADDR   4

void run_all_tests()
{
  //test_dispatcher();
  test_phys();
  //test_net();
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
  phys_write_led(HIGH);
  sleep(1);
  phys_write_led(LOW);
  printf("LED should have blinked\n");
  sleep(1);

  //now test the I2C interface.
  printf("Make sure Arduino is started\n");
  assert(phys_i2c_ping(TEST_PHYS_ADDR) == true);
  assert(phys_i2c_ping(BAD_PHYS_ADDR) == false);
  phys_i2c_write_led(TEST_PHYS_ADDR, 1);
  sleep(1);
  phys_i2c_write_led(TEST_PHYS_ADDR, 0);
  sleep(1);

  for(int i = 0; i <10; i++)
  {
    uint16_t adc = phys_i2c_read_force(TEST_PHYS_ADDR);
    printf("Adc: %f\n", adc);
    assert(adc != ERR);
  }

  printf("Phys interfaces passed tests");

}

void test_net()
{
  std::string addr = "http://cluelesscloset.tech/";
  std::string email = "d@d.com";
  printf("Sending curl to serverwith %s and %s \n", addr.c_str(), email.c_str());
  HangerNet h(addr, email);
  packet p = h.pollServer();

  printf("tried to reach %s with email: %s\n", addr.c_str(), email.c_str());
  printf("Here's your packet type: %d with first data: %d\n", p.packet_type, p.data[0]);

}