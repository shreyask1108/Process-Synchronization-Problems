#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
int state[N];

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

Semaphore chopstick[N];

void * philosopher(void *);
void eat(int);

void eat(int phil)
{
    state[phil]=EATING;
	printf("\nPhilosopher %d is eating",phil);
}

int main()
{
	int i,a[N];
	pthread_t tid[N];       // creation of threads refering to N philosophers
	
	for(i=0;i<N;i++){    // initialing semaphores
		chopstick[i].value = 1;
		chopstick[i].mutex = PTHREAD_MUTEX_INITIALIZER;
    	chopstick[i].c = PTHREAD_COND_INITIALIZER;   
	} 
		    
		
	for(i=0;i<N;i++){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
		// creation of philosopher and assigning it a number.
	}
	for(i=0;i<N;i++)
		pthread_join(tid[i],NULL);  // waits until a thread gets terminated
}

void * philosopher(void * num)
{
	while (1)
	{
		int phil=*(int *)num;
		state[phil]=THINKING;
		printf("\nPhilosopher %d is hungry",phil);
		state[phil]=HUNGRY;
		sem_wait(&chopstick[phil]);
		sem_wait(&chopstick[(phil+1)%N]);

		eat(phil);

		sleep(2);
		printf("\nPhilosopher %d has finished eating",phil);
		sem_signal(&chopstick[(phil+1)%N]);
		sem_signal(&chopstick[phil]);
		state[phil]=THINKING;
        printf("\nPhilosopher %d is thinking",phil);
        sleep(2);
	}
}