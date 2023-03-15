#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{

    int value;             // Value of Semaphore
    pthread_mutex_t mutex; // mutex variable for exclusive access to value of s as well as for condition variable
    pthread_cond_t c;      // condition variable used for block() and wakeup() operation

} Semaphore;

void sem_wait(Semaphore *s) // Wait function to block process
{
    pthread_mutex_lock(&s->mutex);

    while (s->value == 0)
    {
        pthread_cond_wait(&s->c, &s->mutex);
    }
    (s->value)--;
    pthread_mutex_unlock(&s->mutex);
}

void sem_signal(Semaphore *s) // Signal function to wakeup process
{
    pthread_mutex_lock(&s->mutex);
    (s->value)++;
    pthread_cond_signal(&s->c);
    pthread_mutex_unlock(&s->mutex);
}

Semaphore empty = { // Initialisation of semaphore
    .value = 1,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

Semaphore full = {
    .value = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .c = PTHREAD_COND_INITIALIZER
};

Semaphore readcount_mutex; // To mutually exclude access of readcount variable
Semaphore shared_resource; // To mutually exclude readers and writers from simultaneously accessing the shared resource
Semaphore fifo_queue;      // FIFO queue of both readers and writers
int readcount;             // Represents number of readers currently reading

void *reader(void *num)
{

    int id = *(int *)num; // Reader number

    // Entry Section

    printf("Reader: %d has entered the queue.\n", id);

    sem_wait(&fifo_queue);

    sem_wait(&readcount_mutex);
    readcount++;
    if (readcount == 1)
        sem_wait(&shared_resource);
    sem_signal(&readcount_mutex);

    sem_signal(&fifo_queue);

    // Critical Section

    printf("Reader: %d has started reading.\n", id);
    sleep(2); // Reading time = 2 seconds.
    printf("Reader: %d has completed reading.\n", id);

    // Exit Section

    sem_wait(&readcount_mutex);
    readcount--;
    if (readcount == 0)
        sem_signal(&shared_resource);
    sem_signal(&readcount_mutex);

    printf("Reader: %d has been dequeued.\n", id);
}

void *writer(void *num)
{

    int id = *(int *)num; // Writer number

    // Entry Section

    printf("Writer: %d has entered the queue.\n", id);

    sem_wait(&fifo_queue);
    sem_wait(&shared_resource);
    sem_signal(&fifo_queue);

    // Critical Section

    printf("Writer: %d has started writing.\n", id);
    sleep(3); // Writing time = 3 seconds
    printf("Writer: %d has completed writing.\n", id);

    // Exit Section

    sem_signal(&shared_resource);

    printf("Writer: %d has been dequeued.\n", id);
}

int main()
{

    int N = 10;       // Total number of readers and writers.
    pthread_t tid[N]; // Declaring threads
    int a[N];

    readcount_mutex = empty; // Initializing all the semaphores
    shared_resource = empty;
    fifo_queue = empty;
    readcount = 0;

    int cur_read = 0;  // Index to be assigned to upcoming reader
    int cur_write = 0; // Index to be assigned to upcoming writer

    srand((unsigned int)time(NULL)); // Assigning random sequence of readers and writers
    for (int i = 0; i < N; i++)
    {
        if (rand() % 3 == 0)
        { // 67% probability of assigning a reader and 33% probability of assgning a writer
            cur_write++;
            a[i] = cur_write;
            pthread_create(&tid[i], NULL, writer, (void *)&a[i]); // Initializing a writer thread
        }
        else
        {
            cur_read++;
            a[i] = cur_read;
            pthread_create(&tid[i], NULL, reader, (void *)&a[i]); // Initialiing a reader thread
        }
        sleep(1); // Each reader/writer arrives 1 second after the previous reader/writer.
    }

    for (int i = 0; i < N; i++)
        pthread_join(tid[i], NULL);

    printf("All threads have been executed successfully.\n");

    return 0;
}