#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define MAX_LINE_LENGTH 1024

DATAFRAME *creer_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME));
    if (!df) {
        fprintf(stderr, "L'allocation de mémoire a échoué pour la dataframe\n");
        return NULL;
    }
    df->colonnes = NULL;
    df->nombre_colonnes = 0;
    df->max_colonnes = 0;
    return df;
}

void remplir_dataframe_par_utilisateur(DATAFRAME *df) {
    if (!df) {
        fprintf(stderr, "La dataframe est NULL.\n");
        return;
    }

    printf("Entrez le nombre de colonnes: ");
    int num_columns;
    scanf("%d", &num_columns);
    df->colonnes = (COLUMN **)malloc(num_columns * sizeof(COLUMN *));
    df->max_colonnes = num_columns;
    df->nombre_colonnes = 0;

    for (int i = 0; i < num_columns; i++) {
        char type[20], name[100];
        printf("Entrez le type et le nom de la colonne %d (par exemple, INT Age): ", i + 1);
        scanf("%s %s", type, name);
        ENUM_TYPE col_type = analyser_type(type);
        COLUMN *new_column = creer_colonne(col_type, name);
        if (!new_column) {
            continue;
        }
        ajouter_colonne_dataframe(df, new_column);

        printf("Entrez le nombre de lignes pour la colonne '%s': ", name);
        int num_rows;
        scanf("%d", &num_rows);
        for (int j = 0; j < num_rows; j++) {
            printf("Saisir les données pour la ligne %d: ", j + 1);
            void *data = lire_donnee_par_type(col_type);
            inserer_valeur(new_column, data);
        }
    }
}

ENUM_TYPE analyser_type(const char *typeStr) {
    if (strcmp(typeStr, "INT") == 0) return INT;
    else if (strcmp(typeStr, "FLOAT") == 0) return FLOAT;
    else if (strcmp(typeStr, "DOUBLE") == 0) return DOUBLE;
    else if (strcmp(typeStr, "CHAR") == 0) return CHAR;
    else if (strcmp(typeStr, "STRING") == 0) return STRING;
    else if (strcmp(typeStr, "STRUCTURE") == 0) return STRUCTURE;
    return -1;
}

void remplir_dataframe_statique(DATAFRAME *df) {
    // Ajouter la première colonne d'entiers
    COLUMN *col1 = creer_colonne(INT, "Colonne d'Entiers");
    if (ajouter_colonne_dataframe(df, col1) == 0) {
        int data1[] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col1, &data1[i]);
        }
    }

    // Ajouter la deuxième colonne de flottants
    COLUMN *col2 = creer_colonne(FLOAT, "Colonne de Flottants");
    if (ajouter_colonne_dataframe(df, col2) == 0) {
        float data2[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col2, &data2[i]);
        }
    }

    // Ajouter la troisième colonne de chaînes de caractères
    COLUMN *col3 = creer_colonne(STRING, "Colonne de Chaînes");
    if (ajouter_colonne_dataframe(df, col3) == 0) {
        char *data3[] = {"Un", "Deux", "Trois", "Quatre", "Cinq"};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col3, data3[i]);
        }
    }

    // Ajouter la quatrième colonne de double
    COLUMN *col4 = creer_colonne(DOUBLE, "Colonne de Doubles");
    if (ajouter_colonne_dataframe(df, col4) == 0) {
        double data4[] = {10.1, 20.2, 30.3, 40.4, 50.5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col4, &data4[i]);
        }
    }

    // Ajouter la cinquième colonne de CHAR
    COLUMN *col5 = creer_colonne(CHAR, "Colonne de Caractères");
    if (ajouter_colonne_dataframe(df, col5) == 0) {
        char data5[] = {'A', 'B', 'C', 'D', 'E'};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col5, &data5[i]);
        }
    }

    // Ajouter la sixième colonne de STRUCTURE
    COLUMN *col6 = creer_colonne(STRUCTURE, "Colonne de Structures");
    if (ajouter_colonne_dataframe(df, col6) == 0) {
        CustomStructure data6[] = {
                {1, 100.1, "Premier"},
                {2, 200.2, "Deuxième"},
                {3, 300.3, "Troisième"},
                {4, 400.4, "Quatrième"},
                {5, 500.5, "Cinquième"}
        };
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col6, &data6[i]);
        }
    }
}


