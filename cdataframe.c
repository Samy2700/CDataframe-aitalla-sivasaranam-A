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



void hardcode_dataframe(DATAFRAME *df) {
    // Ajouter la première colonne d'entiers
    COLUMN *col1 = create_column(INT, "Colonne d'Entiers");
    if (add_column_to_dataframe(df, col1) == 0) {
        int data1[] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            insert_value(col1, &data1[i]);
        }
    }

    // Ajouter la deuxième colonne de flottants
    COLUMN *col2 = create_column(FLOAT, "Colonne de Flottants");
    if (add_column_to_dataframe(df, col2) == 0) {
        float data2[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        for (int i = 0; i < 5; i++) {
            insert_value(col2, &data2[i]);
        }
    }

    // Ajouter la troisième colonne de chaînes de caractères
    COLUMN *col3 = create_column(STRING, "Colonne de Chaînes");
    if (add_column_to_dataframe(df, col3) == 0) {
        char *data3[] = {"Un", "Deux", "Trois", "Quatre", "Cinq"};
        for (int i = 0; i < 5; i++) {
            insert_value(col3, data3[i]);
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


void remove_column_from_dataframe(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->column_count) {
        printf("Index de colonne invalide ou dataframe vide.\n");
        return;
    }

    delete_column(&df->columns[index]);


    for (unsigned int i = index; i < df->column_count - 1; i++) {
        df->columns[i] = df->columns[i + 1];
    }
    df->column_count--;


    if (df->column_count > 0 && df->column_count == df->max_columns / 4) {
        COLUMN **new_columns = realloc(df->columns, (df->max_columns / 2) * sizeof(COLUMN *));
        if (new_columns) {
            df->columns = new_columns;
            df->max_columns /= 2;
        }
    }
}



void rename_column_title(DATAFRAME *df, unsigned int column_index, const char *new_title) {
    if (!df || column_index >= df->column_count || !new_title) {
        printf("Index de colonne ou nouveau titre invalide.\n");
        return;
    }
    char *new_title_copy = strdup(new_title);
    if (!new_title_copy) {
        printf("Échec de l'allocation de mémoire pour le nouveau titre.\n");
        return;
    }
    free(df->columns[column_index]->title);
    df->columns[column_index]->title = new_title_copy;
}


int check_value_existence(DATAFRAME *df, void *value) {
    if (!df || !value) {
        printf("Dataframe ou valeur invalide.\n");
        return 0;
    }
    for (unsigned int i = 0; i < df->column_count; i++) {
        for (unsigned int j = 0; j < df->columns[i]->size; j++) {
            if (compare_values(df->columns[i]->column_type, df->columns[i]->data[j], value) == 0) {
                return 1;
            }
        }
    }
    return 0;
}



void *get_cell_value(DATAFRAME *df, unsigned int row, unsigned int column) {
    if (df == NULL || column >= df->column_count || row >= df->columns[column]->size) {
        printf("Index de ligne ou de colonne non valide.\n");
        return NULL;
    }
    return df->columns[column]->data[row];
}


void set_cell_value(DATAFRAME *df, unsigned int row, unsigned int column, void *value) {
    if (!df || column >= df->column_count || row >= df->columns[column]->size || !value) {
        printf("Index de ligne ou de colonne non valide, ou valeur nulle fournie.\n");
        return;
    }

    if (df->columns[column]->column_type == STRING) {
        free(df->columns[column]->data[row]);
    }

    switch (df->columns[column]->column_type) {
        case INT:
            *(int *)(df->columns[column]->data[row]) = *(int *)value;
            break;
        case FLOAT:
            *(float *)(df->columns[column]->data[row]) = *(float *)value;
            break;
        case DOUBLE:
            *(double *)(df->columns[column]->data[row]) = *(double *)value;
            break;
        case STRING:
            df->columns[column]->data[row] = strdup((char *)value);
            break;
        case STRUCTURE:
            df->columns[column]->data[row] = malloc(sizeof(CustomStructure));
            if (df->columns[column]->data[row]) {
                memcpy(df->columns[column]->data[row], value, sizeof(CustomStructure));
            } else {
                printf("L'allocation de mémoire a échoué pour la structure.\n");
            }
            break;
        default:
            printf("Type de données non géré.\n");
            break;
    }
}



void display_column_names(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe n'est pas initialisée.\n");
        return;
    }
    printf("Noms de colonnes:\n");
    for (unsigned int i = 0; i < df->column_count; i++) {
        printf("Colonne %u: %s\n", i + 1, df->columns[i]->title);
    }
}


void display_row_count(DATAFRAME *df) {
    if (!df || df->column_count == 0 || !df->columns[0]) {
        printf("La dataframe est vide ou mal initialisée.\n");
    } else {
        printf("Nombre de lignes: %u\n", df->columns[0]->size);
    }
}

void display_column_count(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe est vide ou mal initialisée.\n");
    } else {
        printf("Nombre de colonnes: %u\n", df->column_count);
    }
}

