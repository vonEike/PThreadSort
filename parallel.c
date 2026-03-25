#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "parallel.h"

typedef struct {
    Point3D *array;
    int left;
    int right;
} ThreadData;

static void* threadFunction(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    heapSort(data->array + data->left,
             data->right - data->left);
    pthread_exit(NULL);
}

static int comparePoints(const Point3D *a, const Point3D *b) {
    if (a->x < b->x) return -1;
    if (a->x > b->x) return 1;

    if (a->y < b->y) return -1;
    if (a->y > b->y) return 1;

    if (a->z < b->z) return -1;
    if (a->z > b->z) return 1;

    return 0;
}

static void merge(Point3D *array,
                  int left,
                  int mid,
                  int right) {

    int total = right - left;
    Point3D *temp = malloc(total * sizeof(Point3D));

    int i = left;
    int j = mid;
    int k = 0;

    while (i < mid && j < right) {
        if (comparePoints(&array[i], &array[j]) <= 0)
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }

    while (i < mid)
        temp[k++] = array[i++];

    while (j < right)
        temp[k++] = array[j++];

    memcpy(array + left, temp,
           total * sizeof(Point3D));

    free(temp);
}

void parallelHeapSort(Point3D *array,
                      int size,
                      int threads_count) {

    pthread_t *threads =
        malloc(sizeof(pthread_t) * threads_count);

    ThreadData *data =
        malloc(sizeof(ThreadData) * threads_count);

    int *bounds =
        malloc(sizeof(int) * (threads_count + 1));

    int chunk = size / threads_count;

    for (int i = 0; i < threads_count; i++) {

        bounds[i] = i * chunk;

        data[i].array = array;
        data[i].left = i * chunk;
        data[i].right =
            (i == threads_count - 1)
            ? size
            : (i + 1) * chunk;

        pthread_create(&threads[i],
                       NULL,
                       threadFunction,
                       &data[i]);
    }

    bounds[threads_count] = size;

    for (int i = 0; i < threads_count; i++)
        pthread_join(threads[i], NULL);

    int current_parts = threads_count;

    while (current_parts > 1) {

        int new_parts = 0;

        for (int i = 0; i < current_parts; i += 2) {

            if (i + 1 < current_parts) {

                merge(array,
                      bounds[i],
                      bounds[i + 1],
                      bounds[i + 2]);

                bounds[new_parts++] = bounds[i];

            } else {
                bounds[new_parts++] = bounds[i];
            }
        }

        bounds[new_parts] = size;
        current_parts = new_parts;
    }

    free(threads);
    free(data);
    free(bounds);
}