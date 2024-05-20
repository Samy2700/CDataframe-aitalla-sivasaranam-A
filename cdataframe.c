/**
Nom du projet : CDataFrame
Auteurs : Samy AIT ALLA, Sajin Sivasaranam
Rôle : Implémentation des fonctions de gestion des dataframes
*/

#include "cdataframe.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define MAX_LINE_LENGTH 1024

/**
Rôle : Créer un nouveau dataframe
Paramètres : Aucun
Retour : Pointeur vers le dataframe nouvellement créé
*/
DATAFRAME *creer_dataframe() {
    DATAFRAME *df = (DATAFRAME *)malloc(sizeof(DATAFRAME)); // Allocation de mémoire pour le dataframe
    if (!df) {
        fprintf(stderr, "L'allocation de mémoire a échoué pour la dataframe\n");
        return NULL;
    }
    df->colonnes = NULL;
    df->nombre_colonnes = 0;
    df->max_colonnes = 0;
    return df;
}

/**
Rôle : Remplir un dataframe avec des données saisies par l'utilisateur
Paramètres :
    - df : Pointeur vers le dataframe à remplir
Retour : Aucun car elle remplit directement le dataframe passé en paramètre.
*/
void remplir_dataframe_par_utilisateur(DATAFRAME *df) {
    if (!df) {
        fprintf(stderr, "La dataframe est NULL.\n");
        return;
    }

    printf("Entrez le nombre de colonnes: ");
    int nombre_colonnes;
    scanf("%d", &nombre_colonnes);
    df->colonnes = (COLONNE **)malloc(nombre_colonnes * sizeof(COLONNE *));
    df->max_colonnes = nombre_colonnes;
    df->nombre_colonnes = 0;

    for (int i = 0; i < nombre_colonnes; i++) {
        char type[20], nom[100];
        printf("Entrez le type et le nom de la colonne %d (par exemple, INT Age): ", i + 1);
        scanf("%s %s", type, nom);
        TYPE_ENUM type_colonne = analyser_type(type);
        COLONNE *nouvelle_colonne = creer_colonne(type_colonne, nom);
        if (!nouvelle_colonne) {
            continue;
        }
        ajouter_colonne_dataframe(df, nouvelle_colonne);

        printf("Entrez le nombre de lignes pour la colonne '%s': ", nom);
        int nombre_lignes;
        scanf("%d", &nombre_lignes);
        for (int j = 0; j < nombre_lignes; j++) {
            printf("Saisir les donnees pour la ligne %d: ", j + 1);
            void *donnee = lire_donnee_par_type(type_colonne);
            inserer_valeur(nouvelle_colonne, donnee);
        }
    }
}

/**
Rôle : Analyser le type de colonne à partir d'une chaîne de caractères
Paramètres :
    - typeStr : Chaîne de caractères représentant le type
Retour : Valeur de l'énumération TYPE_ENUM correspondant au type
*/
TYPE_ENUM analyser_type(const char *typeStr) {
    if (strcmp(typeStr, "INT") == 0) return INT;
    else if (strcmp(typeStr, "FLOAT") == 0) return FLOAT;
    else if (strcmp(typeStr, "DOUBLE") == 0) return DOUBLE;
    else if (strcmp(typeStr, "CHAR") == 0) return CHAR;
    else if (strcmp(typeStr, "STRING") == 0) return STRING;
    else if (strcmp(typeStr, "STRUCTURE") == 0) return STRUCTURE;
    return -1;
}

