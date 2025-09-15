#define _GNU_SOURCE         /* See feature_test_macros(7) - MUST BE FIRST */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/membarrier.h>
#include <errno.h>
#include <time.h>

#define NUM_CORES 2
#define QUEUE_SIZE 1024
#define MAX_PACKET_SIZE 1500

// Estructura para representar un paquete
typedef struct {
    uint8_t data[MAX_PACKET_SIZE];
    uint32_t size;
    uint32_t id;
} packet_t;

// Estructura para la cola en anillo
typedef struct {
    packet_t* packets;
    volatile uint32_t head;
    volatile uint32_t tail;
    uint32_t size;
    pthread_mutex_t lock;  // Changed from spinlock to mutex for better stability
} ring_queue_t;

// Colas de entrada y salida para cada core
ring_queue_t input_queues[NUM_CORES];
ring_queue_t output_queue;

// Flag para indicar a los hilos que deben terminar
volatile int should_exit = 0;

// Inicializar una cola en anillo
int init_ring_queue(ring_queue_t* queue, uint32_t size) {
    if (!queue) return -1;
    
    queue->packets = (packet_t*)calloc(size, sizeof(packet_t));  // Use calloc to zero-initialize
    if (queue->packets == NULL) {
        perror("Failed to allocate memory for queue");
        return -1;
    }
    
    queue->head = 0;
    queue->tail = 0;
    queue->size = size;
    
    int ret = pthread_mutex_init(&queue->lock, NULL);  // Use mutex instead of spinlock
    if (ret != 0) {
        perror("Failed to initialize mutex");
        free(queue->packets);
        queue->packets = NULL;
        return -1;
    }
    
    return 0;
}

// Liberar recursos de la cola
void destroy_ring_queue(ring_queue_t* queue) {
    if (!queue) return;
    
    if (queue->packets != NULL) {
        free(queue->packets);
        queue->packets = NULL;
    }
    pthread_mutex_destroy(&queue->lock);
}

// Encolar un paquete (productor)
int enqueue_packet(ring_queue_t* queue, packet_t* packet) {
    if (queue == NULL || packet == NULL || queue->packets == NULL) {
        return -1;
    }
    
    int result = -1;
    pthread_mutex_lock(&queue->lock);
    
    uint32_t next_tail = (queue->tail + 1) % queue->size;
    if (next_tail != queue->head) {
        // Cola tiene espacio
        memcpy(&queue->packets[queue->tail], packet, sizeof(packet_t));
        queue->tail = next_tail;
        result = 0;
    }
    
    pthread_mutex_unlock(&queue->lock);
    return result;
}

// Desencolar un paquete (consumidor)
int dequeue_packet(ring_queue_t* queue, packet_t* packet) {
    if (queue == NULL || packet == NULL || queue->packets == NULL) {
        return -1;
    }
    
    int result = -1;
    pthread_mutex_lock(&queue->lock);
    
    if (queue->head != queue->tail) {
        // Cola no está vacía
        memcpy(packet, &queue->packets[queue->head], sizeof(packet_t));
        queue->head = (queue->head + 1) % queue->size;
        result = 0;
    }
    
    pthread_mutex_unlock(&queue->lock);
    return result;
}

// Función para procesar un paquete
void process_packet(packet_t* packet) {
    if (!packet) return;
    
    // Simulamos procesamiento
    usleep(10);  // 10 microsegundos de procesamiento simulado
    
    // Aquí iría la lógica real de procesamiento
    packet->id += 1000;  // Solo para demostrar que se procesó
}

// Función para el hilo de trabajo de cada core
void* worker_thread(void* arg) {
    if (!arg) {
        fprintf(stderr, "Worker thread received NULL argument\n");
        return NULL;
    }
    
    int core_id = *((int*)arg);
    if (core_id < 0 || core_id >= NUM_CORES) {
        fprintf(stderr, "Invalid core_id: %d\n", core_id);
        return NULL;
    }
    
    cpu_set_t cpuset;
    
    // Configurar afinidad de CPU para este hilo
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    
    int ret = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (ret != 0) {
        printf("Error: Could not set thread affinity for worker %d: %s\n", 
               core_id, strerror(ret));
    }
    
    // Configurar prioridad en tiempo real - make this optional
    struct sched_param param;
    param.sched_priority = 99;  // Máxima prioridad para SCHED_FIFO
    ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    if (ret != 0) {
        printf("Warning: Could not set thread priority for worker %d: %s\n", 
               core_id, strerror(ret));
        // Continue anyway, this might not be critical
    }
    
    printf("Worker %d iniciado en CPU %d\n", core_id, sched_getcpu());
    
    packet_t packet;
    while (!should_exit) {
        // Intentar obtener un paquete de la cola de entrada
        printf("Worker %d CPU %d : Intentar obtener un paquete de la cola de entrada\n", core_id, sched_getcpu());
        if (dequeue_packet(&input_queues[core_id], &packet) == 0) {
            
            // Procesar el paquete (run-to-completion)
            printf("Worker %d CPU %d : Procesar el paquete (run-to-completion)\n", core_id, sched_getcpu());
            process_packet(&packet);
            printf("Worker %d CPU %d : Enviar a la cola de salida\n", core_id, sched_getcpu());
            // Enviar a la cola de salida
            enqueue_packet(&output_queue, &packet);
        } else {
            // No hay paquetes, esperar un poco
            usleep(100);
        }
    }
    
    printf("Worker %d terminando\n", core_id);
    return NULL;
}

