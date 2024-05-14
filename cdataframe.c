#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define MAX_LINE_LENGTH 1024


DATAFRAME *create_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME));
    if (!df) {
        fprintf(stderr, "L'allocation de mémoire a échoué pour la dataframe\n");
        return NULL;
    }
    df->columns = NULL;
    df->column_count = 0;
    df->max_columns = 0;
    return df;
}


void fill_dataframe_from_user(DATAFRAME *df) {
    if (!df) {
        fprintf(stderr, "La dataframe est NULL.\n");
        return;
    }

    printf("Entrez le nombre de colonnes: ");
    int num_columns;
    scanf("%d", &num_columns);
    df->columns = (COLUMN **)malloc(num_columns * sizeof(COLUMN *));
    df->max_columns = num_columns;
    df->column_count = 0;

    for (int i = 0; i < num_columns; i++) {
        char type[20], name[100];
        printf("Entrez le type et le nom de la colonne %d (par exemple, INT Age): ", i + 1);
        scanf("%s %s", type, name);
        ENUM_TYPE col_type = analyse_type(type);
        COLUMN *new_column = create_column(col_type, name);
        if (!new_column) {
            continue;
        }
        add_column_to_dataframe(df, new_column);

        printf("Entrez le nombre de lignes pour la colonne '%s': ", name);
        int num_rows;
        scanf("%d", &num_rows);
        for (int j = 0; j < num_rows; j++) {
            printf("Saisir les données pour la ligne %d: ", j + 1);
            void *data = read_data_based_on_type(col_type);
            insert_value(new_column, data);
        }
    }
}


ENUM_TYPE analyse_type(const char *typeStr) {
    if (strcmp(typeStr, "INT") == 0) return INT;
    else if (strcmp(typeStr, "FLOAT") == 0) return FLOAT;
    else if (strcmp(typeStr, "DOUBLE") == 0) return DOUBLE;
    else if (strcmp(typeStr, "CHAR") == 0) return CHAR;
    else if (strcmp(typeStr, "STRING") == 0) return STRING;
    else if (strcmp(typeStr, "STRUCTURE") == 0) return STRUCTURE;
    return -1;
}



void hard_fill_dataframe(DATAFRAME *df, void **data, unsigned int num_rows, unsigned int num_columns, ENUM_TYPE *column_types, char **column_titles) {
    if (!df || !data || !column_types || !column_titles) {
        fprintf(stderr, "Arguments non valides pour le remplissage en dur de la dataframe.\n");
        return;
    }

    df->columns = (COLUMN **)malloc(num_columns * sizeof(COLUMN *));
    if (!df->columns) {
        fprintf(stderr, "L'allocation de mémoire a échoué pour les colonnes.\n");
        return;
    }
    df->column_count = num_columns;
    df->max_columns = num_columns;

    for (unsigned int i = 0; i < num_columns; i++) {
        COLUMN *col = create_column(column_types[i], column_titles[i]);
        if (!col) {
            fprintf(stderr, "Échec de la création de la colonne '%s'.\n", column_titles[i]);
            continue;
        }

        add_column_to_dataframe(df, col);

        for (unsigned int j = 0; j < num_rows; j++) {
            void *value = ((void **)data[i])[j];
            if (!insert_value(col, value)) {
                fprintf(stderr, "Échec de l'insertion de la valeur dans la colonne '%s'.\n", column_titles[i]);
            }
        }
    }
}


void *read_data_based_on_type(ENUM_TYPE type) {
    void *data = NULL;
    switch (type) {
        case INT: {
            int *value = malloc(sizeof(int));
            printf("Saisissez un entier: ");
            scanf("%d", value);
            data = value;
            break;
        }
        case FLOAT: {
            float *value = malloc(sizeof(float));
            printf("Entrez un flottant: ");
            scanf("%f", value);
            data = value;
            break;
        }
        case DOUBLE: {
            double *value = malloc(sizeof(double));
            printf("Entrez un double: ");
            scanf("%lf", value);
            data = value;
            break;
        }
        case CHAR: {
            char *value = malloc(sizeof(char));
            printf("Entrez un caractère: ");
            scanf(" %c", value);
            data = value;
            break;
        }
        case STRING: {
            char *value = malloc(256);
            printf("Entrez une chaîne de caractère: ");
            scanf("%255s", value);
            data = value;
            break;
        }
        case STRUCTURE: {
            CustomStructure *value = malloc(sizeof(CustomStructure));
            printf("Entrez les valeurs de la structure (id, valeur): ");
            scanf("%d %lf", &value->id, &value->value);
            data = value;
            break;
        }
        default:
            printf("Type de données non pris en charge.\n");
            break;
    }
    return data;
}


