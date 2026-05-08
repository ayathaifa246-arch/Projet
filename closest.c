#include "closest.h"

/* calcule la distance euclidienne entre deux drones en 3D */
float distanceDrone(Drone *a, Drone *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

/* comparaison par coordonnee X pour qsort */
int compareX(const void *a, const void *b)
{
    Drone *d1 = (Drone *)a;
    Drone *d2 = (Drone *)b;

    if (d1->x < d2->x)
        return -1;

    if (d1->x > d2->x)
        return 1;

    return 0;
}

/* comparaison par coordonnee Y pour qsort */
int compareY(const void *a, const void *b)
{
    Drone *d1 = (Drone *)a;
    Drone *d2 = (Drone *)b;

    if (d1->y < d2->y)
        return -1;

    if (d1->y > d2->y)
        return 1;

    return 0;
}

/* recherche naive : compare toutes les paires, utilisee pour n <= 3 */
float bruteForce(Drone *tab, int n)
{
    float min = FLT_MAX;

    int i;
    int j;

    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            float d = distanceDrone((tab + i), (tab + j));

            if (d < min)
                min = d;
        }
    }

    return min;
}

/* coeur recursif de l'algorithme Divide & Conquer */
float closestUtil(Drone *tab, int n)
{
    /* cas de base : on utilise bruteForce pour les petits groupes */
    if (n <= 3)
        return bruteForce(tab, n);

    int mid = n / 2;
    Drone *midDrone = (tab + mid);

    /* appels recursifs sur les deux moities */
    float dl = closestUtil(tab, mid);
    float dr = closestUtil((tab + mid), n - mid);

    /* on garde la plus petite distance */
    float d = (dl < dr) ? dl : dr;

    /* construction de la bande centrale */
    Drone *strip = (Drone *)malloc(n * sizeof(Drone));

    if (strip == NULL)
    {
        printf("Erreur : allocation impossible\n");
        exit(1);
    }

    int i;
    int j = 0;

    for (i = 0; i < n; i++)
    {
        if (fabs((tab + i)->x - midDrone->x) < d)
        {
            *(strip + j) = *(tab + i);
            j++;
        }
    }

    /* tri de la bande par Y pour limiter les comparaisons a 7 voisins */
    qsort(strip, j, sizeof(Drone), compareY);

    /* parcours de la bande */
    int k;

    for (i = 0; i < j; i++)
    {
        for (k = i + 1; k < j && ((strip + k)->y - (strip + i)->y) < d; k++)
        {
            float dist = distanceDrone((strip + i), (strip + k));

            if (dist < d)
                d = dist;
        }
    }

    free(strip);

    return d;
}

/* trie le tableau par X puis lance l'algorithme recursif */
float closest(Drone *tab, int n)
{
    qsort(tab, n, sizeof(Drone), compareX);

    return closestUtil(tab, n);
}
