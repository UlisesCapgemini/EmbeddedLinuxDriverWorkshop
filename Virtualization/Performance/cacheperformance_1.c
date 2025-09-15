
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024 * 1024

void sequential_access(int *array)
{
    for (int i = 0; i < SIZE; i++)
    {
        array[i] += 1;
    }
}

void stride_access(int *array, int stride)
{
    for (int i = 0; i < SIZE; i += stride)
    {
        array[i] += 1;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s [sequential|stride]\n", argv[0]);
        return 1;
    }

    int *array = malloc(SIZE * sizeof(int));
    if (!array)
    {
        perror("malloc");
        return 1;
    }
    memset(array, 0, SIZE * sizeof(int));

    if (strcmp(argv[1], "sequential") == 0)
    {
        sequential_access(array);
    }
    else if (strcmp(argv[1], "stride") == 0)
    {
        stride_access(array, 64);
    }
    else
    {
        printf("Argumento inválido. Use 'sequential' o 'stride'.\n");
        free(array);
        return 1;
    }

    free(array);
    return 0;
}
