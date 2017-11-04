//TODO  TCP connection parameters

#ifndef NET_H
#define NET_H

#define DATA_LEN 252

typedef struct {
  int type,
  char data[DATA_LEN];
} packet;


//queue management functions implemented in dispatcher.c
void push_job(packet* p);
packet pull_job();

#endif //NET_H