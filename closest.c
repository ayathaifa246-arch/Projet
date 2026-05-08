#include "closest.h"

/*
 * distanceDrone
 * -------------
 * Calcule la distance euclidienne entre deux drones a et b dans l'espace 3D.
 * Formule : sqrt( (dx)^2 + (dy)^2 + (dz)^2 )
 */
float distanceDrone(Drone *a, Drone *b)
{
    float dx = a->x - b->x;
    float dy = a->y - b->y;
    float dz = a->z - b->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

/*
 * compareX
 * --------
 * Fonction de comparaison utilisee par qsort pour trier les drones
 * par ordre croissant de leur coordonnee X.
 * Retourne -1, 0 ou 1 selon la relation entre d1->x et d2->x.
 */
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

/*
 * compareY
 * --------
 * Fonction de comparaison utilisee par qsort pour trier les drones
 * par ordre croissant de leur coordonnee Y.
 * Necessaire pour le tri de la bande centrale (strip) afin de
 * limiter les comparaisons a 7 voisins au maximum.
 */
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

/*
 * bruteForce
 * ----------
 * Approche naive : compare toutes les paires de drones du tableau.
 * Complexite temporelle : O(n^2)
 * Complexite spatiale   : O(1)
 * Utilisee uniquement pour les petits sous-tableaux (n <= 3) en base
 * de la recursion, la ou le cout quadratique est negligeable.
 */
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

/*
 * closestUtil
 * -----------
 * Coeur de l'algorithme Divide & Conquer.
 *
 * Principe :
 *   1. Cas de base : si n <= 3, on utilise bruteForce directement.
 *   2. On divise le tableau en deux moitiés gauche et droite.
 *   3. On resout recursivement chaque moitie -> dl et dr.
 *   4. d = min(dl, dr) est la meilleure distance connue.
 *   5. On construit une bande verticale de largeur 2*d autour du drone median.
 *   6. On trie cette bande par Y pour exploiter la propriete geometrique :
 *      dans la bande, chaque drone ne peut etre dangereux qu'avec les 7
 *      drones suivants au maximum (rectangle 2d x d contient au plus 8 points).
 *   7. On met a jour d si une paire plus proche est trouvee dans la bande.
 *
 * Complexite temporelle : O(n log^2 n) avec le tri de la bande,
 *                         O(n log n)   si on maintient un tableau Y global pre-trie.
 * Complexite spatiale   : O(n) pour le tableau strip alloue dynamiquement.
 */
float closestUtil(Drone *tab, int n)
{
    /* Cas de base : on bascule vers la force brute pour les petits groupes */
    if (n <= 3)
        return bruteForce(tab, n);

    /* --- Etape 1 : Division --- */
    int mid = n / 2;

    /* Le drone median sert de frontiere entre les deux demi-espaces */
    Drone *midDrone = (tab + mid);

    /* --- Etape 2 : Conquete (appels recursifs) --- */
    float dl = closestUtil(tab, mid);
    float dr = closestUtil((tab + mid), n - mid);

    /* d = la meilleure distance trouvee dans les deux sous-espaces */
    float d = (dl < dr) ? dl : dr;

    /* --- Etape 3 : Fusion - construction de la bande centrale --- */

    /*
     * On alloue un tableau temporaire pour stocker les drones
     * dont la distance en X au drone median est inferieure a d.
     * Ces drones sont les seuls susceptibles de former une paire
     * plus proche que d a cheval sur les deux moitiés.
     */
    Drone *strip = (Drone *)malloc(n * sizeof(Drone));

    if (strip == NULL)
    {
        printf("Erreur : allocation de la bande centrale impossible\n");
        exit(1);
    }

    int i;
    int j = 0;

    /* Remplissage de la bande : on navigue avec l'arithmetique des pointeurs */
    for (i = 0; i < n; i++)
    {
        if (fabs((tab + i)->x - midDrone->x) < d)
        {
            *(strip + j) = *(tab + i);
            j++;
        }
    }

    /*
     * Tri de la bande par Y.
     * Cette etape est cruciale : elle garantit que lors du parcours
     * suivant, on peut s'arreter des que l'ecart en Y depasse d.
     * Consequence geometrique : on ne compare jamais plus de 7 voisins.
     */
    qsort(strip, j, sizeof(Drone), compareY);

    /* --- Etape 4 : Recherche dans la bande triee --- */
    int k;

    for (i = 0; i < j; i++)
    {
        /*
         * Grace au tri par Y, on s'arrete des que l'ecart vertical
         * entre les deux drones depasse d : les suivants ne peuvent
         * pas etre plus proches.
         */
        for (k = i + 1; k < j && ((strip + k)->y - (strip + i)->y) < d; k++)
        {
            float dist = distanceDrone((strip + i), (strip + k));

            if (dist < d)
                d = dist;
        }
    }

    /* Liberation du tableau temporaire : pas de fuite memoire */
    free(strip);

    return d;
}

/*
 * closest
 * -------
 * Fonction principale du module.
 * Pre-condition : tab pointe vers un tableau de n drones valides.
 *
 * Etape 1 : tri du tableau par coordonnee X via qsort -> O(n log n)
 * Etape 2 : lancement de l'algorithme recursif              -> O(n log^2 n)
 *
 * Complexite globale : O(n log^2 n)
 */
float closest(Drone *tab, int n)
{
    /* Le tri initial par X est indispensable au bon fonctionnement
     * du Divide & Conquer : il garantit que la coupe au milieu du
     * tableau correspond bien a une coupe spatiale dans l'espace. */
    qsort(tab, n, sizeof(Drone), compareX);

    return closestUtil(tab, n);
}