/**
Rôle : Remplir un dataframe avec des données prédéfinies
Paramètres :
    - df : Pointeur vers le dataframe à remplir
Retour : Aucun car elle remplit directement la dataframe en dur
*/
void remplir_dataframe_statique(DATAFRAME *df) {
    // Ajouter la première colonne d'entiers
    COLONNE *col1 = creer_colonne(INT, "Colonne d'Entiers");
    if (ajouter_colonne_dataframe(df, col1) == 0) {
        int data1[] = {1, 2, 3, 4, 5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col1, &data1[i]);
        }
    }

    // Ajouter la deuxième colonne de flottants
    COLONNE *col2 = creer_colonne(FLOAT, "Colonne de Flottants");
    if (ajouter_colonne_dataframe(df, col2) == 0) {
        float data2[] = {1.1, 2.2, 3.3, 4.4, 5.5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col2, &data2[i]);
        }
    }

    // Ajouter la troisième colonne de chaînes de caractères
    COLONNE *col3 = creer_colonne(STRING, "Colonne de Chaines");
    if (ajouter_colonne_dataframe(df, col3) == 0) {
        char *data3[] = {"Un", "Deux", "Trois", "Quatre", "Cinq"};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col3, data3[i]);
        }
    }

    // Ajouter la quatrième colonne de double
    COLONNE *col4 = creer_colonne(DOUBLE, "Colonne de Doubles");
    if (ajouter_colonne_dataframe(df, col4) == 0) {
        double data4[] = {10.1, 20.2, 30.3, 40.4, 50.5};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col4, &data4[i]);
        }
    }

    // Ajouter la cinquième colonne de CHAR
    COLONNE *col5 = creer_colonne(CHAR, "Colonne de Caracteres");
    if (ajouter_colonne_dataframe(df, col5) == 0) {
        char data5[] = {'A', 'B', 'C', 'D', 'E'};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col5, &data5[i]);
        }
    }

    // Ajouter la sixième colonne de STRUCTURE
    COLONNE *col6 = creer_colonne(STRUCTURE, "Colonne de Structures");
    if (ajouter_colonne_dataframe(df, col6) == 0) {
        StructurePersonnalisee data6[] = {
                {1, 100.1, "Premier"},
                {2, 200.2, "Deuxieme"},
                {3, 300.3, "Troisieme"},
                {4, 400.4, "Quatrieme"},
                {5, 500.5, "Cinquieme"}
        };
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col6, &data6[i]);
        }
    }
    // Ajouter la septième colonne de UINT
    COLONNE *col7 = creer_colonne(UINT, "Colonne de UINT");
    if (ajouter_colonne_dataframe(df, col7) == 0) {
        unsigned int data7[] = {10, 20, 30, 40, 50};
        for (int i = 0; i < 5; i++) {
            inserer_valeur(col7, &data7[i]);
        }
    }
}


/**
Rôle : Lire une donnée en fonction de son type
Paramètres :
    - type : Type de la donnée à lire
Retour : Pointeur vers la donnée lue
*/
void *lire_donnee_par_type(TYPE_ENUM type) {
    void *donnee = NULL;
    switch (type) {
        case INT: {
            int *valeur = malloc(sizeof(int));
            printf("Saisissez un entier: ");
            scanf("%d", valeur);
            donnee = valeur;
            break;
        }
        case FLOAT: {
            float *valeur = malloc(sizeof(float));
            printf("Entrez un flottant: ");
            scanf("%f", valeur);
            donnee = valeur;
            break;
        }
        case DOUBLE: {
            double *valeur = malloc(sizeof(double));
            printf("Entrez un double: ");
            scanf("%lf", valeur);
            donnee = valeur;
            break;
        }
        case CHAR: {
            char *valeur = malloc(sizeof(char));
            printf("Entrez un caractere: ");
            scanf(" %c", valeur);
            donnee = valeur;
            break;
        }
        case STRING: {
            char *valeur = malloc(256);
            printf("Entrez une chaine de caractere: ");
            scanf("%255s", valeur);
            donnee = valeur;
            break;
        }
        case STRUCTURE: {
            StructurePersonnalisee *valeur = malloc(sizeof(StructurePersonnalisee));
            printf("Entrez les valeurs de la structure (identifiant, valeur, description): ");
            scanf("%d %lf %s", &valeur->identifiant, &valeur->valeur, valeur->description);
            donnee = valeur;
            break;
        }
        default:
            printf("Type de donnees non pris en charge.\n");
            break;
    }
    return donnee;
}

/**
Rôle : Libérer la mémoire allouée pour un dataframe
Paramètres :
    - df : Pointeur vers le dataframe à libérer
Retour : Aucun car elle libère simplement la mémoire allouée pour le dataframe.
*/
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

