#include "dispatcher.h"
#include "net.h"
#include "all_tests.h"

#include <pthread>

#define TEST


void main()
{

#ifdef TEST
  run_all_tests();
#else
  dispatcher_init();
  net_init();

  pthread_t d_thread;
  pthread_t n_thread;

  pthread_create(&d_thread, NULL, dispatcher_loop, (void *) 0);
  pthread_create(&n_thread, NULL, net_loop, (void *) 0);

#endif 
}