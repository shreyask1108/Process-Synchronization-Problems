#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TOBACCO 0
#define PAPER 1
#define MATCHES 2
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

Semaphore empty = {                                                           // Initialisation of semaphore
                    .value = 0,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                  };
Semaphore full = {
                    .value = 0,
                    .mutex = PTHREAD_MUTEX_INITIALIZER,
                    .c = PTHREAD_COND_INITIALIZER
                 };
pthread_mutex_t mx;
Semaphore agentSem, tobaccoSem, paperSem, matchesSem;


typedef struct {
    int ingredient;
    int thread_no;
} ingredients;

void *agent(void *arg)
{
    while (1) {
        sem_wait(&agentSem);
        int rand_num = rand() % 3;
        if (rand_num == 0) {
            printf("Agent put TOBACCO and PAPER on the table.\n");
            sem_signal(&tobaccoSem);
            sem_signal(&paperSem);
        } else if (rand_num == 1) {
            printf("Agent put TOBACCO and MATCHES on the table.\n");
            sem_signal(&tobaccoSem);
            sem_signal(&matchesSem);
        } else {
            printf("Agent put PAPER and MATCHES on the table.\n");
            sem_signal(&paperSem);
            sem_signal(&matchesSem);
        }

        usleep(500000);
    }
}

void *smoker(void *arg)
{   
    ingredients* ingre = (ingredients*) arg; 
    int ingredient = ingre->ingredient;
    int thread_no = ingre->thread_no;
    while (1) {
        if (ingredient == TOBACCO && paperSem.value==1 && matchesSem.value==1 ) {
            sem_wait(&paperSem);
            sem_wait(&matchesSem);
            sem_signal(&agentSem);
            printf("Smoker(havingIngredient = TOBACCO :: id = %d) is smoking\n", thread_no);
        } else if (ingredient == PAPER && tobaccoSem.value==1 && matchesSem.value==1 ) {
            sem_wait(&tobaccoSem);
            sem_wait(&matchesSem);
            sem_signal(&agentSem);
            printf("Smoker(havingIngredient = PAPER :: id = %d) is smoking\n",thread_no);
        } else if(ingredient == MATCHES && paperSem.value==1 && tobaccoSem.value==1 ) {
            sem_wait(&tobaccoSem);
            sem_wait(&paperSem);
            sem_signal(&agentSem);
            printf("Smoker(havingIngredient = MATCHES :: id = %d) is smoking\n", thread_no);
        }
        usleep(500000);
    }
}

int main()
{
    pthread_t agent_thread;
    int N = 50;
    pthread_t smoker_thread_with_tobacco[N];
    pthread_t smoker_thread_with_paper[N];
    pthread_t smoker_thread_with_matches[N];
    ingredients ingredient[3];
    ingredient[0].ingredient = TOBACCO;
    ingredient[1].ingredient = PAPER;
    ingredient[2].ingredient = MATCHES;

    agentSem.value=1;
    tobaccoSem.value=0;
    paperSem.value=0;
    matchesSem.value=0;

    pthread_create(&agent_thread, NULL, agent, NULL);
    for (int i = 0; i < N; i++) {
        ingredient[0].thread_no  = i;
        ingredient[1].thread_no = i;
        ingredient[2].thread_no = i;
        pthread_create(&smoker_thread_with_tobacco[i], NULL, smoker,(void*) &ingredient[0]);
        pthread_create(&smoker_thread_with_paper[i], NULL, smoker,(void*) &ingredient[1]);
        pthread_create(&smoker_thread_with_matches[i], NULL, smoker,(void*) &ingredient[2]);
    }

    pthread_join(agent_thread, NULL);
    for (int i = 0; i < N; i++) {
        pthread_join(smoker_thread_with_tobacco[i], NULL);
        pthread_join(smoker_thread_with_paper[i], NULL);
        pthread_join(smoker_thread_with_matches[i], NULL);
    }


    return 0;
}
