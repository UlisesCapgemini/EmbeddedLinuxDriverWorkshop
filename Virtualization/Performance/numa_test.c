
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* to test it run on command window 
 * perf stat ./numa_test_st region1
 * perf stat ./numa_test_st region2 */
#define SIZE 1024 * 1024

void access_region(int *region)
{
    for (int i = 0; i < SIZE; i++)
    {
        region[i] += 1;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s [region1|region2]\n", argv[0]);
        return 1;
    }

    // Simular dos regiones de memoria como si fueran nodos NUMA
    int *region1 = malloc(SIZE * sizeof(int));
    int *region2 = malloc(SIZE * sizeof(int));

    if (!region1 || !region2)
    {
        perror("malloc");
        return 1;
    }

    memset(region1, 0, SIZE * sizeof(int));
    memset(region2, 0, SIZE * sizeof(int));

    if (strcmp(argv[1], "region1") == 0)
    {
        access_region(region1);
    }
    else if (strcmp(argv[1], "region2") == 0)
    {
        access_region(region2);
    }
    else
    {
        printf("Argumento inválido. Use 'region1' o 'region2'.\n");
        free(region1);
        free(region2);
        return 1;
    }

    free(region1);
    free(region2);
    return 0;
}
