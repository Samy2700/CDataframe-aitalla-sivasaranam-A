/**
Nom du projet : CDataFrame
Auteurs : Samy AIT ALLA, Sajin Sivasaranam
Rôle : Implémentation des fonctions de tri des colonnes
*/

#include "sort.h"
#include <stdlib.h>
#include <stdio.h>

// Définir les directions de tri
#define ASC 0
#define DESC 1

/**
Rôle : Trier une colonne
Paramètres :
    - col : Pointeur vers la colonne à trier
    - direction_tri : Direction du tri (0 pour ascendant, 1 pour descendant)
Retour : Aucun car elle trie directement la colonne spécifiée.
*/
void trier_colonne(COLONNE *col, int direction_tri) {
    if (col == NULL || col->donnees == NULL || col->taille == 0) { // Vérification des données de la colonne
        fprintf(stderr, "La colonne ou les donnees ne sont pas initialisees ou sont vides.\n");
        return;
    }

    if (col->index == NULL) { // Allocation de mémoire pour l'index si nécessaire
        col->index = malloc(col->taille_max * sizeof(unsigned long long));
        if (col->index == NULL) {
            fprintf(stderr, "Echec de l'allocation de memoire pour le tableau d'index.\n");
            return;
        }
    }

    // Initialiser l'index
    for (unsigned int i = 0; i < col->taille; i++) {
        col->index[i] = i;
    }

    // Effectuer le tri rapide
    tri_rapide(col->index, 0, col->taille - 1, direction_tri, col);

    col->index_valide = 1;
    col->direction_tri = direction_tri;
}

/**
Rôle : Tri rapide pour une colonne
Paramètres :
    - index : Tableau des indices des éléments à trier
    - bas : Index inférieur
    - haut : Index supérieur
    - direction_tri : Direction du tri (0 pour ascendant, 1 pour descendant)
    - col : Pointeur vers la colonne
Retour : Aucun car elle implémente directement l'algorithme de tri rapide.
*/
void tri_rapide(unsigned long long *index, int bas, int haut, int direction_tri, COLONNE *col) {
    if (bas < haut) {
        int pi = partitionner(index, bas, haut, direction_tri, col); // Obtenir l'index de partition
        tri_rapide(index, bas, pi - 1, direction_tri, col); // Tri récursif sur la sous-partie gauche
        tri_rapide(index, pi + 1, haut, direction_tri, col); // Tri récursif sur la sous-partie droite
    }
}

/**
Rôle : Partitionner pour le tri rapide
Paramètres :
    - index : Tableau des indices des éléments à partitionner
    - bas : Index inférieur
    - haut : Index supérieur
    - direction_tri : Direction du tri (0 pour ascendant, 1 pour descendant)
    - col : Pointeur vers la colonne
Retour : Index de partition
*/
int partitionner(unsigned long long *index, int bas, int haut, int direction_tri, COLONNE *col) {
    TYPE_COLONNE *pivot = col->donnees[index[haut]]; // Sélection du pivot
    int i = bas - 1;

    for (int j = bas; j <= haut - 1; j++) {
        int comparison = comparer_valeurs(col->type_colonne, col->donnees[index[j]], pivot);
        int should_swap = (direction_tri == ASC) ? (comparison <= 0) : (comparison > 0);

        if (should_swap) { // Échanger les indices si nécessaire
            i++;
            echanger_indexes(&index[i], &index[j]);
        }
    }

    echanger_indexes(&index[i + 1], &index[haut]); // Placer le pivot à la position correcte
    return i + 1;
}

/**
Rôle : Échanger deux indices
Paramètres :
    - a : Pointeur vers le premier indice
    - b : Pointeur vers le deuxième indice
Retour : Aucun car elle échange simplement les valeurs des index spécifiés.
*/
void echanger_indexes(unsigned long long *a, unsigned long long *b) {
    unsigned long long temp = *a;
    *a = *b;
    *b = temp;
}

/**
Rôle : Imprimer une colonne par index
Paramètres :
    - col : Pointeur vers la colonne
Retour : Aucun car elle imprime simplement les valeurs de la colonne par index.
*/
void imprimer_colonne_par_index(COLONNE *col) {
    if (col == NULL || col->index == NULL || col->index_valide == 0) {
        printf("La colonne ou l'index n'est pas initialisé ou n'est pas valide.\n");
        return;
    }

    char tampon[256];
    for (unsigned int i = 0; i < col->taille; i++) {
        convertir_valeur(col, col->index[i], tampon, sizeof(tampon));
        printf("[%llu] %s\n", col->index[i], tampon);
    }
}

/**
Rôle : Effacer l'index d'une colonne
Paramètres :
    - col : Pointeur vers la colonne
Retour : Aucun car elle efface simplement l'index de la colonne spécifiée.
*/
void effacer_index(COLONNE *col) {
    if (col == NULL || col->index == NULL) {
        return;
    }

    free(col->index); // Libérer la mémoire allouée pour l'index
    col->index = NULL;
    col->index_valide = 0;
}

/**
Rôle : Vérifier la validité de l'index d'une colonne
Paramètres :
    - col : Pointeur vers la colonne
Retour : 1 si l'index est valide, -1 s'il est invalide, 0 s'il n'existe pas
*/
int verifier_index(COLONNE *col) {
    if (col == NULL || col->index == NULL) {
        return 0;
    }

    return col->index_valide ? 1 : -1;
}

/**
Rôle : Mettre à jour l'index d'une colonne
Paramètres :
    - col : Pointeur vers la colonne
Retour : Aucun  car elle met à jour directement l'index de la colonne spécifiée.
*/
void mettre_a_jour_index(COLONNE *col) {
    if (col == NULL || col->donnees == NULL || col->taille == 0) {
        fprintf(stderr, "La colonne ou les donnees ne sont pas initialisees ou sont vides.\n");
        return;
    }

    if (col->index == NULL) {
        col->index = malloc(col->taille_max * sizeof(unsigned long long));
        if (col->index == NULL) {
            fprintf(stderr, "Echec de l'allocation de memoire pour le tableau d'index.\n");
            return;
        }
    }

    // Initialiser l'index
    for (unsigned int i = 0; i < col->taille; i++) {
        col->index[i] = i;
    }

    col->index_valide = 1;
}

/**
Rôle : Rechercher une valeur dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur à rechercher
Retour : 1 si la valeur est trouvée, 0 sinon
*/
int rechercher_valeur_dans_colonne(COLONNE *col, void *valeur) {
    if (col == NULL || col->index == NULL || col->index_valide == 0) {
        fprintf(stderr, "La colonne ou l'index n'est pas initialise ou n'est pas valide.\n");
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
