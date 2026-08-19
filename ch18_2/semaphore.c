#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t sem1, sem2;
static int num;

void *read(void *arg)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        fputs("Input num: ", stdout);
        sem_wait(&sem2);
        scanf("%d", &num);
        sem_port(&sem1);
    }
    return NULL;
}

void *accu(void *arg)
{
    int sum =0, i;
    for(i = 0; i < 5; i++)
    {
        sem_wait(&sem1);
        sum += num;
        sem_post(&sem2);
    }
    printf("Result: %d\n", sum);
    return NULL;
}

int main(int argc, char*argv[])
{
    pthread_t id1, id2;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);

    pthread_create(&id1, NULL, read, NULL);
    pthread_create(&id2, NULL, accu, NULL);

    pthread_join(id1, NULL);
    pthread_join(id2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    return 0;
}