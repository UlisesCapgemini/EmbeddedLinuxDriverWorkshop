#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

volatile int shared_flag = 0;
volatile int shared_data = 0;

// ARM-specific barriers
static inline void dmb_ish(void) {
    __asm__ volatile ("dmb ish" : : : "memory");
}

static inline void dsb_sy(void) {
    __asm__ volatile ("dsb sy" : : : "memory");
}

void* producer_thread(void* arg) {
    for (int i = 0; i < 1000; i++) {
        // Prepare data
        shared_data = i;
        
        // Data memory barrier - ensure data is visible
        dmb_ish();
        
        // Set flag
        shared_flag = 1;
        
        // Wait for consumer to process
        while (shared_flag == 1) {
            // Busy wait
        }
    }
    return NULL;
}

void* consumer_thread(void* arg) {
    for (int i = 0; i < 1000; i++) {
        // Wait for new data
        while (shared_flag == 0) {
            // Busy wait
        }
        
        // Data synchronization barrier - ensure we see the latest data
        dsb_sy();
        
        // Process data
        printf("Received: %d\n", shared_data);
        
        // Reset flag
        shared_flag = 0;
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    pthread_create(&cons, NULL, consumer_thread, NULL);
    pthread_create(&prod, NULL, producer_thread, NULL);
    
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    return 0;
}