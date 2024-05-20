/*
Nom du projet : CDataFrame
Auteurs : Samy AIT ALLA, Sajin Sivasaranam
Rôle : Implémentation des fonctions de gestion des colonnes
*/

#include "column.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Taille de réallocation lors de l'extension de la colonne
#define TAILLE_REALLOC 256

/**
Rôle : Créer une nouvelle colonne avec un type spécifié et un titre
Paramètres :
    - type : Type des données dans la colonne (TYPE_ENUM)
    - titre : Titre de la colonne
Retour : Pointeur vers la colonne nouvellement créée
*/
COLONNE *creer_colonne(TYPE_ENUM type, char *titre) {
    COLONNE *col = (COLONNE *)malloc(sizeof(COLONNE)); // Allocation de mémoire pour la colonne
    if (col == NULL) {
        fprintf(stderr, "Echec de l'allocation de memoire pour la structure des colonnes.\n");
        return NULL;
    }

    col->titre = strdup(titre); // Allocation de mémoire et copie du titre
    if (col->titre == NULL) {
        free(col); // Libération de la mémoire en cas d'échec
        fprintf(stderr, "Échec de l'allocation de mémoire pour le titre de la colonne.\n");
        return NULL;
    }

    col->taille = 0;             // Initialisation de la taille de la colonne à 0
    col->taille_max = 0;         // Initialisation de la capacité maximale de la colonne à 0
    col->type_colonne = type;    // Initialisation du type de données de la colonne
    col->donnees = NULL;         // Initialisation du tableau de données à NULL
    col->index = NULL;           // Initialisation de l'index à NULL

    return col; // Retour de la colonne nouvellement créée
}

/**
Rôle : Insérer une valeur dans la colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur à insérer
Retour : 1 si succès, 0 en cas d'échec
*/
int inserer_valeur(COLONNE *col, void *valeur) {
    // Vérifier si une réallocation est nécessaire
    if (col->taille >= col->taille_max) {
        size_t nouvelle_taille_max = col->taille_max == 0 ? TAILLE_REALLOC : col->taille_max + TAILLE_REALLOC;
        TYPE_COLONNE **nouvelles_donnees = (TYPE_COLONNE **)realloc(col->donnees, nouvelle_taille_max * sizeof(TYPE_COLONNE *));
        if (!nouvelles_donnees) {
            fprintf(stderr, "La reallocation de memoire a echoue.\n");
            return 0;
        }
        col->donnees = nouvelles_donnees;              // Mise à jour du tableau de données
        col->taille_max = nouvelle_taille_max;      // Mise à jour de la capacité maximale
    }

    void *nouvelle_valeur = NULL;
    switch (col->type_colonne) {    // Sélection du type de données de la colonne
        // Allocation de mémoire et copie de la valeur en fonction du type
        case UINT:
            nouvelle_valeur = malloc(sizeof(unsigned int));
            if (nouvelle_valeur) *(unsigned int *)nouvelle_valeur = *(unsigned int *)valeur;
            break;
        case INT:
            nouvelle_valeur = malloc(sizeof(int));
            if (nouvelle_valeur) *(int *)nouvelle_valeur = *(int *)valeur;
            break;
        case CHAR:
            nouvelle_valeur = malloc(sizeof(char));
            if (nouvelle_valeur) *(char *)nouvelle_valeur = *(char *)valeur;
            break;
        case FLOAT:
            nouvelle_valeur = malloc(sizeof(float));
            if (nouvelle_valeur) *(float *)nouvelle_valeur = *(float *)valeur;
            break;
        case DOUBLE:
            nouvelle_valeur = malloc(sizeof(double));
            if (nouvelle_valeur) *(double *)nouvelle_valeur = *(double *)valeur;
            break;
        case STRING:
            nouvelle_valeur = strdup((char *)valeur);
            break;
        case STRUCTURE:
            nouvelle_valeur = malloc(sizeof(StructurePersonnalisee));
            if (nouvelle_valeur) memcpy(nouvelle_valeur, valeur, sizeof(StructurePersonnalisee));
            break;
        default:
            fprintf(stderr, "Type non pris en charge pour l'insertion.\n");
            return 0;
    }

    // Vérification de l'allocation de la nouvelle valeur
    if (!nouvelle_valeur) {
        fprintf(stderr, "L'allocation de memoire pour la nouvelle valeur a echoue.\n");
        return 0;
    }

    // Insertion de la nouvelle valeur dans le tableau de données
    col->donnees[col->taille++] = nouvelle_valeur;
    return 1;
}

