#include <stdio.h>
#include <stdlib.h>
#include "io.h"

int readArrayFromFile(const char *filename,
                      Point3D **array,
                      int *count) {

    FILE *file = fopen(filename, "r");
    if (!file)
        return 1;

    if (fscanf(file, "%d", count) != 1) {
        fclose(file);
        return 2;
    }

    *array = malloc((*count) * sizeof(Point3D));
    if (!(*array)) {
        fclose(file);
        return 3;
    }

    for (int i = 0; i < *count; i++) {
        if (fscanf(file, "%lf %lf %lf",
                   &((*array)[i].x),
                   &((*array)[i].y),
                   &((*array)[i].z)) != 3) {
            free(*array);
            fclose(file);
            return 4;
        }
    }

    fclose(file);
    return 0;
}

int writeArrayToFile(const char *filename,
                     Point3D *array,
                     int count) {

    FILE *file = fopen(filename, "w");
    if (!file)
        return 1;

    fprintf(file, "%d\n", count);

    for (int i = 0; i < count; i++)
        fprintf(file, "%.6f %.6f %.6f\n",
                array[i].x,
                array[i].y,
                array[i].z);

    fclose(file);
    return 0;
}