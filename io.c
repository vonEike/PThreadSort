#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"

static int readLegacyFormat(FILE *file,
                            Point3D **array,
                            int *count) {

    if (fscanf(file, "%d", count) != 1)
        return 2;

    if (*count <= 0)
        return 2;

    *array = malloc((size_t)(*count) * sizeof(Point3D));
    if (!(*array))
        return 3;

    for (int i = 0; i < *count; i++) {
        if (fscanf(file, "%lf %lf %lf",
                   &((*array)[i].x),
                   &((*array)[i].y),
                   &((*array)[i].z)) != 3) {
            free(*array);
            *array = NULL;
            return 4;
        }
    }

    return 0;
}

int readArrayFromFile(const char *filename,
                      Point3D **array,
                      int *count) {

    FILE *file = fopen(filename, "r");
    if (!file)
        return 1;

    int legacyResult = readLegacyFormat(file, array, count);
    fclose(file);

    if (legacyResult == 0)
        return 0;

    file = fopen(filename, "r");
    if (!file)
        return 1;

    int inPointsSection = 0;
    double *values = NULL;
    int valuesCount = 0;
    int valuesCapacity = 0;
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        if (!inPointsSection) {
            if (strstr(line, "3D points") != NULL)
                inPointsSection = 1;
            continue;
        }

        for (char *p = line; *p != '\0'; p++) {
            if (*p == ',')
                *p = '.';
        }

        char *cursor = line;
        while (isspace((unsigned char)*cursor))
            cursor++;

        if (*cursor == '\0')
            continue;

        char *endPtr;
        double value = strtod(cursor, &endPtr);

        if (endPtr == cursor)
            continue;

        while (isspace((unsigned char)*endPtr))
            endPtr++;

        if (*endPtr != '\0' && *endPtr != '\n')
            continue;

        if (valuesCount == valuesCapacity) {
            int newCapacity = valuesCapacity == 0 ? 32 : valuesCapacity * 2;
            double *newValues = realloc(values,
                                        (size_t)newCapacity * sizeof(double));
            if (!newValues) {
                free(values);
                fclose(file);
                return 3;
            }
            values = newValues;
            valuesCapacity = newCapacity;
        }

        values[valuesCount++] = value;
    }

    fclose(file);

    int pointsCount = valuesCount / 3;
    if (pointsCount <= 0) {
        free(values);
        return 2;
    }

    *array = malloc((size_t)pointsCount * sizeof(Point3D));
    if (!(*array)) {
        free(values);
        return 3;
    }

    for (int i = 0; i < pointsCount; i++) {
        (*array)[i].x = values[i * 3];
        (*array)[i].y = values[i * 3 + 1];
        (*array)[i].z = values[i * 3 + 2];
    }

    *count = pointsCount;
    free(values);

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