/**
Rôle : Supprimer une colonne
Paramètres :
    - col_ptr : Pointeur vers le pointeur de la colonne à supprimer
Retour : Aucun car elle libère simplement la mémoire allouée pour la colonne spécifiée.
*/
void supprimer_colonne(COLONNE **col_ptr) {
    if (col_ptr == NULL || *col_ptr == NULL) {
        fprintf(stderr, "Tentative de suppression d'un pointeur de colonne nul.\n");
        return;
    }

    COLONNE *col = *col_ptr;

    // Parcourir et libérer les données de la colonne
    for (unsigned int i = 0; i < col->taille; i++) {
        if (col->donnees[i] != NULL) {
            if (col->type_colonne == STRING) {
                free(*(char **)(col->donnees[i]));
            }
            free(col->donnees[i]);
        }
    }

    free(col->donnees);      // Libérer la mémoire allouée pour le tableau de données
    free(col->titre);        // Libérer la mémoire allouée pour le titre de la colonne
    free(col);               // Libérer la mémoire de la colonne elle-même

    *col_ptr = NULL;         // Réinitialiser le pointeur de colonne
}

/**
Rôle : Convertir une structure personnalisée en chaîne de caractères
Paramètres :
    - sp : Pointeur vers la structure personnalisée
    - str : Tampon de chaîne de caractères
    - taille : Taille du tampon
Retour : Aucun car elle convertit directement une structure personnalisée en chaîne de caractères.
*/
void StructurePersonnaliseeEnString(StructurePersonnalisee *sp, char *str, size_t taille) {
    if (sp == NULL || str == NULL) {
        snprintf(str, taille, "NULL");
        return;
    }
    // Formatage de la structure en chaîne de caractères
    snprintf(str, taille, "ID: %d, Valeur: %.2f, Description: %s", sp->identifiant, sp->valeur, sp->description);
}

/**
Rôle : Convertir la valeur à un index spécifié en chaîne de caractères
Paramètres :
    - col : Pointeur vers la colonne
    - index : Index de la valeur à convertir
    - str : Tampon de chaîne de caractères
    - taille : Taille du tampon
Retour : Aucun car elle convertit simplement la valeur à un index spécifié en chaîne de caractères.
*/
void convertir_valeur(COLONNE *col, unsigned long long int index, char *str, int taille) {
    if (col == NULL || str == NULL) {
        snprintf(str, taille, "NULL");
        return;
    }
    if (index >= col->taille || col->donnees[index] == NULL) {
        snprintf(str, taille, "NULL");
        return;
    }

    switch (col->type_colonne) {   // Sélection du type de données de la colonne
        // Conversion de la valeur en fonction du type de données
        case UINT:
            snprintf(str, taille, "%u", *(unsigned int *)(col->donnees[index]));
            break;
        case INT:
            snprintf(str, taille, "%d", *(int *)(col->donnees[index]));
            break;
        case CHAR:
            snprintf(str, taille, "%c", *(char *)(col->donnees[index]));
            break;
        case FLOAT:
            snprintf(str, taille, "%.2f", *(float *)(col->donnees[index]));
            break;
        case DOUBLE:
            snprintf(str, taille, "%.2lf", *(double *)(col->donnees[index]));
            break;
        case STRING:
            snprintf(str, taille, "%s", (char *)(col->donnees[index]));
            break;
        case STRUCTURE:
            StructurePersonnaliseeEnString((StructurePersonnalisee *)(col->donnees[index]), str, taille);
            break;
        default:
            snprintf(str, taille, "Type non pris en charge.");
            break;
    }
}

/**
Rôle : Imprimer le contenu d'une colonne
Paramètres :
    - col : Pointeur vers la colonne
Retour : Aucun car elle imprime simplement le contenu de la colonne.
*/
void imprimer_colonne(COLONNE *col) {
    if (col == NULL) {
        printf("Tentative d'impression d'une colonne nulle.\n");
        return;
    }

    printf("Colonne '%s':\n", col->titre);  // Afficher le titre de la colonne
    char tampon[256];  // Tampon pour stocker la valeur convertie en chaîne de caractères

    // Parcourir et afficher les valeurs de la colonne
    for (unsigned int i = 0; i < col->taille; i++) {
        convertir_valeur(col, i, tampon, sizeof(tampon)); // Conversion de la valeur en chaîne de caractères
        printf("[%u] %s\n", i, tampon); // Affichage de l'indice et de la valeur
    }
}