void free_dataframe(DATAFRAME *df) {
    if (df == NULL) return;


    for (unsigned int i = 0; i < df->column_count; i++) {
        if (df->columns[i] != NULL) {
            delete_column(&df->columns[i]);
        }
    }


    free(df->columns);


    free(df);
}


int add_column_to_dataframe(DATAFRAME *df, COLUMN *col) {
    if (df == NULL || col == NULL) return -1;
    if (df->column_count == df->max_columns) {
        size_t new_size = df->max_columns == 0 ? 1 : df->max_columns * 2;
        COLUMN **new_cols = realloc(df->columns, new_size * sizeof(COLUMN *));
        if (!new_cols) return -1;
        df->columns = new_cols;
        df->max_columns = new_size;
    }
    df->columns[df->column_count++] = col;
    return 0;
}


void display_full_dataframe(DATAFRAME *df) {
    if (!df || !df->columns) {
        printf("La dataframe est vide ou non initialisée.\n");
        return;
    }
    printf("La dataframe contient %u colonnes:\n", df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Colonne %d: %s\n", i + 1, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}

void display_dataframe_rows(DATAFRAME *df, unsigned int rows) {
    if (!df || !df->columns) {
        printf("La dataframe est vide ou non initialisée\n");
        return;
    }
    printf("Afficher jusqu'à %u lignes de chacune des colonnes %u:\n", rows, df->column_count);
    for (unsigned int i = 0; i < df->column_count; i++) {
        COLUMN *col = df->columns[i];
        printf("Colonne %d (%s):\n", i + 1, col->title);


        for (unsigned int j = 0; j < rows && j < col->size; j++) {
            switch (col->column_type) {
                case INT:
                    printf("%d: %d\n", j + 1, *((int*)col->data[j]));
                    break;
                case FLOAT:
                    printf("%d: %f\n", j + 1, *((float*)col->data[j]));
                    break;
                case DOUBLE:
                    printf("%d: %lf\n", j + 1, *((double*)col->data[j]));
                    break;
                case STRING:
                    printf("%d: %s\n", j + 1, (char*)col->data[j]);
                    break;
                case STRUCTURE:
                    CustomStructure *cs = (CustomStructure*)col->data[j];
                    printf("%d: ID = %d, Value = %.2f\n", j + 1, cs->id, cs->value);
                    break;
                default:
                    printf("%d: Type de données non géré.\n", j + 1);
                    break;
            }
        }
    }
}



void add_row_to_dataframe(DATAFRAME *df, void **row_data) {
    if (!df || !df->columns) {
        printf("La dataframe n'est pas correctement initialisée.\n");
        return;
    }

    for (unsigned int i = 0; i < df->column_count; i++) {
        if (!insert_value(df->columns[i], row_data[i])) {
            printf("Échec de l'insertion des données dans la colonne %u.\n", i + 1);
        }
    }
}


void delete_row_from_dataframe(DATAFRAME *df, unsigned int row_index) {
    if (!df) {
        printf("La dataframe n'est pas initialisée.\n");
        return;
    }


    for (unsigned int i = 0; i < df->column_count; i++) {
        if (row_index >= df->columns[i]->size) {
            printf("L'index de la ligne %u est hors limites pour la colonne %u.\n", row_index, i + 1);
            return;
        }
    }


    for (unsigned int i = 0; i < df->column_count; i++) {
        df->columns[i]->size--;
    }
}


void display_dataframe_columns(DATAFRAME *df, unsigned int columns) {
    if (!df || !df->columns) {
        printf("La dataframe est vide ou non initialisée.\n");
        return;
    }
    printf("Affichage des %u premières colonnes sur %u colonnes totales:\n", columns, df->column_count);
    for (unsigned int i = 0; i < columns && i < df->column_count; i++) {
        printf("Colonne %u (%s):\n", i + 1, df->columns[i]->title);
        print_col(df->columns[i]);
    }
}


