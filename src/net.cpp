#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"

static bool running;
void net_init()
{

}

//Constantly poll the network interface for new jobs from the server
void * net_loop(void * x)
{
  while(running)
  {
    //nothing
  }

  return (void *) 0;
}