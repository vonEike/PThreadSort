#ifndef PARALLEL_H
#define PARALLEL_H

#include "point.h"

void parallelHeapSort(Point3D *array,
                      int size,
                      int threads_count);

#endif