// Función para el hilo de generación de paquetes (simulación de I/O)
void* packet_generator(void* arg) {
    uint32_t packet_id = 0;
    
    while (!should_exit) {
        // Crear un nuevo paquete
        packet_t new_packet;
        memset(&new_packet, 0, sizeof(packet_t));  // Initialize to zero
        
        new_packet.size = (rand() % (MAX_PACKET_SIZE - 64)) + 64;  // Tamaño aleatorio entre 64 y MAX_PACKET_SIZE
        new_packet.id = packet_id++;
        
        // Llenar con datos aleatorios
        for (uint32_t i = 0; i < new_packet.size && i < MAX_PACKET_SIZE; i++) {
            new_packet.data[i] = rand() % 256;
        }
        
        // Distribuir a un core aleatorio para balanceo de carga
        int target_core = rand() % NUM_CORES;
        printf("Distribuir a un core aleatorio para balanceo de carga %d\n", target_core);
        
        // Intentar encolar, si está llena, esperar
        int retry_count = 0;
        while (!should_exit && enqueue_packet(&input_queues[target_core], &new_packet) != 0) {
            usleep(100);
            retry_count++;
            if (retry_count > 100) {  // Avoid infinite loops
                printf("Warning: Failed to enqueue packet after 100 retries\n");
                break;
            }
        }
        
        // Generar paquetes a una tasa razonable
        usleep(500);  // 500 microsegundos entre paquetes
    }
    
    printf("Packet generator terminando\n");
    return NULL;
}

// Función para el hilo que consume los paquetes procesados
void* output_consumer(void* arg) {
    packet_t packet;
    uint32_t packets_processed = 0;
    
    while (!should_exit) {
        if (dequeue_packet(&output_queue, &packet) == 0) {
            packets_processed++;
            
            // Cada 1000 paquetes, mostrar estadísticas
            if (packets_processed % 1000 == 0) {
                printf("Procesados %u paquetes\n", packets_processed);
            }
        } else {
            usleep(100);
        }
    }
    
    printf("Output consumer terminando, procesados %u paquetes\n", packets_processed);
    return NULL;
}

int main() {
    int i, ret;
    pthread_t workers[NUM_CORES];
    pthread_t generator, consumer;
    int core_ids[NUM_CORES];
    
    // Inicializar semilla para números aleatorios
    srand(time(NULL));
    
    // Bloquear memoria para evitar paginación
    if (mlockall(MCL_CURRENT | MCL_FUTURE) != 0) {
        perror("Warning: mlockall failed");
        // Continue anyway, this might not be critical
    }
    
    // Inicializar colas
    for (i = 0; i < NUM_CORES; i++) {
        if (init_ring_queue(&input_queues[i], QUEUE_SIZE) != 0) {
            fprintf(stderr, "Failed to initialize input queue %d\n", i);
            // Cleanup previously initialized queues
            for (int j = 0; j < i; j++) {
                destroy_ring_queue(&input_queues[j]);
            }
            return 1;
        }
    }
    
    if (init_ring_queue(&output_queue, QUEUE_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize output queue\n");
        // Cleanup input queues
        for (i = 0; i < NUM_CORES; i++) {
            destroy_ring_queue(&input_queues[i]);
        }
        return 1;
    }
    
    // Crear hilos de trabajo para cada core
    for (i = 0; i < NUM_CORES; i++) {
        core_ids[i] = i;
        ret = pthread_create(&workers[i], NULL, worker_thread, &core_ids[i]);
        if (ret != 0) {
            fprintf(stderr, "Failed to create worker thread %d: %s\n", i, strerror(ret));
            should_exit = 1;
            // Wait for any created threads to exit
            for (int j = 0; j < i; j++) {
                pthread_join(workers[j], NULL);
            }
            // Cleanup queues
            for (int k = 0; k < NUM_CORES; k++) {
                destroy_ring_queue(&input_queues[k]);
            }
            destroy_ring_queue(&output_queue);
            return 1;
        }
    }
    
    // Crear hilo generador de paquetes
    ret = pthread_create(&generator, NULL, packet_generator, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to create packet generator thread: %s\n", strerror(ret));
        should_exit = 1;
        // Wait for worker threads to exit
        for (i = 0; i < NUM_CORES; i++) {
            pthread_join(workers[i], NULL);
        }
        // Cleanup queues
        for (i = 0; i < NUM_CORES; i++) {
            destroy_ring_queue(&input_queues[i]);
        }
        destroy_ring_queue(&output_queue);
        return 1;
    }
    
    // Crear hilo consumidor de salida
    ret = pthread_create(&consumer, NULL, output_consumer, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to create output consumer thread: %s\n", strerror(ret));
        should_exit = 1;
        // Wait for worker threads and generator to exit
        for (i = 0; i < NUM_CORES; i++) {
            pthread_join(workers[i], NULL);
        }
        pthread_join(generator, NULL);
        // Cleanup queues
        for (i = 0; i < NUM_CORES; i++) {
            destroy_ring_queue(&input_queues[i]);
        }
        destroy_ring_queue(&output_queue);
        return 1;
    }
    
    printf("All threads started successfully. Press Enter to exit...\n");
    getchar();
    
    // Signal threads to exit
    should_exit = 1;
    
    // Wait for threads to finish
    for (i = 0; i < NUM_CORES; i++) {
        pthread_join(workers[i], NULL);
    }
    pthread_join(generator, NULL);
    pthread_join(consumer, NULL);
    
    // Cleanup
    for (i = 0; i < NUM_CORES; i++) {
        destroy_ring_queue(&input_queues[i]);
    }
    destroy_ring_queue(&output_queue);
    
    printf("Program terminated successfully\n");
    return 0;
}