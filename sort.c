#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Définir les directions de tri
#define ASC 0
#define DESC 1

// Trier une colonne
void trier_colonne(COLUMN *col, int direction_tri) {
    if (col == NULL || col->donnees == NULL || col->taille == 0) {
        fprintf(stderr, "La colonne ou les données ne sont pas initialisées ou sont vides.\n");
        return;
    }

    if (col->index == NULL) {
        col->index = malloc(col->max_taille * sizeof(unsigned long long));
        if (col->index == NULL) {
            fprintf(stderr, "Échec de l'allocation de mémoire pour le tableau d'index.\n");
            return;
        }
    }

    // Initialiser l'index
    for (unsigned int i = 0; i < col->taille; i++) {
        col->index[i] = i;
    }

    // Effectuer le tri rapide
    tri_rapide(col->index, 0, col->taille - 1, direction_tri, col);

    col->valid_index = 1;
    col->direction_tri = direction_tri;
}

// Tri rapide
void tri_rapide(unsigned long long *index, int bas, int haut, int direction_tri, COLUMN *col) {
    if (bas < haut) {
        int pi = partitionner(index, bas, haut, direction_tri, col);
        tri_rapide(index, bas, pi - 1, direction_tri, col);
        tri_rapide(index, pi + 1, haut, direction_tri, col);
    }
}

// Partitionner pour le tri rapide
int partitionner(unsigned long long *index, int bas, int haut, int direction_tri, COLUMN *col) {
    COL_TYPE *pivot = col->donnees[index[haut]];
    int i = bas - 1;

    for (int j = bas; j <= haut - 1; j++) {
        int comparison = comparer_valeurs(col->type_colonne, col->donnees[index[j]], pivot);
        int should_swap = (direction_tri == ASC) ? (comparison <= 0) : (comparison > 0);

        if (should_swap) {
            i++;
            echanger_indexes(&index[i], &index[j]);
        }
    }

    echanger_indexes(&index[i + 1], &index[haut]);
    return i + 1;
}

// Échanger deux index
void echanger_indexes(unsigned long long *a, unsigned long long *b) {
    unsigned long long temp = *a;
    *a = *b;
    *b = temp;
}

// Imprimer une colonne par index
void imprimer_colonne_par_index(COLUMN *col) {
    if (col == NULL || col->index == NULL || col->valid_index == 0) {
        printf("La colonne ou l'index n'est pas initialisé ou n'est pas valide.\n");
        return;
    }

    char buffer[256];
    for (unsigned int i = 0; i < col->taille; i++) {
        convertir_valeur(col, col->index[i], buffer, sizeof(buffer));
        printf("[%llu] %s\n", col->index[i], buffer);
    }
}

// Effacer l'index d'une colonne
void effacer_index(COLUMN *col) {
    if (col == NULL || col->index == NULL) {
        return;
    }

    free(col->index);
    col->index = NULL;
    col->valid_index = 0;
}

// Vérifier la validité de l'index d'une colonne
int verifier_index(COLUMN *col) {
    if (col == NULL || col->index == NULL) {
        return 0;
    }

    return col->valid_index ? 1 : -1;
}

// Mettre à jour l'index d'une colonne
void mettre_a_jour_index(COLUMN *col) {
    if (col == NULL || col->donnees == NULL || col->taille == 0) {
        fprintf(stderr, "La colonne ou les données ne sont pas initialisées ou sont vides.\n");
        return;
    }

    if (col->index == NULL) {
        col->index = malloc(col->max_taille * sizeof(unsigned long long));
        if (col->index == NULL) {
            fprintf(stderr, "Échec de l'allocation de mémoire pour le tableau d'index.\n");
            return;
        }
    }

    // Initialiser l'index
    for (unsigned int i = 0; i < col->taille; i++) {
        col->index[i] = i;
    }

    col->valid_index = 1;
}

// Rechercher une valeur dans une colonne
int rechercher_valeur_dans_colonne(COLUMN *col, void *valeur) {
    if (col == NULL || col->index == NULL || col->valid_index == 0) {
        fprintf(stderr, "La colonne ou l'index n'est pas initialisé ou n'est pas valide.\n");
        return -1;
    }

    int bas = 0, haut = col->taille - 1;
    while (bas <= haut) {
        int mid = bas + (haut - bas) / 2;
        int comparison = comparer_valeurs(col->type_colonne, col->donnees[col->index[mid]], valeur);

        if (comparison == 0) {
            return 1;
        } else if (comparison < 0) {
            bas = mid + 1;
        } else {
            haut = mid - 1;
        }
    }

    return 0;
}