void *lire_donnee_par_type(ENUM_TYPE type) {
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

void liberer_dataframe(DATAFRAME *df) {
    if (df == NULL) return;

    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        if (df->colonnes[i] != NULL) {
            supprimer_colonne(&df->colonnes[i]);
        }
    }

    free(df->colonnes);
    free(df);
}

int ajouter_colonne_dataframe(DATAFRAME *df, COLUMN *col) {
    if (df == NULL || col == NULL) return -1;
    if (df->nombre_colonnes == df->max_colonnes) {
        size_t new_size = df->max_colonnes == 0 ? 1 : df->max_colonnes * 2;
        COLUMN **new_cols = realloc(df->colonnes, new_size * sizeof(COLUMN *));
        if (!new_cols) return -1;
        df->colonnes = new_cols;
        df->max_colonnes = new_size;
    }
    df->colonnes[df->nombre_colonnes++] = col;
    return 0;
}

void afficher_dataframe_complete(DATAFRAME *df) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisée.\n");
        return;
    }
    printf("La dataframe contient %u colonnes:\n", df->nombre_colonnes);
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        printf("Colonne %d: %s\n", i + 1, df->colonnes[i]->titre);
        imprimer_colonne(df->colonnes[i]);
    }
}

void afficher_lignes_dataframe(DATAFRAME *df, unsigned int lignes) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisée\n");
        return;
    }
    printf("Afficher jusqu'à %u lignes de chacune des colonnes %u:\n", lignes, df->nombre_colonnes);
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        COLUMN *col = df->colonnes[i];
        printf("Colonne %d (%s):\n", i + 1, col->titre);

        for (unsigned int j = 0; j < lignes && j < col->taille; j++) {
            switch (col->type_colonne) {
                case INT:
                    printf("%d: %d\n", j + 1, *((int*)col->donnees[j]));
                    break;
                case FLOAT:
                    printf("%d: %f\n", j + 1, *((float*)col->donnees[j]));
                    break;
                case DOUBLE:
                    printf("%d: %lf\n", j + 1, *((double*)col->donnees[j]));
                    break;
                case STRING:
                    printf("%d: %s\n", j + 1, (char*)col->donnees[j]);
                    break;
                case STRUCTURE:
                    CustomStructure *cs = (CustomStructure*)col->donnees[j];
                    printf("%d: ID = %d, Value = %.2f\n", j + 1, cs->id, cs->value);
                    break;
                default:
                    printf("%d: Type de données non géré.\n", j + 1);
                    break;
            }
        }
    }
}

void ajouter_ligne_dataframe(DATAFRAME *df, void **donnees_ligne) {
    if (!df || !df->colonnes) {
        printf("La dataframe n'est pas correctement initialisée.\n");
        return;
    }

    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        if (!inserer_valeur(df->colonnes[i], donnees_ligne[i])) {
            printf("Échec de l'insertion des données dans la colonne %u.\n", i + 1);
        }
    }
}

void supprimer_ligne_dataframe(DATAFRAME *df, unsigned int indice_ligne) {
    if (!df) {
        printf("La dataframe n'est pas initialisée.\n");
        return;
    }

    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        if (indice_ligne >= df->colonnes[i]->taille) {
            printf("L'index de la ligne %u est hors limites pour la colonne %u.\n", indice_ligne, i + 1);
            return;
        }
    }

    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        df->colonnes[i]->taille--;
    }
}

void afficher_colonnes_dataframe(DATAFRAME *df, unsigned int colonnes) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisée.\n");
        return;
    }
    printf("Affichage des %u premières colonnes sur %u colonnes totales:\n", colonnes, df->nombre_colonnes);
    for (unsigned int i = 0; i < colonnes && i < df->nombre_colonnes; i++) {
        printf("Colonne %u (%s):\n", i + 1, df->colonnes[i]->titre);
        imprimer_colonne(df->colonnes[i]);
    }
}