/**
Rôle : Ajouter une colonne à un dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - col : Pointeur vers la colonne à ajouter
Retour : 0 en cas de succès, -1 en cas d'erreur
*/
int ajouter_colonne_dataframe(DATAFRAME *df, COLONNE *col) {
    if (df == NULL || col == NULL) return -1;
    // Vérifie si le dataframe a atteint le nombre maximum de colonnes
    if (df->nombre_colonnes == df->max_colonnes) {
        // Calcule la nouvelle taille du tableau de colonnes
        size_t nouvelle_taille = df->max_colonnes == 0 ? 1 : df->max_colonnes * 2;
        //réalloue la mémoire pour agrandir le tableau de colonnes
        COLONNE **nouvelles_colonnes = realloc(df->colonnes, nouvelle_taille * sizeof(COLONNE *));
        if (!nouvelles_colonnes) return -1;
        df->colonnes = nouvelles_colonnes;
        df->max_colonnes = nouvelle_taille;
    }
    // Ajoute la nouvelle colonne à la première position disponible dans le tableau
    df->colonnes[df->nombre_colonnes++] = col;
    return 0;
}

/**
Rôle : Afficher le dataframe complet
Paramètres :
    - df : Pointeur vers le dataframe à afficher
Retour : Aucun car elle affiche simplement le contenu du dataframe.
*/
void afficher_dataframe_complete(DATAFRAME *df) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisee.\n");
        return;
    }
    printf("La dataframe contient %u colonnes:\n", df->nombre_colonnes);
    // Parcourt et affiche chaque colonne du dataframe
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        printf("Colonne %d: %s\n", i + 1, df->colonnes[i]->titre);
        imprimer_colonne(df->colonnes[i]);
    }
}

/**
Rôle : Afficher un nombre spécifique de lignes pour chaque colonne du dataframe
Paramètres :
    - df : Pointeur vers le dataframe à afficher
    - lignes : Nombre de lignes à afficher
Retour : Aucun  car elle affiche directement un certain nombre de lignes du dataframe.
*/
void afficher_lignes_dataframe(DATAFRAME *df, unsigned int lignes) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisee\n");
        return;
    }
    printf("Afficher jusqu'a %u lignes de chacune des colonnes %u:\n", lignes, df->nombre_colonnes);
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        COLONNE *col = df->colonnes[i];
        printf("Colonne %d (%s):\n", i + 1, col->titre);
        // Affiche les données de la colonne, jusqu'à un maximum de 'lignes' ou la taille de la colonne
        for (unsigned int j = 0; j < lignes && j < col->taille; j++) {
            // Affiche les données en fonction du type de la colonne
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
                    StructurePersonnalisee *sp = (StructurePersonnalisee*)col->donnees[j];
                    printf("%d: Identifiant = %d, Valeur = %.2f, Description = %s\n", j + 1, sp->identifiant, sp->valeur, sp->description);
                    break;
                default:
                    printf("%d: Type de donnees non géré.\n", j + 1);
                    break;
            }
        }
    }
}

/**
Rôle : Ajouter une ligne de données à un dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - donnees_ligne : Tableau de pointeurs vers les données de la ligne à ajouter
Retour : Aucun car elle ajoute directement les données dans le dataframe.
*/
void ajouter_ligne_dataframe(DATAFRAME *df, void **donnees_ligne) {
    if (!df || !df->colonnes) {
        printf("La dataframe n'est pas correctement initialisee.\n");
        return;
    }

    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        if (!inserer_valeur(df->colonnes[i], donnees_ligne[i])) {
            printf("Echec de l'insertion des donnees dans la colonne %u.\n", i + 1);
        }
    }
}

