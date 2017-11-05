#include "net.hpp"

#include <curl/curl.h>
#include <string>
#include <cstdlib>

//Constantly poll the network interface for new jobs from the server
void * net_loop(void * x)
{
  HangerNet h("http://cluelesscloset.tech/", "d@d.com");
  
  while(h.running)
  {
    packet p = h.pollServer();

    if(h.isPacketValid())
    {
      push_job(&p);
      //respond with some kind of status?
      //threadSafeGetStatus()...
      //h.sendStatus()
    }

  }

  return (void *) 0;
}

HangerNet::HangerNet(std::string statUrl, std::string authEmail)
{
  running = true;
  status_url = statUrl;
  email = authEmail;
  curl = curl_easy_init();
}

HangerNet::~HangerNet()
{
  curl_easy_cleanup(curl);
}

/*
 * Callback to write the received data to a string (data)
 */
size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

//receive callback to write data
//use curl to get a RESTFUL GET function from the webap
std::string HangerNet::curl_get_string(std::string url, std::string authEmail, int timeout_ms)
{
    if (curl) {
        //set opts
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.42.0");
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
        curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(curl, CURLOPT_EXPECT_100_TIMEOUT_MS, timeout_ms);
        
        std::string response_string;
        std::string header_string; //ignore
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string); //ignore
        
        //get response for debugging
        char* url;
        long response_code;
        double elapsed;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code); //HTTP response code
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed); //just shows total time
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url); //shows last used url (in case of redirects)
        
        //makes the actual transaction
        curl_easy_perform(curl);

        //debugski
        printf("HTTP response: %d\n", response_code);
        printf("time elapsed: %d\n", elapsed);
        printf("Url visited: %s\n", url);
        printf("Header string %s\n", header_string);
        printf("Response string: %s\n", response_string);
        return response_string;
    } else {
      printf("Curl not initialized before trying to call curl_get_string. Re-init-ing");
      curl = curl_easy_init();
      return ""; //empty string means nothing useful. 
    }
}

/* 
 * Uses curl to poll the server
 * If it timesout, return nothing
 * if it gets a legit message, return that
 */

packet HangerNet::pollServer()
{
  lastPacketValid = false;

  std::string response = curl_get_string(status_url, email, 500);
  if(response.empty())
  {
    packet p;
    return p;
  }
  else
  {
    lastPacketValid = true;
    return parsePacket(response);
  }
}

bool HangerNet::isPacketValid()
{
  return lastPacketValid;
}

//server response in, packet out
//given a message like this---"{"hanger" : 5}"
  //just turns into a ping message
packet HangerNet::parsePacket(std::string inMsg)
{
  //top notch parser incomig ;)

  //yoink all of the digits out of inMsg and make them into a string.
  std::string numYoink = "";
  for(unsigned int i = 0; i < inMsg.length(); i++)
  {
      char c = inMsg[i];
      if( c >= '0' && c <= '9')
      {
        numYoink += c;
      }
  }
  int num = strtol(numYoink.c_str(), 0, 10);

  //make a packet out of the results;
  packet p;
  p.packet_type = JOB_LIT;
  p.data[0] = num;

  return p;
}