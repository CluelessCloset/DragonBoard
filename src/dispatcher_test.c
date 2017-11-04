#include "../src/net.h"
#include "../src/dispatcher.h"
#include <assert.h>
#include "all_tests.h"

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
}