#include "column.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Taille de réallocation lors de l'extension de la colonne
#define REALOC_SIZE 256

// Création d'une nouvelle colonne avec un type spécifié et un titre
COLUMN *creer_colonne(ENUM_TYPE type, char *titre) {
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));
    if (col == NULL) {
        fprintf(stderr, "Échec de l'allocation de mémoire pour la structure des colonnes.\n");
        return NULL;
    }

    col->titre = strdup(titre);
    if (col->titre == NULL) {
        free(col);
        fprintf(stderr, "Échec de l'allocation de mémoire pour le titre de la colonne.\n");
        return NULL;
    }

    col->taille = 0;             // Initialisation de la taille de la colonne à 0
    col->max_taille = 0;         // Initialisation de la capacité maximale de la colonne à 0
    col->type_colonne = type;   // Initialisation du type de données de la colonne
    col->donnees = NULL;          // Initialisation du tableau de données à NULL
    col->index = NULL;         // Initialisation de l'index à NULL

    return col; // Retour de la colonne nouvellement créée
}

// Insertion d'une valeur dans la colonne
int inserer_valeur(COLUMN *col, void *valeur) {
    if (col->taille >= col->max_taille) {
        size_t new_max_taille = col->max_taille == 0 ? REALOC_SIZE : col->max_taille + REALOC_SIZE;
        COL_TYPE **new_data = (COL_TYPE **)realloc(col->donnees, new_max_taille * sizeof(COL_TYPE *));
        if (!new_data) {
            fprintf(stderr, "La réallocation de mémoire a échoué.\n");
            return 0;
        }
        col->donnees = new_data;              // Mise à jour du tableau de données
        col->max_taille = new_max_taille;      // Mise à jour de la capacité maximale
    }

    void *new_valeur = NULL;
    switch (col->type_colonne) {    // Sélection du type de données de la colonne
        // Allocation de mémoire et copie de la valeur en fonction du type
        case UINT:
            new_valeur = malloc(sizeof(unsigned int));
            if (new_valeur) *(unsigned int *)new_valeur = *(unsigned int *)valeur;
            break;
        case INT:
            new_valeur = malloc(sizeof(int));
            if (new_valeur) *(int *)new_valeur = *(int *)valeur;
            break;
        case CHAR:
            new_valeur = malloc(sizeof(char));
            if (new_valeur) *(char *)new_valeur = *(char *)valeur;
            break;
        case FLOAT:
            new_valeur = malloc(sizeof(float));
            if (new_valeur) *(float *)new_valeur = *(float *)valeur;
            break;
        case DOUBLE:
            new_valeur = malloc(sizeof(double));
            if (new_valeur) *(double *)new_valeur = *(double *)valeur;
            break;
        case STRING:
            new_valeur = strdup((char *)valeur);
            break;
        case STRUCTURE:
            new_valeur = malloc(sizeof(CustomStructure));
            if (new_valeur) memcpy(new_valeur, valeur, sizeof(CustomStructure));
            break;
        default:
            fprintf(stderr, "Type non pris en charge pour l'insertion.\n");
            return 0;
    }
    // Vérification de l'allocation de la nouvelle valeur
    if (!new_valeur) {
        fprintf(stderr, "L'allocation de mémoire pour la nouvelle valeur a échoué.\n");
        return 0;
    }

    // Insertion de la nouvelle valeur dans le tableau de données
    col->donnees[col->taille++] = new_valeur;
    return 1;
}

// Suppression d'une colonne
void supprimer_colonne(COLUMN **col_ptr) {
    if (col_ptr == NULL || *col_ptr == NULL) {
        fprintf(stderr, "Tentative de suppression d'un pointeur de colonne nul.\n");
        return;
    }

    COLUMN *col = *col_ptr;

    for (unsigned int i = 0; i < col->taille; i++) {   // Parcours des données de la colonne
        if (col->donnees[i] != NULL) {
            if (col->type_colonne == STRING) {   // Vérification du type de données
                free(*(char **)(col->donnees[i]));   // Libération de la mémoire allouée pour une chaîne de caractères
            }
            free(col->donnees[i]);     // Libération de la mémoire allouée pour la valeur
        }
    }

    free(col->donnees);      // Libération de la mémoire allouée pour le tableau de données
    free(col->titre);     // Libération de la mémoire allouée pour le titre de la colonne
    free(col);

    *col_ptr = NULL;   // Réinitialisation du pointeur de colonne
}