/**
Rôle : Compter le nombre d'occurrences d'une valeur dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur à compter
Retour : Nombre d'occurrences de la valeur dans la colonne
*/
int compter_occurrences(COLONNE *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int compteur = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) == 0) {  // Comparaison de la valeur
            compteur++;  // Incrémentation du compteur si la valeur correspond
        }
    }
    return compteur;  // Retour du nombre d'occurrences
}

/**
Rôle : Obtenir la valeur à un index spécifié dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - index : Index de la valeur à obtenir
Retour : Pointeur vers la valeur à l'index spécifié
*/
void *obtenir_valeur_a(COLONNE *col, unsigned int index) {
    if (col == NULL || index >= col->taille) return NULL;  // Vérification de la colonne et de l'index
    return col->donnees[index];  // Retour de la valeur à l'index spécifié
}

/**
Rôle : Compter le nombre de valeurs supérieures à une valeur donnée dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur de comparaison
Retour : Nombre de valeurs supérieures à la valeur donnée
*/
int compter_superieur_a(COLONNE *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int compteur = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) > 0) {
            compteur++;  // Incrémentation du compteur si la valeur est supérieure
        }
    }
    return compteur; // Retour du nombre de valeurs supérieures
}

/**
Rôle : Compter le nombre de valeurs inférieures à une valeur donnée dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur de comparaison
Retour : Nombre de valeurs inférieures à la valeur donnée
*/
int compter_inferieur_a(COLONNE *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int compteur = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) < 0) {
            compteur++;  // Incrémentation du compteur si la valeur est inférieure
        }
    }
    return compteur;  // Retour du nombre de valeurs inférieures
}

/**
Rôle : Compter le nombre de valeurs égales à une valeur donnée dans une colonne
Paramètres :
    - col : Pointeur vers la colonne
    - valeur : Pointeur vers la valeur de comparaison
Retour : Nombre de valeurs égales à la valeur donnée
*/
int compter_egal_a(COLONNE *col, void *valeur) {
    return compter_occurrences(col, valeur);  // Utilisation de la fonction de comptage d'occurrences
}

/**
Rôle : Comparer deux valeurs en fonction de leur type
Paramètres :
    - type : Type des valeurs à comparer
    - donnee1 : Pointeur vers la première valeur
    - donnee2 : Pointeur vers la deuxième valeur
Retour : 0 si les valeurs sont égales, >0 si donnee1 > donnee2, <0 si donnee1 < donnee2
*/
int comparer_valeurs(TYPE_ENUM type, void *donnee1, void *donnee2) {
    if (donnee1 == NULL || donnee2 == NULL) return -1;

    switch (type) {  // Sélection du type de données
        // Comparaison des valeurs en fonction de leur type
        case INT:
            return (*(int *)donnee1 > *(int *)donnee2) - (*(int *)donnee1 < *(int *)donnee2);
        case FLOAT:
            return (*(float *)donnee1 > *(float *)donnee2) - (*(float *)donnee1 < *(float *)donnee2);
        case DOUBLE:
            return (*(double *)donnee1 > *(double *)donnee2) - (*(double *)donnee1 < *(double *)donnee2);
        case CHAR:
            return (*(char *)donnee1 > *(char *)donnee2) - (*(char *)donnee1 < *(char *)donnee2);
        case STRING:
            return strcmp((char *)donnee1, (char *)donnee2);   // Utilisation de la fonction strcmp pour les chaînes de caractères
        case STRUCTURE:
            return ((StructurePersonnalisee *)donnee1)->valeur > ((StructurePersonnalisee *)donnee2)->valeur ? 1 :
                   ((StructurePersonnalisee *)donnee1)->valeur < ((StructurePersonnalisee *)donnee2)->valeur ? -1 : 0;
        default:
            fprintf(stderr, "Type non pris en charge pour comparaison.\n");
            return -1;
    }
}