void supprimer_colonne_dataframe(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->nombre_colonnes) {
        printf("Index de colonne invalide ou dataframe vide.\n");
        return;
    }

    supprimer_colonne(&df->colonnes[index]);

    for (unsigned int i = index; i < df->nombre_colonnes - 1; i++) {
        df->colonnes[i] = df->colonnes[i + 1];
    }
    df->nombre_colonnes--;

    if (df->nombre_colonnes > 0 && df->nombre_colonnes == df->max_colonnes / 4) {
        COLUMN **new_colonnes = realloc(df->colonnes, (df->max_colonnes / 2) * sizeof(COLUMN *));
        if (new_colonnes) {
            df->colonnes = new_colonnes;
            df->max_colonnes /= 2;
        }
    }
}

void renommer_titre_colonne(DATAFRAME *df, unsigned int indice_colonne, const char *nouveau_titre) {
    if (!df || indice_colonne >= df->nombre_colonnes || !nouveau_titre) {
        printf("Index de colonne ou nouveau titre invalide.\n");
        return;
    }
    char *new_title_copy = strdup(nouveau_titre);
    if (!new_title_copy) {
        printf("Échec de l'allocation de mémoire pour le nouveau titre.\n");
        return;
    }
    free(df->colonnes[indice_colonne]->titre);
    df->colonnes[indice_colonne]->titre = new_title_copy;
}

int verifier_existence_valeur(DATAFRAME *df, void *valeur) {
    if (!df || !valeur) {
        printf("Dataframe ou valeur invalide.\n");
        return 0;
    }
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        for (unsigned int j = 0; j < df->colonnes[i]->taille; j++) {
            if (comparer_valeurs(df->colonnes[i]->type_colonne, df->colonnes[i]->donnees[j], valeur) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

void *obtenir_valeur_cellule(DATAFRAME *df, unsigned int ligne, unsigned int colonne) {
    if (df == NULL || colonne >= df->nombre_colonnes || ligne >= df->colonnes[colonne]->taille) {
        printf("Index de ligne ou de colonne non valide.\n");
        return NULL;
    }
    return obtenir_valeur_a(df->colonnes[colonne], ligne);
}


void definir_valeur_cellule(DATAFRAME *df, unsigned int ligne, unsigned int colonne, void *valeur) {
    if (!df || colonne >= df->nombre_colonnes || ligne >= df->colonnes[colonne]->taille || !valeur) {
        printf("Index de ligne ou de colonne non valide, ou valeur nulle fournie.\n");
        return;
    }

    if (df->colonnes[colonne]->type_colonne == STRING) {
        free(df->colonnes[colonne]->donnees[ligne]);
    }

    switch (df->colonnes[colonne]->type_colonne) {
        case INT:
            *(int *)(df->colonnes[colonne]->donnees[ligne]) = *(int *)valeur;
            break;
        case FLOAT:
            *(float *)(df->colonnes[colonne]->donnees[ligne]) = *(float *)valeur;
            break;
        case DOUBLE:
            *(double *)(df->colonnes[colonne]->donnees[ligne]) = *(double *)valeur;
            break;
        case STRING:
            df->colonnes[colonne]->donnees[ligne] = strdup((char *)valeur);
            break;
        case STRUCTURE:
            df->colonnes[colonne]->donnees[ligne] = malloc(sizeof(CustomStructure));
            if (df->colonnes[colonne]->donnees[ligne]) {
                memcpy(df->colonnes[colonne]->donnees[ligne], valeur, sizeof(CustomStructure));
            } else {
                printf("L'allocation de mémoire a échoué pour la structure.\n");
            }
            break;
        default:
            printf("Type de données non géré.\n");
            break;
    }
}

void afficher_noms_colonnes(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe n'est pas initialisée.\n");
        return;
    }
    printf("Noms de colonnes:\n");
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        printf("Colonne %u: %s\n", i + 1, df->colonnes[i]->titre);
    }
}

void afficher_nombre_lignes(DATAFRAME *df) {
    if (!df || df->nombre_colonnes == 0 || !df->colonnes[0]) {
        printf("La dataframe est vide ou mal initialisée.\n");
    } else {
        printf("Nombre de lignes: %u\n", df->colonnes[0]->taille);
    }
}

void afficher_nombre_colonnes(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe est vide ou mal initialisée.\n");
    } else {
        printf("Nombre de colonnes: %u\n", df->nombre_colonnes);
    }
}

