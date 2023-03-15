#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define BARB 4   //cons... number of barber threads
#define CUST 8 // prod... number of customer threads
#define MAX 15    //max number of chairs
#define MAX2  2     //max number of customers served per barber

typedef struct
{

  int value;  //Value of Semaphore
  pthread_mutex_t mutex; //mutex variable for exclusive access to value of s as well as for condition variable
  pthread_cond_t c; // condition variable used for block() and wakeup() operation

}Semaphore;

void sem_wait(Semaphore *s)   //Wait function to block process
{
  pthread_mutex_lock(&s->mutex);

  while (s->value == 0)
  {
    pthread_cond_wait(&s->c,&s->mutex);
  }
  (s->value)--;
  pthread_mutex_unlock(&s->mutex);
}

void sem_signal(Semaphore *s)   //Signal function to wakeup process
{
  pthread_mutex_lock(&s->mutex);
  (s->value)++;
  pthread_cond_signal(&s->c);
  pthread_mutex_unlock(&s->mutex);
}

Semaphore barbers = {                                                           // Initialisation of semaphore
                    .value = BARB,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                  };
Semaphore customers = {
                    .value = 0,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                 };
Semaphore mutex1 = {
                    .value = 1,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                 };


int customer_count=0;    //counter 

int chairs[MAX];           //stores the customer id sitting on the ith chair
int in = 0;             //next empty seat where the customer sits
int out = 0;            //next seat served by the barbers



void *barber (void *n)
{
        for(int i=0;i<MAX2;i++)
        {
            sem_wait(&customers);
            sem_wait(&mutex1);
            int num = chairs[out];
            customer_count--;
            printf("Barber %d wakes up and serves customer %d sitting on chair %d . Customers waiting: %d.\n",((int *)n),num,out,customer_count);
            out=(out+1)%MAX;
            sem_signal(&mutex1);
            printf("Barber %d sleeps \n",((int *)n));
            sem_signal(&barbers); 
        }
}

void *customer(void *n)
{
        sem_wait(&mutex1);
        if(customer_count<MAX)
        {
            int num = ((int *)n);
            customer_count++;
            chairs[in]=num;
            printf("Customer %d arrives and sits in chair %d. Customers waiting: %d.\n",((int *)n),in,customer_count);
            in =( in+1)%MAX; 
            sem_signal(&customers);
            sem_signal(&mutex1);
            sem_wait(&barbers);
        }
        else
        {
            printf("Customer %d leaves:no empty chairs . Customer count is %d.\n",((int *)n),customer_count);
            sem_signal(&mutex1);
        }
}


int main(){
    pthread_t barbers1[BARB];
    pthread_t customers1[CUST];
        
   

   for (int i = 0; i < BARB; i++)
  {
    pthread_create( & barbers1[i], NULL, (void * ) barber, (i));       //Creating consumer threads
  }
    for (int i = 0; i < CUST; i++)
  {
    pthread_create( & customers1[i], NULL, (void * ) customer, (i));        //Creating producer threads
  }

  for (int i = 0; i < CUST; i++)
  {
    pthread_join(customers1[i], NULL);
  }
  for (int i = 0; i < BARB; i++)
  {
    pthread_join(barbers1[i], NULL);
  }


    pthread_mutex_destroy(&mutex1.mutex);
    pthread_mutex_destroy(&mutex1.c);
    pthread_mutex_destroy(&customers.mutex);
    pthread_mutex_destroy(&barbers.mutex);
    pthread_cond_destroy(&customers.c);
    pthread_cond_destroy(&barbers.c);
    

}


