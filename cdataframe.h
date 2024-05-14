#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "column.h"

// Structure pour un dataframe
typedef struct dataframe {
    COLUMN **columns;         // Tableau de pointeurs vers COLUMN
    unsigned int column_count;  // Nombre de colonnes dans le dataframe
    unsigned int max_columns;   // Capacité maximale du tableau de colonnes
} DATAFRAME;

// Prototypes des fonctions pour gérer le dataframe
DATAFRAME *create_dataframe();
void fill_dataframe_from_user(DATAFRAME *df);
void hard_fill_dataframe(DATAFRAME *df, void **data, unsigned int num_rows, unsigned int num_columns, ENUM_TYPE *column_types, char **column_titles);
ENUM_TYPE analyse_type(const char *typeStr);
void *read_data_based_on_type(ENUM_TYPE type);
void free_dataframe(DATAFRAME *df);

// Prototypes des fonctions pour afficher le dataframe
void display_full_dataframe(DATAFRAME *df);
void display_dataframe_rows(DATAFRAME *df, unsigned int rows);
void display_dataframe_columns(DATAFRAME *df, unsigned int columns);

// Prototypes des fonctions pour les opérations usuelles
void add_row_to_dataframe(DATAFRAME *df, void **row_data);
void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index);
int add_column_to_dataframe(DATAFRAME *df, COLUMN *col);
void remove_column_from_dataframe(DATAFRAME *df, unsigned int index);
void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title);
int check_value_existence(DATAFRAME *df, void *value);
void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column);
void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value);
void display_column_names(DATAFRAME *df);

// Prototypes des fonctions pour l'analyse et les statistiques
void display_row_count(DATAFRAME *df);
void display_column_count(DATAFRAME *df);
int count_cells_equal_to(DATAFRAME *df, void *value);
int count_cells_greater_than(DATAFRAME *df, void *value);
int count_cells_less_than(DATAFRAME *df, void *value);

// Prototypes des nouvelles fonctions pour la gestion des fichiers CSV
DATAFRAME *load_from_csv(char *file_name, ENUM_TYPE *dftype, int size);
void save_into_csv(DATAFRAME *cdf, char *file_name);

#endif // CDATAFRAME_H
