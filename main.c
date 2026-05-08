#include "closest.h"

int main()
{
    int n = 10000;

    /* allocation du tableau de drones */
    Drone *essaim = (Drone *)malloc(n * sizeof(Drone));

    if (essaim == NULL)
    {
        printf("Erreur : allocation memoire impossible\n");
        return 1;
    }

    srand(time(NULL));

    int i;
    Drone *curseur = essaim;

    /* initialisation aleatoire des coordonnees de chaque drone */
    for (i = 0; i < n; i++)
    {
        curseur->id = i + 1;
        curseur->x  = (float)(rand() % 10000);
        curseur->y  = (float)(rand() % 10000);
        curseur->z  = (float)(rand() % 10000);
        curseur++; /* on passe au drone suivant */
    }

    /* mesure du temps d'execution de l'algorithme */
    clock_t debut = clock();

    float minDistance = closest(essaim, n);

    clock_t fin  = clock();
    double temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    /* recherche des deux drones correspondant a la distance minimale
     * on utilise une tolerance car les flottants ne sont jamais exactement egaux */
    Drone drone1, drone2;
    int   trouve    = 0;
    float tolerance = 0.001f;

    for (i = 0; i < n && !trouve; i++)
    {
        int j;
        for (j = i + 1; j < n && !trouve; j++)
        {
            float d = distanceDrone((essaim + i), (essaim + j));

            if (d <= minDistance + tolerance)
            {
                drone1 = *(essaim + i);
                drone2 = *(essaim + j);
                trouve = 1;
            }
        }
    }

    /* affichage des resultats */
    printf("=== RESULTAT ===\n");
    printf("Distance minimale : %f\n\n", minDistance);

    printf("Drone 1\n");
    printf("ID : %d\n",   drone1.id);
    printf("X  : %.2f\n", drone1.x);
    printf("Y  : %.2f\n", drone1.y);
    printf("Z  : %.2f\n", drone1.z);

    printf("\nDrone 2\n");
    printf("ID : %d\n",   drone2.id);
    printf("X  : %.2f\n", drone2.x);
    printf("Y  : %.2f\n", drone2.y);
    printf("Z  : %.2f\n", drone2.z);

    printf("\nTemps d'execution : %.6f secondes\n", temps);

    /* liberation de la memoire */
    free(essaim);
    essaim = NULL;

    return 0;
}
    return 0;
}
