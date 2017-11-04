//TODO  TCP connection parameters

#ifndef NET_H
#define NET_H

#define DATA_LEN 252

typedef struct {
  int packet_type;
  char data[DATA_LEN];
} packet;


//queue management functions implemented in dispatcher.c
void push_job(packet* p);
packet pull_job();


void net_init();
//Constantly poll the network interface for new jobs from the server
void * net_loop(void * x);

#endif //NET_H