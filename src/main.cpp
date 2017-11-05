#include "dispatcher.hpp"
#include "net.hpp"
#include "all_tests.hpp"

#include <pthread.h>

//#define TEST


int main()
{

#ifdef TEST
  run_all_tests();
#else
  dispatcher_init();

  pthread_t d_thread;
  pthread_t n_thread;

  pthread_create(&d_thread, NULL, dispatcher_loop, (void *) 0);
  pthread_create(&n_thread, NULL, net_loop, (void *) 0);

  pthread_join(d_thread, NULL);
  pthread_join(n_thread, NULL);
#endif 
  return 0;
}
