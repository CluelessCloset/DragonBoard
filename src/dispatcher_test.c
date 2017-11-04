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

  p1.type = 1;
  p2.type = 2;
  p3.type = 3;

  init_dispatcher();
  push_job(p1);

  r = pull_job();
  assert(r.type == p1.type);

  push_job(p1);
  push_job(p2);
  push_job(p3);

  r = pull_job();
  assert(r.type == p1.type);
  r = pull_job();
  assert(r.type == p2.type);
  r = pull_job();
  assert(r.type == p3.type);

  for(int i = 0; i < BUFFER_LEN; i++)
    push_job(p1);

  r = pull_job();
  assert(r.type == p1.type);
}