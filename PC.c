#define _BSD_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define DefaultCount 100
#define THREADS 2
#define LOOP 1000

typedef struct tArgs
{
  int id;
}tArgs;

// mutex
pthread_mutex_t mutex;

//condition
pthread_cond_t equalNone;

// refill boolean
int needsRefill = 0;

// Number of items left
int count = DefaultCount;

void *consumers(void *info)
{
  tArgs *args = (tArgs *) info;
  int pID = args->id; (void)pID;
  printf("Thread %d has started\n", pID);

  for (int i = 0; i < LOOP; )
  {
    pthread_mutex_lock(&mutex);
    while (count == 0 && !needsRefill)
    {
      printf("count: %d. Requesting a refill\n", count);
      pthread_cond_signal(&equalNone);
      // usleep(900000);
      needsRefill = 1;
      printf("Thank you producer for the refill\n" );
    }

    if (!needsRefill)
    {
      count--;
      i++;
    }
    pthread_mutex_unlock(&mutex);
  }
  printf("Thread %d has terminated\n", pID);
  pthread_exit(NULL);
}

void *producers()
{
  printf("The producer has started\n");
  pthread_mutex_lock(&mutex);
  printf("The producer is WAITING\n");
  pthread_cond_wait(&equalNone, &mutex);
  printf("The Producer has been AWOKEN: %d\n", count);
  while(count == 0 && needsRefill)
  {
    printf("Setting the count back to 10\n");
    count = DefaultCount;
    needsRefill = 0;
    printf("Count is %d\n", count);
    pthread_cond_wait(&equalNone, &mutex);
    printf("Checking to make sure count is positive\n");
  }
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}


int main(void)
{
    // create a thread array for consumers
    pthread_t *consumer = calloc(THREADS, sizeof(pthread_t));
    // create a thread for the producer
    pthread_t *producer = calloc(1, sizeof(pthread_t));
    // create a mutex;
    pthread_mutex_init(&mutex, NULL);
    // create a condition for signalling the producer
    pthread_cond_init(&equalNone, NULL);

    // create an argument array
    tArgs *argument = calloc(THREADS, sizeof(tArgs));

    printf("Creating threads\n");
    // creating the producer thread
    printf("Creating producer\n");
    pthread_create(&producer[0], NULL, producers, NULL);

    for(int i = 0; i < THREADS; i++)
    {
      argument[i].id = i;
      printf("Creating Consumer #%d\n", i);
      pthread_create(&consumer[i], NULL, consumers, &argument[i]);
    }


    // Joining the consumers and producers
    for(int i = 0; i < THREADS; i++)
    {
      pthread_join(consumer[i], NULL);
    }

    // send the signal to the producer so that it finishes
    pthread_mutex_lock(&mutex);
    needsRefill = 0;
    pthread_cond_signal(&equalNone);
    pthread_mutex_unlock(&mutex);
    pthread_join(*producer, NULL);

    // Freeing memory
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&equalNone);
    free(consumer);
    free(argument);
    free(producer);

    printf("The value of count is %d\n", count);
    pthread_exit(NULL);
    return 0;
}
