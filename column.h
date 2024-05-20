#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>

// Structure personnalisée pour des données complexes
typedef struct StructurePersonnalisee {
    int identifiant;
    double valeur;
    char description[100];
} StructurePersonnalisee;

// Types énumérés pour les types de colonnes
enum type_enum {
    VALEURNULLE = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum type_enum TYPE_ENUM;

// Union pour stocker différents types de données dans une colonne
union type_colonne {
    unsigned int valeur_uint;
    int valeur_int;
    char valeur_char;
    float valeur_float;
    double valeur_double;
    char* valeur_string;
    void* valeur_structure;
};
typedef union type_colonne TYPE_COLONNE;

// Structure de la colonne
struct colonne {
    char *titre;
    unsigned int taille;
    unsigned int taille_max;
    TYPE_ENUM type_colonne;
    TYPE_COLONNE **donnees;
    unsigned long long int *index;
    int index_valide;
    int direction_tri;
};
typedef struct colonne COLONNE;

// Prototypes des fonctions pour gérer les colonnes
COLONNE *creer_colonne(TYPE_ENUM type, char *titre);
int inserer_valeur(COLONNE *col, void *valeur);
void supprimer_colonne(COLONNE **col);
void convertir_valeur(COLONNE *col, unsigned long long int index, char *str, int taille);
void imprimer_colonne(COLONNE *col);

int compter_occurrences(COLONNE *col, void *valeur);
void *obtenir_valeur_a(COLONNE *col, unsigned int index);
int compter_superieur_a(COLONNE *col, void *valeur);
int compter_inferieur_a(COLONNE *col, void *valeur);
int compter_egal_a(COLONNE *col, void *valeur);
int comparer_valeurs(TYPE_ENUM type, void *donnee1, void *donnee2);

#endif // COLUMN_H
