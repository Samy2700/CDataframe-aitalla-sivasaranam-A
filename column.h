#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>


typedef struct CustomStructure {     //Créarion d'un structure personnalisé, avec un idétifiant, valeur et description 
    int id;
    double value;
    char description[100];
} CustomStructure;


enum enum_type {                               // Enumération des types de données possibles
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type ENUM_TYPE;


union column_type {                   //Union représentant les types de variables avec leurs de collones
    unsigned int uint_value;
    unsigned int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type COL_TYPE;


struct column {                          //Création d'une structure de colonne
    char *title;                                  // Titre de la colonne
    unsigned int size;                            // Taille actuelle de la colonne
    unsigned int max_size;                        // Taille max de la colonne
    ENUM_TYPE column_type;                        // Type des données de la colonne                 
    COL_TYPE **data;                              // Type des données de la colonne
    unsigned long long int *index;                // Index pour le tri
    int valid_index;                              // Indiquation de la validation de l'index
    int sort_dir;                                 // Direction de tri
};
typedef struct column COLUMN;

// Fonctions pour la manipulation des colonnes
// Crée une nouvelle colonne avec un type et un titre donnés
COLUMN *create_column(ENUM_TYPE type, char *title);

// Insère une valeur dans la colonne
int insert_value(COLUMN *col, void *value);

// Supprime une colonne et libère la mémoire associée
void delete_column(COLUMN **col);

// Convertit la valeur d'une colonne à un index donné en chaîne de caractères
void convert_value(COLUMN *col, unsigned long long int index, char *str, int size);

// Affiche les valeurs de la colonne
void print_col(COLUMN *col);

//Compte le nombre d'occurences de la colonnes
int count_occurrences(COLUMN *col, void *value);

// Obtient la valeur à un index donné dans la colonne
void *get_value_at(COLUMN *col, unsigned int index);

// Compte le nombre de valeurs supérieures, inférieures et équales à une valeur donnée dans la colonne
int count_greater_than(COLUMN *col, void *value);
int count_less_than(COLUMN *col, void *value);
int count_equal_to(COLUMN *col, void *value);

// Compare deux valeurs en fonction de leur type
int compare_values(ENUM_TYPE type, void *data1, void *data2);


#endif // COLUMN_H
