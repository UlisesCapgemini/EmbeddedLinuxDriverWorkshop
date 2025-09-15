#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>

// Shared data structure
struct message {
    int data;
    atomic_int ready;
};

struct message* msg;

void* writer_thread(void* arg) {
    for (int i = 0; i < 100; i++) {
        // Prepare data
        msg->data = i * 10;
        
        // Release semantics: all previous writes become visible
        // to other threads that acquire this atomic
        atomic_store_explicit(&msg->ready, 1, memory_order_release);
        
        printf("Writer: Published message %d\n", i);
        
        // Wait until consumer reads the message
        while (atomic_load_explicit(&msg->ready, memory_order_relaxed) == 1) {
            // Busy wait
        }
    }
    return NULL;
}

void* reader_thread(void* arg) {
    for (int i = 0; i < 100; i++) {
        int ready;
        
        // Wait for message to be ready
        do {
            // Acquire semantics: synchronizes with the release store
            // ensuring all writes by the other thread are visible
            ready = atomic_load_explicit(&msg->ready, memory_order_acquire);
        } while (!ready);
        
        // Safe to read data now
        printf("Reader: Received data %d\n", msg->data);
        
        // Mark as processed
        atomic_store_explicit(&msg->ready, 0, memory_order_release);
    }
    return NULL;
}

int main() {
    // Allocate shared message
    msg = malloc(sizeof(struct message));
    msg->data = 0;
    atomic_init(&msg->ready, 0);
    
    pthread_t writer, reader;
    
    pthread_create(&reader, NULL, reader_thread, NULL);
    pthread_create(&writer, NULL, writer_thread, NULL);
    
    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    
    free(msg);
    return 0;
}