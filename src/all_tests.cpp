#include "all_tests.hpp"
#include "net.hpp"
#include "dispatcher.hpp"
#include "phys.hpp"

#include <cassert>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>


void run_all_tests()
{
  test_dispatcher();
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
  phys_write_led(HIGH);
  sleep(1);
  phys_write_led(LOW);
  printf("LED should have blinked\n");
}

void test_net();