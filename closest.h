#ifndef CLOSEST_H
#define CLOSEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

/* structure d'un drone dans l'espace 3D */
typedef struct
{
    int   id;
    float x;
    float y;
    float z;

} Drone;

/* distance euclidienne entre deux drones */
float distanceDrone(Drone *a, Drone *b);

/* comparaison pour trier par X */
int compareX(const void *a, const void *b);

/* comparaison pour trier par Y */
int compareY(const void *a, const void *b);

/* recherche naive O(n^2) */
float bruteForce(Drone *tab, int n);

/* algorithme recursif Divide & Conquer */
float closestUtil(Drone *tab, int n);

/* fonction principale */
float closest(Drone *tab, int n);

#endif*tab, int n);

#endif
