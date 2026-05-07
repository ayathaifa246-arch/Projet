#ifndef CLOSEST_H
#define CLOSEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

typedef struct
{
    int id;
    float x;
    float y;
    float z;

} Drone;

/* Distance euclidienne entre deux drones */
float distanceDrone(Drone *a, Drone *b);

/* Fonction de comparaison pour qsort */
int compareX(const void *a, const void *b);

/* Recherche naïve */
float bruteForce(Drone *tab, int n);

/* Algorithme récursif optimisé */
float closestUtil(Drone *tab, int n);

/* Fonction principale */
float closest(Drone *tab, int n);

#endif