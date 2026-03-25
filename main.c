#include <stdio.h>
#include <stdlib.h>
#include "parallel.h"
#include "io.h"

#define THREADS 4

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s 'input file .txt' output file .txt\n", argv[0]);
        return 1;
    }

    Point3D *array = NULL;
    int count;

    if (readArrayFromFile(argv[1],
                          &array,
                          &count) != 0) {
        printf("Error reading input file\n");
        return 1;
    }

    parallelHeapSort(array, count, THREADS);

    if (writeArrayToFile(argv[2],
                         array,
                         count) != 0) {
        printf("Error writing output file\n");
        free(array);
        return 1;
    }

    free(array);
    return 0;
}