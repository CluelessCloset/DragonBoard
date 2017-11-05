//TODO  TCP connection parameters
#ifndef NET_H
#define NET_H

#include <string>

//packet stuff
#define DATA_LEN 252

#define JOB_LIT           1
#define JOB_GIVE_STATUS   2
#define JOB_UPDATE        3

typedef struct {
  int packet_type;
  char data[DATA_LEN];
} packet;

//Thread to constantly poll the network interface for new jobs from the server
void * net_loop(void * x);

//queue management functions implemented in dispatcher.c
void push_job(packet* p);
packet pull_job();


class HangerNet
{
public:
  HangerNet(std::string statUrl, std::string authEmail);
  ~HangerNet();

  packet pollServer();
  bool packetValid();
private:


  //use curl to retrieve some info
  std::string response  curl_get_string(std::string url, std::string authEmail, int timeout_ms);

  //parses server response -> packet
  packet parsePacket(std::string inMsg);
  //state variables
  //last packet valid
  bool lastPacketValid;
  //thread is running
  bool running;
  //CURL * for the interface
  auto curl;
  //url to get status messages from
  std::string status_url; 
  std::string email;
}



#endif //NET_H