/**
Rôle : Supprimer une ligne de données d'un dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - indice_ligne : Index de la ligne à supprimer
Retour : Aucun car elle supprime directement la ligne spécifiée du dataframe.
*/
void supprimer_ligne_dataframe(DATAFRAME *df, unsigned int indice_ligne) {
    if (!df) {
        printf("La dataframe n'est pas initialisee.\n");
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

/**
Rôle : Afficher un nombre spécifique de colonnes du dataframe
Paramètres :
    - df : Pointeur vers le dataframe à afficher
    - colonnes : Nombre de colonnes à afficher
Retour : Aucun car elle affiche un certain nombre de colonnes du dataframe.
*/
void afficher_colonnes_dataframe(DATAFRAME *df, unsigned int colonnes) {
    if (!df || !df->colonnes) {
        printf("La dataframe est vide ou non initialisee.\n");
        return;
    }
    printf("Affichage des %u premieres colonnes sur %u colonnes totales:\n", colonnes, df->nombre_colonnes);
    for (unsigned int i = 0; i < colonnes && i < df->nombre_colonnes; i++) {
        printf("Colonne %u (%s):\n", i + 1, df->colonnes[i]->titre);
        imprimer_colonne(df->colonnes[i]);
    }
}

/**
Rôle : Supprimer une colonne d'un dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - index : Index de la colonne à supprimer
Retour : Aucun car elle supprime directement la colonne spécifiée du dataframe.
*/
void supprimer_colonne_dataframe(DATAFRAME *df, unsigned int index) {
    if (!df || index >= df->nombre_colonnes) {
        printf("Index de colonne invalide ou dataframe vide.\n");
        return;
    }
    // Supprime la colonne à l'index spécifié
    supprimer_colonne(&df->colonnes[index]);

    // Décale les colonnes restantes pour combler l'espace laissé par la colonne supprimée
    for (unsigned int i = index; i < df->nombre_colonnes - 1; i++) {
        df->colonnes[i] = df->colonnes[i + 1];
    }
    df->nombre_colonnes--;

    // Si le nombre de colonnes est un quart de la capacité maximale, rétrécit le tableau de colonnes
    if (df->nombre_colonnes > 0 && df->nombre_colonnes == df->max_colonnes / 4) {
        //Réalloue  la mémoire pour réduire la taille du tableau de colonnes
        COLONNE **nouvelles_colonnes = realloc(df->colonnes, (df->max_colonnes / 2) * sizeof(COLONNE *));
        // Met à jour le tableau de colonnes et la capacité maximale
        if (nouvelles_colonnes) {
            df->colonnes = nouvelles_colonnes;
            df->max_colonnes /= 2;
        }
    }
}

/**
Rôle : Renommer le titre d'une colonne
Paramètres :
    - df : Pointeur vers le dataframe
    - indice_colonne : Index de la colonne à renommer
    - nouveau_titre : Nouveau titre de la colonne
Retour : Aucun car elle renomme directement le titre de la colonne spécifiée.
*/
void renommer_titre_colonne(DATAFRAME *df, unsigned int indice_colonne, const char *nouveau_titre) {
    if (!df || indice_colonne >= df->nombre_colonnes || !nouveau_titre) {
        printf("Index de colonne ou nouveau titre invalide.\n");
        return;
    }
    // Crée une copie du nouveau titre
    char *nouveau_titre_copie = strdup(nouveau_titre);
    if (!nouveau_titre_copie) {
        printf("Echec de l'allocation de memoire pour le nouveau titre.\n");
        return;
    }
    // Libère la mémoire de l'ancien titre de la colonne
    free(df->colonnes[indice_colonne]->titre);
    // Affecte le nouveau titre à la colonne
    df->colonnes[indice_colonne]->titre = nouveau_titre_copie;
}

/**
Rôle : Vérifier l'existence d'une valeur dans le dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - valeur : Pointeur vers la valeur à vérifier
Retour : 1 si la valeur existe, 0 sinon
*/
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

/**
Rôle : Obtenir la valeur d'une cellule spécifique du dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - ligne : Index de la ligne
    - colonne : Index de la colonne
Retour : Pointeur vers la valeur de la cellule
*/
void *obtenir_valeur_cellule(DATAFRAME *df, unsigned int ligne, unsigned int colonne) {
    if (df == NULL || colonne >= df->nombre_colonnes || ligne >= df->colonnes[colonne]->taille) {
        printf("Index de ligne ou de colonne non valide.\n");
        return NULL;
    }
    return obtenir_valeur_a(df->colonnes[colonne], ligne);
}

/**
Rôle : Définir la valeur d'une cellule spécifique du dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - ligne : Index de la ligne
    - colonne : Index de la colonne
    - valeur : Pointeur vers la nouvelle valeur de la cellule
Retour : Aucun car elle définit simplement la valeur d'une cellule spécifique dans le dataframe.
*/
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
            df->colonnes[colonne]->donnees[ligne] = malloc(sizeof(StructurePersonnalisee));
            if (df->colonnes[colonne]->donnees[ligne]) {
                memcpy(df->colonnes[colonne]->donnees[ligne], valeur, sizeof(StructurePersonnalisee));
            } else {
                printf("L'allocation de memoire a echoue pour la structure.\n");
            }
            break;
        default:
            printf("Type de donnees non gere.\n");
            break;
    }
}

