#ifndef IO_H
#define IO_H

#include "point.h"

int readArrayFromFile(const char *filename,
                      Point3D **array,
                      int *count);

int writeArrayToFile(const char *filename,
                     Point3D *array,
                     int count);

#endif