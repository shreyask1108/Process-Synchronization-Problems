#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>

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

Semaphore Barrier = {                          // Initialisation of semaphore
    .value = 4,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

Semaphore hackerQueue = {
    .value = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

Semaphore serfQueue = {
    .value = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

Semaphore mutex = {
    .value = 1,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

int hackers=0,serfs=0;

void * Hacker(void *);
void * Serf(void *);

int main(){
    int a[4];
    pthread_t pid[8];
    for(int i=0;i<4;i++){
        a[i]=i;
        pthread_create(&pid[i],NULL,Hacker,(void *)&a[i]);
        pthread_create(&pid[i+4],NULL,Serf,(void *)&a[i]);
    }
    for(int i=0;i<4;i++){
        pthread_join(pid[i],NULL);
        pthread_join(pid[i+4],NULL);
    }
}

void * Hacker(void * num){
    while (1){
        int hacker =*(int *)num;
        bool isCaptain =false;
        sem_wait(&mutex);
        hackers++;
        if(hackers==4){
            sem_signal(&hackerQueue); sem_signal(&hackerQueue); sem_signal(&hackerQueue); sem_signal(&hackerQueue);
            hackers-=4;
            isCaptain=true;
        }
        else if(hackers==2&&serfs>=2){
            sem_signal(&hackerQueue); sem_signal(&hackerQueue); sem_signal(&serfQueue); sem_signal(&serfQueue);
            serfs-=2;
            hackers-=2;
            isCaptain=true;
        }
        else{
            sem_signal(&mutex);
        }
        sem_wait(&hackerQueue);
        sem_wait(&Barrier);
        printf("hacker %d boarding\n",hacker);

        if(isCaptain){
            sleep(1);
            printf("hacker %d has started rowing\n",hacker);
            sem_signal(&mutex);
        }
        sem_signal(&Barrier);
    }
}

void * Serf(void * num){
    while (1)
    {
        int serf =*(int *)num;
        bool isCaptain =false;
        sem_wait(&mutex);
        serfs++;
        if(serfs==4){
            sem_signal(&serfQueue); sem_signal(&serfQueue); sem_signal(&serfQueue); sem_signal(&serfQueue);
            serfs-=4;
            isCaptain=true;
        }
        else if(serfs==2&&hackers>=2){
            sem_signal(&serfQueue); sem_signal(&serfQueue); sem_signal(&hackerQueue); sem_signal(&hackerQueue);
            serfs-=2;
            hackers-=2;
            isCaptain=true;
        }
        else{
            sem_signal(&mutex);
        }
        sem_wait(&serfQueue);
        sem_wait(&Barrier);
        printf("serf %d boarding\n",serf);
        if(isCaptain){
            sleep(1);
            printf("serf %d has started rowing\n",serf);
            sem_signal(&mutex);
        }
        sem_signal(&Barrier);
    }
}