// Conversion d'une structure personnalisée en chaîne de caractères
void CustomStructureToString(CustomStructure *cs, char *str, size_t taille) {
    if (cs == NULL || str == NULL) {
        snprintf(str, taille, "NULL");
        return;
    }
    // Formatage de la structure en chaîne de caractères
    snprintf(str, taille, "ID: %d, Value: %.2f, Description: %s", cs->id, cs->value, cs->description);
}

// Conversion de la valeur à un index spécifié en chaîne de caractères
void convertir_valeur(COLUMN *col, unsigned long long int index, char *str, int taille) {
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
            CustomStructureToString((CustomStructure *)(col->donnees[index]), str, taille);
            break;
        default:
            snprintf(str, taille, "Type non pris en charge.");
            break;
    }
}

// Impression du contenu de la colonne
void imprimer_colonne(COLUMN *col) {
    if (col == NULL) {
        printf("Tentavive d'impression d'une colonne nulle.\n");
        return;
    }

    printf("Colonne '%s':\n", col->titre);  // Affichage du titre de la colonne
    char buffer[256];  // Tampon pour stocker la valeur convertie en chaîne de caractères

    for (unsigned int i = 0; i < col->taille; i++) {
        convertir_valeur(col, i, buffer, sizeof(buffer)); // Conversion de la valeur en chaîne de caractères
        printf("[%u] %s\n", i, buffer); // Affichage de l'indice et de la valeur
    }
}

// Comptage des occurrences d'une valeur dans la colonne
int compter_occurrences(COLUMN *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) == 0) {  // Comparaison de la valeur
            count++;  // Incrémentation du compteur si la valeur correspond
        }
    }
    return count;  // Retour du nombre d'occurrences
}

// Récupération de la valeur à un index spécifié dans la colonne
void *obtenir_valeur_a(COLUMN *col, unsigned int index) {
    if (col == NULL || index >= col->taille) return NULL;  // Vérification de la colonne et de l'index
    return col->donnees[index];  // Retour de la valeur à l'index spécifié
}

// Comptage des valeurs supérieures à une valeur donnée dans la colonne
int compter_superieur_a(COLUMN *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) > 0) {
            count++;  // Incrémentation du compteur si la valeur est supérieure
        }
    }
    return count; // Retour du nombre de valeurs supérieures
}

// Comptage des valeurs inférieures à une valeur donnée dans la colonne
int compter_inferieur_a(COLUMN *col, void *valeur) {
    if (col == NULL || valeur == NULL) return 0;

    int count = 0;
    for (unsigned int i = 0; i < col->taille; i++) {
        if (comparer_valeurs(col->type_colonne, col->donnees[i], valeur) < 0) {
            count++;  // Incrémentation du compteur si la valeur est inférieure
        }
    }
    return count;  // Retour du nombre de valeurs inférieures
}

// Comptage des valeurs égales à une valeur donnée dans la colonne
int compter_egal_a(COLUMN *col, void *valeur) {
    return compter_occurrences(col, valeur);  // Utilisation de la fonction de comptage d'occurrences
}

// Comparaison de deux valeurs en fonction de leur type
int comparer_valeurs(ENUM_TYPE type, void *data1, void *data2) {
    if (data1 == NULL || data2 == NULL) return -1;

    switch (type) {  // Sélection du type de données
        // Comparaison des valeurs en fonction de leur type
        case INT:
            return (*(int *)data1 > *(int *)data2) - (*(int *)data1 < *(int *)data2);
        case FLOAT:
            return (*(float *)data1 > *(float *)data2) - (*(float *)data1 < *(float *)data2);
        case DOUBLE:
            return (*(double *)data1 > *(double *)data2) - (*(double *)data1 < *(double *)data2);
        case CHAR:
            return (*(char *)data1 > *(char *)data2) - (*(char *)data1 < *(char *)data2);
        case STRING:
            return strcmp((char *)data1, (char *)data2);   // Utilisation de la fonction strcmp pour les chaînes de caractères
        case STRUCTURE:
            return ((CustomStructure *)data1)->value > ((CustomStructure *)data2)->value ? 1 :
                   ((CustomStructure *)data1)->value < ((CustomStructure *)data2)->value ? -1 : 0;
        default:
            fprintf(stderr, "Type non pris en charge pour comparaison.\n");
            return -1;
    }
}