int compter_cellules_egales_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_egal_a(df->colonnes[i], valeur);
    }
    return count;
}

int compter_cellules_superieures_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_superieur_a(df->colonnes[i], valeur);
    }
    return count;
}

int compter_cellules_inferieures_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_inferieur_a(df->colonnes[i], valeur);
    }
    return count;
}

// Charger un dataframe à partir d'un fichier CSV
DATAFRAME* charger_depuis_csv(char *nom_fichier, ENUM_TYPE *dftype, int taille) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        fprintf(stderr, "Échec de l'ouverture du fichier CSV : %s\n", nom_fichier);
        return NULL;
    }

    DATAFRAME *df = creer_dataframe();
    if (!df) {
        fprintf(stderr, "Échec de la création du dataframe.\n");
        fclose(file);
        return NULL;
    }

    char line[1024];
    char *token;
    int row_count = 0;

    // Lire l'en-tête pour les titres de colonnes
    if (fgets(line, sizeof(line), file)) {
        char *column_titles[taille];
        int col_index = 0;

        token = strtok(line, ",");
        while (token != NULL && col_index < taille) {
            column_titles[col_index] = strdup(token);
            col_index++;
            token = strtok(NULL, ",");
        }

        for (int i = 0; i < taille; i++) {
            COLUMN *col = creer_colonne(dftype[i], column_titles[i]);
            if (ajouter_colonne_dataframe(df, col) != 0) {
                fprintf(stderr, "Échec de l'ajout de la colonne : %s\n", column_titles[i]);
            }
        }
    }

    // Lire les données
    while (fgets(line, sizeof(line), file)) {
        void **row_data = malloc(taille * sizeof(void *));
        int col_index = 0;

        token = strtok(line, ",");
        while (token != NULL && col_index < taille) {
            switch (dftype[col_index]) {
                case INT:
                    row_data[col_index] = malloc(sizeof(int));
                    *(int *)row_data[col_index] = atoi(token);
                    break;
                case FLOAT:
                    row_data[col_index] = malloc(sizeof(float));
                    *(float *)row_data[col_index] = atof(token);
                    break;
                case DOUBLE:
                    row_data[col_index] = malloc(sizeof(double));
                    *(double *)row_data[col_index] = atof(token);
                    break;
                case CHAR:
                    row_data[col_index] = malloc(sizeof(char));
                    *(char *)row_data[col_index] = token[0];
                    break;
                case STRING:
                    row_data[col_index] = strdup(token);
                    break;
                case STRUCTURE:
                    row_data[col_index] = malloc(sizeof(CustomStructure));
                    // Supposons que la structure soit sérialisée en une seule colonne (à adapter si besoin)
                    sscanf(token, "%d %lf %s", &((CustomStructure *)row_data[col_index])->id,
                           &((CustomStructure *)row_data[col_index])->value,
                           ((CustomStructure *)row_data[col_index])->description);
                    break;
                default:
                    fprintf(stderr, "Type non pris en charge pour l'importation.\n");
                    break;
            }
            col_index++;
            token = strtok(NULL, ",");
        }

        ajouter_ligne_dataframe(df, row_data);
        free(row_data);
        row_count++;
    }

    fclose(file);
    return df;
}

// Sauvegarder un dataframe dans un fichier CSV
void sauvegarder_dans_csv(DATAFRAME *df, char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "w");
    if (!file) {
        fprintf(stderr, "Échec de l'ouverture du fichier pour l'écriture : %s\n", nom_fichier);
        return;
    }

    // Écrire les titres des colonnes
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        fprintf(file, "%s", df->colonnes[i]->titre);
        if (i < df->nombre_colonnes - 1) {
            fprintf(file, ",");
        }
    }
    fprintf(file, "\n");

    // Déterminer le nombre de lignes
    unsigned int row_count = df->colonnes[0]->taille;

    // Écrire les données
    for (unsigned int row = 0; row < row_count; row++) {
        for (unsigned int col = 0; col < df->nombre_colonnes; col++) {
            char buffer[256];
            convertir_valeur(df->colonnes[col], row, buffer, sizeof(buffer));
            fprintf(file, "%s", buffer);
            if (col < df->nombre_colonnes - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