/**
Rôle : Afficher les noms des colonnes du dataframe
Paramètres :
    - df : Pointeur vers le dataframe
Retour : Aucun car elle affiche simplement les noms des colonnes du dataframe.
*/
void afficher_noms_colonnes(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe n'est pas initialisee.\n");
        return;
    }
    printf("Noms de colonnes:\n");
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        printf("Colonne %u: %s\n", i + 1, df->colonnes[i]->titre);
    }
}

/**
Rôle : Afficher le nombre de lignes du dataframe
Paramètres :
    - df : Pointeur vers le dataframe
Retour : Aucun car elle affiche simplement le nombre de lignes du dataframe.
*/
void afficher_nombre_lignes(DATAFRAME *df) {
    if (!df || df->nombre_colonnes == 0 || !df->colonnes[0]) {
        printf("La dataframe est vide ou mal initialisee.\n");
    } else {
        printf("Nombre de lignes: %u\n", df->colonnes[0]->taille);
    }
}

/**
Rôle : Afficher le nombre de colonnes du dataframe
Paramètres :
    - df : Pointeur vers le dataframe
Retour : Aucun car elle affiche simplement le nombre de colonnes du dataframe.
*/
void afficher_nombre_colonnes(DATAFRAME *df) {
    if (!df) {
        printf("La dataframe est vide ou mal initialisee.\n");
    } else {
        printf("Nombre de colonnes: %u\n", df->nombre_colonnes);
    }
}

/**
Rôle : Compter le nombre de cellules égales à une valeur donnée dans le dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - valeur : Pointeur vers la valeur à comparer
Retour : Nombre de cellules égales à la valeur donnée
*/
int compter_cellules_egales_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_egal_a(df->colonnes[i], valeur);
    }
    return count;
}

/**
Rôle : Compter le nombre de cellules supérieures à une valeur donnée dans le dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - valeur : Pointeur vers la valeur à comparer
Retour : Nombre de cellules supérieures à la valeur donnée
*/
int compter_cellules_superieures_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_superieur_a(df->colonnes[i], valeur);
    }
    return count;
}

/**
Rôle : Compter le nombre de cellules inférieures à une valeur donnée dans le dataframe
Paramètres :
    - df : Pointeur vers le dataframe
    - valeur : Pointeur vers la valeur à comparer
Retour : Nombre de cellules inférieures à la valeur donnée
*/
int compter_cellules_inferieures_a(DATAFRAME *df, void *valeur) {
    int count = 0;
    for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
        count += compter_inferieur_a(df->colonnes[i], valeur);
    }
    return count;
}

