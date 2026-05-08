#ifndef CLOSEST_H
#define CLOSEST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

/*
 * Structure representant un drone dans l'espace 3D.
 * Chaque drone possede un identifiant unique et trois coordonnees flottantes.
 */
typedef struct
{
    int   id;
    float x;
    float y;
    float z;

} Drone;

/* Calcule la distance euclidienne entre deux drones dans l'espace 3D */
float distanceDrone(Drone *a, Drone *b);

/* Fonction de comparaison pour le tri rapide (qsort) selon l'axe X */
int compareX(const void *a, const void *b);

/* Fonction de comparaison pour le tri rapide (qsort) selon l'axe Y */
int compareY(const void *a, const void *b);

/* Recherche naïve en O(n^2) : compare toutes les paires possibles */
float bruteForce(Drone *tab, int n);

/* Coeur recursif de l'algorithme Divide & Conquer */
float closestUtil(Drone *tab, int n);

/* Fonction principale : trie le tableau puis lance la recherche optimisee */
float closest(Drone *tab, int n);

#endif