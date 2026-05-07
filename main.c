#include "closest.h"

int main()
{
    int n = 10000;

    Drone *essaim;

    essaim = (Drone *)malloc(n * sizeof(Drone));

    if (essaim == NULL)
    {
        printf("Erreur allocation mémoire\n");
        return 1;
    }

    srand(time(NULL));

    int i;

    for (i = 0; i < n; i++)
    {
        (essaim + i)->id = i + 1;

        (essaim + i)->x = rand() % 10000;
        (essaim + i)->y = rand() % 10000;
        (essaim + i)->z = rand() % 10000;
    }

    clock_t debut = clock();

    float minDistance = closest(essaim, n);

    clock_t fin = clock();

    double temps =
        ((double)(fin - debut)) / CLOCKS_PER_SEC;

    printf("Distance minimale : %f\n", minDistance);

    printf("Temps execution : %f secondes\n", temps);

    free(essaim);

    return 0;
}