/**
Rôle : Charger un dataframe à partir d'un fichier CSV
Paramètres :
    - nom_fichier : Nom du fichier CSV
    - dftype : Tableau des types des colonnes
    - taille : Taille du tableau des types
Retour : Pointeur vers le dataframe chargé
*/
DATAFRAME* charger_depuis_csv(char *nom_fichier, TYPE_ENUM *dftype, int taille) {
    FILE *file = fopen(nom_fichier, "r");
    if (!file) {
        fprintf(stderr, "Echec de l'ouverture du fichier CSV : %s\n", nom_fichier);
        return NULL;
    }

    DATAFRAME *df = creer_dataframe();
    if (!df) {
        fprintf(stderr, "Echec de la creation du dataframe.\n");
        fclose(file);
        return NULL;
    }

    char ligne[1024];
    char *token;
    int nombre_lignes = 0;

    // Lire l'en-tête pour les titres de colonnes
    if (fgets(ligne, sizeof(ligne), file)) {
        char *titres_colonnes[taille];
        int indice_colonne = 0;

        // Tokenize la ligne de l'en-tête pour extraire les titres de colonnes
        token = strtok(ligne, ",");
        while (token != NULL && indice_colonne < taille) {
            titres_colonnes[indice_colonne] = strdup(token);
            indice_colonne++;
            token = strtok(NULL, ",");
        }

        // Crée et ajoute chaque colonne au dataframe
        for (int i = 0; i < taille; i++) {
            COLONNE *col = creer_colonne(dftype[i], titres_colonnes[i]);
            if (ajouter_colonne_dataframe(df, col) != 0) {
                fprintf(stderr, "Echec de l'ajout de la colonne : %s\n", titres_colonnes[i]);
            }
        }
    }

    // Lecture des données
    while (fgets(ligne, sizeof(ligne), file)) {
        void **donnees_ligne = malloc(taille * sizeof(void *));   //Tableau qui stocke les données de la ligne
        int indice_colonne = 0;

        token = strtok(ligne, ",");
        while (token != NULL && indice_colonne < taille) {
            // Allocation de la mémoire et convertit les valeurs en fonction du type de chaque colonne
            switch (dftype[indice_colonne]) {
                case INT:
                    donnees_ligne[indice_colonne] = malloc(sizeof(int));
                    *(int *)donnees_ligne[indice_colonne] = atoi(token);
                    break;
                case FLOAT:
                    donnees_ligne[indice_colonne] = malloc(sizeof(float));
                    *(float *)donnees_ligne[indice_colonne] = atof(token);
                    break;
                case DOUBLE:
                    donnees_ligne[indice_colonne] = malloc(sizeof(double));
                    *(double *)donnees_ligne[indice_colonne] = atof(token);
                    break;
                case CHAR:
                    donnees_ligne[indice_colonne] = malloc(sizeof(char));
                    *(char *)donnees_ligne[indice_colonne] = token[0];
                    break;
                case STRING:
                    donnees_ligne[indice_colonne] = strdup(token);
                    break;
                case STRUCTURE:
                    donnees_ligne[indice_colonne] = malloc(sizeof(StructurePersonnalisee));
                    sscanf(token, "%d %lf %s", &((StructurePersonnalisee *)donnees_ligne[indice_colonne])->identifiant,
                           &((StructurePersonnalisee *)donnees_ligne[indice_colonne])->valeur,
                           ((StructurePersonnalisee *)donnees_ligne[indice_colonne])->description);
                    break;
                default:
                    fprintf(stderr, "Type non pris en charge pour l'importation.\n");
                    break;
            }
            indice_colonne++;
            token = strtok(NULL, ",");
        }

        ajouter_ligne_dataframe(df, donnees_ligne);
        free(donnees_ligne);
        nombre_lignes++;
    }

    fclose(file);
    return df;
}

/**
Rôle : Sauvegarder un dataframe dans un fichier CSV
Paramètres :
    - df : Pointeur vers le dataframe à sauvegarder
    - nom_fichier : Nom du fichier CSV
Retour : Aucun car elle sauvegarde simplement le dataframe dans un fichier CSV.
*/
void sauvegarder_dans_csv(DATAFRAME *df, char *nom_fichier) {
    FILE *file = fopen(nom_fichier, "w");
    if (!file) {
        fprintf(stderr, "Echec de l'ouverture du fichier pour l'ecriture : %s\n", nom_fichier);
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
    unsigned int nombre_lignes = df->colonnes[0]->taille;

    // Écrire les données
    for (unsigned int ligne = 0; ligne < nombre_lignes; ligne++) {
        for (unsigned int col = 0; col < df->nombre_colonnes; col++) {
            char tampon[256];
            convertir_valeur(df->colonnes[col], ligne, tampon, sizeof(tampon));
            fprintf(file, "%s", tampon);
            if (col < df->nombre_colonnes - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}
