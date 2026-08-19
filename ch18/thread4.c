#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD 100
long long num = 0;
void* thread_inc(void* arg)
{
    int i;
    for(i = 0; i < 5000; i++)
        num+=1;
    return NULL;
}

void* thread_des(void*arg)
{
    int i;
    for(i = 0; i <5000; i++)
        num-=1;
    return NULL;
}

void* thread_multiply(void* arg)
{
    int i;
    for(i = 0; i < 5000; i++)
    {
        if(i % 3 == 0)
            num -= 1;
        else if(i % 3 == 1)
            num += 1;
        else
            num *= 1;
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t thread_id[NUM_THREAD];
    int i;
    printf("sizeof longlong: %ld\n", sizeof(long long));
    for (i = 0; i < NUM_THREAD; i++)
    {
        if(i%3 == 0)
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
        else if(i % 3 == 1)
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
        else
            pthread_create(&(thread_id[i]), NULL, thread_multiply, NULL);
    }
        for(i = 0; i <NUM_THREAD; i++)
            pthread_join(thread_id[i], NULL);

        printf("result: %lld\n", num);
        return 0;
}