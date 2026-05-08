#include "closest.h"

int main()
{
    int n = 10000;

    Drone *essaim = (Drone *)malloc(n * sizeof(Drone));

    if (essaim == NULL)
    {
        printf("Erreur critique : allocation memoire impossible\n");
        return 1;
    }

    srand(time(NULL));

    int i;
    Drone *curseur = essaim;

    for (i = 0; i < n; i++)
    {
        curseur->id = i + 1;
        curseur->x  = (float)(rand() % 10000);
        curseur->y  = (float)(rand() % 10000);
        curseur->z  = (float)(rand() % 10000);
        curseur++;
    }

    clock_t debut = clock();

    /* Etape 1 : algorithme optimise Divide & Conquer -> O(n log^2 n) */
    float minDistance = closest(essaim, n);

    clock_t fin   = clock();
    double  temps = ((double)(fin - debut)) / CLOCKS_PER_SEC;

    /* Etape 2 : identification des deux drones concernes.
     * On parcourt une seule fois le tableau en cherchant
     * la paire dont la distance correspond a minDistance.
     * On utilise une tolerance flottante pour la comparaison. */
    Drone drone1, drone2;
    int   trouve = 0;
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

    /* Affichage des resultats */
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

    free(essaim);
    essaim = NULL;

    return 0;
}