#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>

// Structure personnalisée pour des données complexes
typedef struct CustomStructure {
    int id;
    double value;
    char description[100];
} CustomStructure;

// Types énumérés pour les types de colonnes
enum enum_type {
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type ENUM_TYPE;

// Union pour stocker différents types de données dans une colonne
union column_type {
    unsigned int uint_value;
    unsigned int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type COL_TYPE;

// Structure de la colonne
struct column {
    char *titre;
    unsigned int taille;
    unsigned int max_taille;
    ENUM_TYPE type_colonne;
    COL_TYPE **donnees;
    unsigned long long int *index;
    int valid_index;
    int direction_tri;
};
typedef struct column COLUMN;

// Prototypes des fonctions pour gérer les colonnes
COLUMN *creer_colonne(ENUM_TYPE type, char *titre);
int inserer_valeur(COLUMN *col, void *valeur);
void supprimer_colonne(COLUMN **col);
void convertir_valeur(COLUMN *col, unsigned long long int index, char *str, int taille);
void imprimer_colonne(COLUMN *col);

int compter_occurrences(COLUMN *col, void *valeur);
void *obtenir_valeur_a(COLUMN *col, unsigned int index);
int compter_superieur_a(COLUMN *col, void *valeur);
int compter_inferieur_a(COLUMN *col, void *valeur);
int compter_egal_a(COLUMN *col, void *valeur);
int comparer_valeurs(ENUM_TYPE type, void *data1, void *data2);

#endif // COLUMN_H
