#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

// Global flag for graceful shutdown
volatile sig_atomic_t running = 1;

// Signal handler for graceful shutdown
void signal_handler(int sig) {
    printf("\nReceived signal %d, shutting down gracefully...\n", sig);
    running = 0;
}

// Function to display usage information
void print_usage(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Options:\n");
    printf("  -h, --help     Show this help message\n");
    printf("  -v, --version  Show version information\n");
    printf("  -n <name>      Specify a name (default: World)\n");
}

// Function to display version
void print_version(void) {
    printf("Simple Linux App v1.0\n");
    printf("Built on %s at %s\n", __DATE__, __TIME__);
}

int main(int argc, char *argv[]) {
    char name[256] = "World";
    int opt;
    
    // Set up signal handlers
    signal(SIGINT, signal_handler);   // Ctrl+C
    signal(SIGTERM, signal_handler);  // Termination signal
    
    // Parse command line arguments
    while ((opt = getopt(argc, argv, "hvn:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'v':
                print_version();
                return EXIT_SUCCESS;
            case 'n':
                strncpy(name, optarg, sizeof(name) - 1);
                name[sizeof(name) - 1] = '\0';
                break;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }
    
    printf("Simple Linux Application Started\n");
    printf("Process ID: %d\n", getpid());
    printf("Hello, %s!\n", name);
    
    // Main application loop
    int counter = 0;
    while (running) {
        printf("Running... (iteration %d)\n", ++counter);
        
        // Simulate some work
        sleep(2);
        
        // Exit after 10 iterations for demo purposes
        if (counter >= 10) {
            printf("Completed 10 iterations, exiting...\n");
            break;
        }
    }
    
    printf("Application shutting down...\n");
    return EXIT_SUCCESS;
}