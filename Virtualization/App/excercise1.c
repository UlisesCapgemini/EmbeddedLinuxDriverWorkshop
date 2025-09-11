#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Define pthreads code */
void *producer1_pthread(void *arg)
{
    int thread_id = *((int *)arg);

    pthread_t self = pthread_self();
    printf("[pthread%d] goes to sleep\n", thread_id);
    sleep(10);
    printf("[pthread%d] wakeup and exit..\n", thread_id);

    return NULL;
}

void *producer2_pthread(void *arg)
{
    int thread_id = *((int *)arg);

    pthread_t self = pthread_self();

    printf("[pthread%d] goes to sleep\n", thread_id);
    sleep(1);
    printf("[pthread%d] wakeup and exit..\n", thread_id);

    return NULL;
}

int main()
{
    pthread_t pthreads[2];
    int thread_ids[2] = {1, 2};
    pthread_create(&pthreads[0], NULL, producer1_pthread, &thread_ids[0]);
    printf("Main executed, pthread 1 created\n");

    pthread_create(&pthreads[1], NULL, producer2_pthread, &thread_ids[1]);
    printf("Main executed, pthread 2 created, going to sleep\n");
    /* Hold execution until all pthreads finish, since time is not deterministic on this example */
    pthread_join(pthreads[0], NULL);
    pthread_join(pthreads[1], NULL);
    printf("Main wakeup, exit... \n");
    return 0;
}
