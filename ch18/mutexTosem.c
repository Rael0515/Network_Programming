#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREAD 100

long long num = 0;
sem_t sem;

void *thread_inc(void *arg)
{
    int i;
    sem_wait(&sem);
    for(i =0; i < 5000; i++)
        num +=1;
    sem_post(&sem);
    return NULL;
}

void *thread_des(void *arg)
{
    int i;
    for(i = 0; i < 5000; i++)
    {
        sem_wait(&sem);
	//pthread_mutex_lock(&mutex);
        num-=1;
        //pthread_mutex_unlock(&mutex);
	sem_post(&sem);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;

    sem_init(&sem, 0, 1);
    //pthread_mutex_init(&mutex, NULL);

    for(i = 0; i < NUM_THREAD; i++)
    {
        if(i%2)
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
    }

    for(i = 0; i < NUM_THREAD; i++)
        pthread_join(thread_id[i], NULL);
    
    printf("result: %lld\n", num);
    sem_destroy(&sem);
    //pthread_mutex_destroy(&mutex);
    return 0;
}