int count_cells_equal_to(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_equal_to(df->columns[i], value);
    }
    return count;
}

int count_cells_greater_than(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_greater_than(df->columns[i], value);
    }
    return count;
}

int count_cells_less_than(DATAFRAME *df, void *value) {
    int count = 0;
    for (unsigned int i = 0; i < df->column_count; i++) {
        count += count_less_than(df->columns[i], value);
    }
    return count;
}



DATAFRAME *load_from_csv(char *file_name, ENUM_TYPE *dftype, int size) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        fprintf(stderr, "Impossible d'ouvrir le fichier CSV: %s\n", file_name);
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    char *token;
    int num_columns = size;


    if (fgets(line, sizeof(line), file)) {
        token = strtok(line, ",");
        for (int i = 0; i < num_columns; i++) {
            if (token != NULL) {
                token = strtok(NULL, ",");
            }
        }
    } else {
        fprintf(stderr, "Échec de la lecture de la ligne d'en-tête du fichier CSV.\n");
        fclose(file);
        return NULL;
    }


    rewind(file);


    DATAFRAME *df = create_dataframe();
    df->columns = (COLUMN **)malloc(num_columns * sizeof(COLUMN *));
    if (!df->columns) {
        fprintf(stderr, "L'allocation de mémoire a échoué pour les colonnes.\n");
        fclose(file);
        return NULL;
    }
    df->column_count = num_columns;
    df->max_columns = num_columns;


    fgets(line, sizeof(line), file);
    token = strtok(line, ",");
    for (int i = 0; i < num_columns; i++) {
        COLUMN *col = create_column(dftype[i], token);
        df->columns[i] = col;
        token = strtok(NULL, ",");
    }


    while (fgets(line, sizeof(line), file)) {
        void **row_data = malloc(num_columns * sizeof(void *));
        token = strtok(line, ",");

        for (int i = 0; i < num_columns; i++) {
            switch (dftype[i]) {
                case INT: {
                    int *value = malloc(sizeof(int));
                    *value = atoi(token);
                    row_data[i] = value;
                    break;
                }
                case FLOAT: {
                    float *value = malloc(sizeof(float));
                    *value = atof(token);
                    row_data[i] = value;
                    break;
                }
                case DOUBLE: {
                    double *value = malloc(sizeof(double));
                    *value = atof(token);
                    row_data[i] = value;
                    break;
                }
                case CHAR: {
                    char *value = malloc(sizeof(char));
                    *value = *token;
                    row_data[i] = value;
                    break;
                }
                case STRING: {
                    row_data[i] = strdup(token);
                    break;
                }
                case STRUCTURE: {
                    CustomStructure *value = malloc(sizeof(CustomStructure));
                    char description[100] = "";
                    sscanf(token, "%d;%lf;%99[^\n]",
                           &value->id, &value->value, description);
                    strcpy(value->description, description);
                    row_data[i] = value;
                    break;
                }
                default:
                    row_data[i] = strdup("");
                    break;
            }
            token = strtok(NULL, ",");
        }

        add_row_to_dataframe(df, row_data);

        for (int i = 0; i < num_columns; i++) {
            free(row_data[i]);
        }
        free(row_data);
    }

    fclose(file);
    return df;
}


void save_into_csv(DATAFRAME *cdf, char *file_name) {
    FILE *file = fopen(file_name, "w");
    if (!file) {
        fprintf(stderr, "Impossible d'ouvrir le fichier pour écrire au format CSV: %s\n", file_name);
        return;
    }


    for (unsigned int i = 0; i < cdf->column_count; i++) {
        fprintf(file, "%s", cdf->columns[i]->title);
        if (i < cdf->column_count - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");


    unsigned int num_rows = cdf->columns[0]->size;
    for (unsigned int row = 0; row < num_rows; row++) {
        for (unsigned int col = 0; col < cdf->column_count; col++) {
            char buffer[256] = {0};
            convert_value(cdf->columns[col], row, buffer, sizeof(buffer));
            fprintf(file, "%s", buffer);
            if (col < cdf->column_count - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
