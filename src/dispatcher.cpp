#include "net.hpp"
#include "dispatcher.hpp"

#include <stdio>
#include <string>
#include <semaphore>
#include <pthread>
#include <stdbool>

static int empty_index; //next empty spot on the queue
static int full_index; //index of the next job to pull off the queue

static packet job_buffer[BUFFER_LEN]; //ring buffer containing jobs to execute
static sem_t full_spots; //controls producer flow
static sem_t empty_spots; //controls producer flow
static pthread_mutex_t job_mutex;

static bool running;


/*
 * Call once to initiate the job queue
 */
void dispatcher_init()
{
  sem_init(&full_spots, 0, 0); //consumer starts off locked
  sem_init(&empty_spots, 0, BUFFER_LEN); //producer starts off ready to go
  pthread_mutex_init(&job_mutex, NULL);

  empty_index = 0;
  full_index = 0;

  running = true;
}

/*
 * Push a job on the dispatcher queue
 */
void push_job(packet* p)
{
  //wait for the green light
  sem_wait(&empty_spots);

  pthread_mutex_lock(&job_mutex);

  //copy the packet into the next empty slot in the job queue
  memcpy(&job_buffer[empty_index], p, sizeof(packet));
  
  empty_index++;
  if(empty_index == BUFFER_LEN)
    empty_index = 0;
  
  pthread_mutex_unlock(&job_mutex);

  //Increment number of full spots.
  sem_post(&full_spots);
}

/*
 * Pull a job off the dispatcher queue and return it 
 */
packet pull_job()
{
  //wait for a thing to pull
  sem_wait(&full_spots);
  pthread_mutex_lock(&job_mutex);

  packet p = job_buffer[full_index];
  full_index++;
  if(full_index == BUFFER_LEN)
    full_index = 0;

  pthread_mutex_unlock(&job_mutex);

  //increment number of empty spots
  sem_post(&empty_spots);

  return p;
} 
/*
 * Main loop for the job dispatcher
 */
void * dispatcher_loop(void * x)
{
  packet job;

  while(running)
  {
    //blocks until it can pull a job off the queue
    job = pull_job();

    //process the job
    switch(job.packet_type)
    {
      default: //todo, cases
        break;
    }
  }

  return (void *) 0;
}