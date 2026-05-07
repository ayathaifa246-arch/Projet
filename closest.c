#include "closest.h"

float distanceDrone(Drone *a, Drone *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

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
            {
                min = d;
            }
        }
    }

    return min;
}

float closestUtil(Drone *tab, int n)
{
    if (n <= 3)
    {
        return bruteForce(tab, n);
    }

    int mid = n / 2;

    Drone *midDrone = (tab + mid);

    float dl = closestUtil(tab, mid);

    float dr = closestUtil((tab + mid), n - mid);

    float d;

    if (dl < dr)
        d = dl;
    else
        d = dr;

    Drone *strip;

    strip = (Drone *)malloc(n * sizeof(Drone));

    if (strip == NULL)
    {
        printf("Erreur mémoire\n");
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

    int k;

    for (i = 0; i < j; i++)
    {
        for (k = i + 1; k < j; k++)
        {
            float dist = distanceDrone((strip + i), (strip + k));

            if (dist < d)
            {
                d = dist;
            }
        }
    }

    free(strip);

    return d;
}

float closest(Drone *tab, int n)
{
    qsort(tab, n, sizeof(Drone), compareX);

    return closestUtil(tab, n);
}