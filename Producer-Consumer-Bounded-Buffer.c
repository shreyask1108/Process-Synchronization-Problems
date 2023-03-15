#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define PROD 12     //Number of producers
#define CONS 24     //Number of consumers
#define MAX_BUFFER 12      //Size of Buffer
#define ITEMS_PROD 12           //Max items to be produced or consumed 
#define ITEMS_CONS 6           //Max items to be produced or consumed 

typedef struct
{

  int value;  //Value of Semaphore
  pthread_mutex_t mutex; //mutex variable for exclusive access to value of s as well as for condition variable
  pthread_cond_t c; // condition variable used for block() and wakeup() operation

}Semaphore;

void sem_wait(Semaphore *s)           //Wait function to block process
{
  pthread_mutex_lock(&s->mutex);

  while (s->value == 0)
  {
    pthread_cond_wait(&s->c,&s->mutex);
  }
  (s->value)--;
  pthread_mutex_unlock(&s->mutex);
}

void sem_signal(Semaphore *s)         //Signal function to wakeup process
{
  pthread_mutex_lock(&s->mutex);
  (s->value)++;
  pthread_cond_signal(&s->c);
  pthread_mutex_unlock(&s->mutex);
}

Semaphore empty = {                                                           // Initialisation of semaphore empty
                    .value = MAX_BUFFER,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                  };
Semaphore full = {                                                             // Initialisation of semaphore full
                    .value = 0,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                 };

Semaphore mx =  {                                                             // Initialisation of semaphore full
                  .value = 1,
                  .mutex = PTHREAD_MUTEX_INITIALIZER,
                  .c = PTHREAD_COND_INITIALIZER
                };
int buffer[MAX_BUFFER];
int in = 0;

void *Produce(void * n)                   //Producer thread to produce item
{
  int num;
  for (int i = 0; i < ITEMS_PROD; i++) 
  {
    num = (rand()) % 100;                 //Random item to be produced
    sem_wait(&empty);
    sem_wait(&mx);
    buffer[in] = num;
    printf("Producer %d has produced item %d at buffer number %d\n", ((int * ) n), buffer[in], in);
    in = (in + 1);                             //Mod taken to ensure cyclicity
    sem_signal(&mx);
    sem_signal(&full);
  }
}

void *Consume(void * n)                         //Consumer thread to consume item
{
  for (int i = 0; i < ITEMS_CONS; i++)
  {
    sem_wait(&full);
    sem_wait(&mx);
    int num = buffer[in-1];
    printf("Consumer %d has consumed item %d at buffer number %d\n", ((int * ) n), buffer[in-1], in-1);
    in = (in - 1);                 //Mod taken to ensure cyclicity
    sem_signal(&mx);
    sem_signal(&empty);
  }
}

int main() 
{
  pthread_t producers[PROD];
  pthread_t consumers[CONS];
  
  for (int i = 0; i < CONS; i++)
  {
    pthread_create( & consumers[i], NULL, (void * ) Consume, (i));       //Creating consumer threads
  }

  for (int i = 0; i < PROD; i++)
  {
    pthread_create( & producers[i], NULL, (void * ) Produce, (i));        //Creating producer threads
  }

  for (int i = 0; i < PROD; i++)
  {
    pthread_join(producers[i], NULL);
  }

  for (int i = 0; i < CONS; i++)
  {
    pthread_join(consumers[i], NULL);
  }

  //destroying all the mutex and condition variables
  pthread_mutex_destroy(&mx.mutex);
  pthread_mutex_destroy(&empty.mutex);
  pthread_mutex_destroy(&full.mutex);
  pthread_cond_destroy(&mx.c);
  pthread_cond_destroy(&empty.c);
  pthread_cond_destroy(&full.c);
}