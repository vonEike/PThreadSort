#include "heap.h"

static int comparePoints(const Point3D *a, const Point3D *b) {
    if (a->x < b->x) return -1;
    if (a->x > b->x) return 1;

    if (a->y < b->y) return -1;
    if (a->y > b->y) return 1;

    if (a->z < b->z) return -1;
    if (a->z > b->z) return 1;

    return 0;
}

static void swap(Point3D *a, Point3D *b) {
    Point3D temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify(Point3D arr[], int n, int i) {
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && comparePoints(&arr[l], &arr[largest]) > 0)
        largest = l;

    if (r < n && comparePoints(&arr[r], &arr[largest]) > 0)
        largest = r;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(Point3D arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n-1; i > 0; i--) {
        swap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}