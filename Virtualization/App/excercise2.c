#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 10U
#define ALLOWED_PRODUCERS 2U
#define ALLOWED_CONSUMERS 4U
#define NUMBER_OF_THREADS (ALLOWED_PRODUCERS + ALLOWED_CONSUMERS)
#define ITEMS_PER_PRODUCER 6U

/* shared resource */
int buffer[BUFFER_SIZE];
int buffer_index = 0U;

/* Primitives for synchronization */
pthread_mutex_t buffer_mutex;
sem_t empty_slots;
sem_t slots_available;

void *producer(void *arg)
{
    int producer_id = *((int *)arg);
    int *result = malloc(sizeof(int));
    int item = 0;
    /* Produce without saving */
    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
    {
        /* Generate item ie. 100, 200 */
        item = (producer_id * 100) + i;

        printf("[producer %d] waiting for empty\n", producer_id);
        sem_wait(&empty_slots);

        /* write produced data, since other producer thread is running lock the buffer */
        pthread_mutex_lock(&buffer_mutex);
        buffer[buffer_index] = item;
        printf("[producer %d] written buffer [%d] = %d\n", producer_id, buffer_index, item);
        buffer_index++;
        pthread_mutex_unlock(&buffer_mutex);

        sem_post(&slots_available);
        usleep(rand() % 10);
    }
    printf("[producer %d] exit\n", producer_id);

    *result = 1; /* to test return values */
    return result;
}

void *consumer(void *arg)
{
    int consumer_id = *((int *)arg);

    while (1)
    {
        printf("[consumer %d] waiting for available\n", consumer_id);
        /* listen for a filled slot */
        sem_wait(&slots_available);

        /* Lock the buffer to use it */
        pthread_mutex_lock(&buffer_mutex);

        /* Validate that is ok to consume */
        if (buffer_index <= 0)
        {
            pthread_mutex_unlock(&buffer_mutex);
            sem_post(&slots_available);
            break;
        }
        buffer_index--;
        int item = buffer[buffer_index];
        printf("[consumer %d] Consumed buffer [%d] = %d\n", consumer_id, buffer_index, item);
        
        pthread_mutex_unlock(&buffer_mutex); /* finish using buffer */

        sem_post(&empty_slots);
        
        // Sleep to simulate variable consumption time
        usleep(rand() % 10);
    }
    printf("[consumer %d] exit\n", consumer_id);
    return NULL;
}

int main()
{
    pthread_t p_producers[ALLOWED_PRODUCERS];
    pthread_t p_consumers[ALLOWED_CONSUMERS];
    int producers_ids[ALLOWED_PRODUCERS];
    int consumers_ids[ALLOWED_CONSUMERS];
    void *p_presult;
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&slots_available, 0, 0);

    /* Create producers */
    for (int i = 0; i < ALLOWED_PRODUCERS; i++)
    {
        producers_ids[i] = i + 1;
        pthread_create(&p_producers[i], NULL, producer, &producers_ids[i]);
    }
    /* Create consumers */
    for (int i = 0; i < ALLOWED_CONSUMERS; i++)
    {
        consumers_ids[i] = i + 1;
        pthread_create(&p_consumers[i], NULL, consumer, &consumers_ids[i]);
    }

    /* Wait Producers to finish */
    for (int i = 0; i < ALLOWED_PRODUCERS; i++)
    {
        pthread_join(p_producers[i], &p_presult);
        int *result_value = (int *)p_presult;
        printf("Producer thread %d returned: %d\n",producers_ids[i], *result_value); /* Example to get return values, however NULL to ignore is possible */
        free(result_value);
    }

    /* Signal consumers that production is done */
    pthread_mutex_lock(&buffer_mutex);
    buffer_index = 0; /* Special value to signal end */
    pthread_mutex_unlock(&buffer_mutex);
    /* Wake up consumer that has locked his execution with sem_wait() */
    for (int i = 0; i < ALLOWED_CONSUMERS; i++)
    {
        sem_post(&slots_available);
    }
    /* Wait Consumers to finish */
    for (int i = 0; i < ALLOWED_CONSUMERS; i++)
    {
        pthread_join(p_consumers[i], NULL);
    }

    /* cleanup */
    sem_destroy(&empty_slots);
    sem_destroy(&slots_available);

    return 0;
}
