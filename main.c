#include "cdataframe.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions
void afficher_menu_principal();
void afficher_menu_dataframe();
void afficher_menu_colonne();
void afficher_menu_ligne();
void afficher_menu_analyse();
void afficher_menu_tri();
void afficher_menu_cellule();

int main() {
    DATAFRAME *df = creer_dataframe();
    if (!df) {
        fprintf(stderr, "Échec de la création du dataframe.\n");
        return 1;
    }

    int choix;
    char nom_fichier_csv[256] = "Exemple.csv"; // Nom par défaut du fichier CSV
    ENUM_TYPE types_colonnes[] = {INT, FLOAT, STRING, DOUBLE, CHAR, STRUCTURE};  // Types de colonnes exemple
    int nombre_colonnes = sizeof(types_colonnes) / sizeof(types_colonnes[0]);

    printf("Voulez-vous remplir le dataframe vous-même (1), utiliser des données pré-définies (2), ou charger depuis un fichier CSV (3) ? Entrez votre choix : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("Veuillez remplir le dataframe :\n");
            remplir_dataframe_par_utilisateur(df);
            break;
        case 2:
            printf("Remplissage automatique du dataframe avec des données prédéfinies :\n");
            remplir_dataframe_statique(df);
            afficher_dataframe_complete(df); // Afficher les données pour vérifier qu'elles sont correctes
            break;
        case 3:
            printf("Entrez le nom du fichier CSV à charger : ");
            scanf("%255s", nom_fichier_csv); // Limiter la longueur de la chaîne lue
            df = charger_depuis_csv(nom_fichier_csv, types_colonnes, nombre_colonnes);
            if (!df) {
                fprintf(stderr, "Échec du chargement du dataframe depuis le fichier CSV.\n");
                return 1;
            }
            afficher_dataframe_complete(df); // Afficher les données pour vérifier qu'elles sont correctes
            break;
        default:
            printf("Choix invalide.\n");
            liberer_dataframe(df);
            return 1;
    }

    int en_cours = 1;
    int ligne, col, valeur;
    char type_donnee, nouveau_nom[100];

    while (en_cours) {
        afficher_menu_principal();
        scanf("%d", &choix);

        switch (choix) {
            case 1: // Options de Dataframe
                while (1) {
                    afficher_menu_dataframe();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        afficher_dataframe_complete(df);
                    } else if (choix == 2) {
                        printf("Entrez le nombre de lignes à afficher : ");
                        scanf("%d", &ligne);
                        afficher_lignes_dataframe(df, ligne);
                    } else if (choix == 3) {
                        printf("Entrez le nombre de colonnes à afficher : ");
                        scanf("%d", &col);
                        afficher_colonnes_dataframe(df, col);
                    } else if (choix == 4) {
                        printf("Entrez le nom du fichier pour sauvegarder le dataframe dans un fichier CSV : ");
                        scanf("%s", nom_fichier_csv);
                        sauvegarder_dans_csv(df, nom_fichier_csv);
                        printf("Dataframe sauvegardé dans %s\n", nom_fichier_csv);
                    } else if (choix == 5) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 2: // Options de Ligne
                while (1) {
                    afficher_menu_ligne();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        printf("Entrez les données pour une nouvelle ligne :\n");
                        void **donnees_ligne = malloc(df->nombre_colonnes * sizeof(void *));
                        for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
                            printf("Entrez la valeur pour la colonne %s : ", df->colonnes[i]->titre);
                            donnees_ligne[i] = lire_donnee_par_type(df->colonnes[i]->type_colonne);
                        }
                        ajouter_ligne_dataframe(df, donnees_ligne);
                        printf("Valeur ajoutée avec succès.\n");
                        for (unsigned int i = 0; i < df->nombre_colonnes; i++) {
                            free(donnees_ligne[i]);
                        }
                        free(donnees_ligne);
                    } else if (choix == 2) {
                        printf("Entrez l'indice de ligne à supprimer (0-%u) : ", df->colonnes[0]->taille - 1);
                        scanf("%d", &ligne);
                        supprimer_ligne_dataframe(df, ligne);
                    } else if (choix == 3) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 3: // Options de Colonne
                while (1) {
                    afficher_menu_colonne();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        printf("Entrez le type de données pour la nouvelle colonne (i pour int, f pour float, c pour char, s pour string, u pour structure) : ");
                        scanf(" %c", &type_donnee);
                        printf("Entrez le nom pour la nouvelle colonne : ");
                        scanf("%s", nouveau_nom);
                        COLUMN *nouvelle_colonne = NULL;
                        switch (type_donnee) {
                            case 'i':
                                nouvelle_colonne = creer_colonne(INT, nouveau_nom);
                                break;
                            case 'f':
                                nouvelle_colonne = creer_colonne(FLOAT, nouveau_nom);
                                break;
                            case 'c':
                                nouvelle_colonne = creer_colonne(CHAR, nouveau_nom);
                                break;
                            case 's':
                                nouvelle_colonne = creer_colonne(STRING, nouveau_nom);
                                break;
                            case 'u':
                                nouvelle_colonne = creer_colonne(STRUCTURE, nouveau_nom);
                                break;
                            default:
                                printf("Type spécifié invalide.\n");
                                continue;
                        }
                        if (ajouter_colonne_dataframe(df, nouvelle_colonne) == 0) {
                            printf("Nouvelle colonne '%s' ajoutée avec succès.\n", nouveau_nom);

                            // Demande à l'utilisateur de fournir des données
                            printf("Entrez le nombre de lignes pour la nouvelle colonne : ");
                            int nombre_lignes;
                            scanf("%d", &nombre_lignes);
                            for (int i = 0; i < nombre_lignes; i++) {
                                printf("Entrez les données pour la ligne %d : ", i + 1);
                                void *data = lire_donnee_par_type(nouvelle_colonne->type_colonne);
                                inserer_valeur(nouvelle_colonne, data);
                                free(data); // Libérer la mémoire après l'insertion
                            }
                        } else {
                            printf("Échec de l'ajout de la nouvelle colonne.\n");
                        }
                    } else if (choix == 2) {
                        printf("Entrez l'indice de la colonne à supprimer (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        supprimer_colonne_dataframe(df, col);
                    } else if (choix == 3) {
                        printf("Entrez l'indice de la colonne à renommer (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        printf("Entrez le nouveau nom : ");
                        scanf("%s", nouveau_nom);
                        renommer_titre_colonne(df, col, nouveau_nom);
                    } else if (choix == 4) {
                        afficher_noms_colonnes(df);
                    } else if (choix == 5) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 4: // Options de Cellule
                while (1) {
                    afficher_menu_cellule();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        printf("Entrez l'indice de ligne et de colonne pour obtenir la valeur (format: ligne colonne) :\n");
                        printf("Exemple : 2 3 (pour la 3ème ligne et la 4ème colonne)\n");
                        printf("Entrez l'indice de ligne (0-%u) et de colonne (0-%u) : ", df->colonnes[0]->taille - 1, df->nombre_colonnes - 1);
                        scanf("%d %d", &ligne, &col);
                        void *valeur = obtenir_valeur_cellule(df, ligne, col);
                        if (valeur != NULL) {
                            char buffer[256];
                            convertir_valeur(df->colonnes[col], ligne, buffer, sizeof(buffer));
                            printf("Valeur à [%d, %d] : %s\n", ligne, col, buffer);
                        }
                    } else if (choix == 2) {
                        printf("Entrez l'indice de ligne et de colonne pour définir la valeur, suivi de la valeur à définir (format: ligne colonne valeur) :\n");
                        printf("Exemple : 2 3 10 (pour définir la valeur 10 à la 3ème ligne et la 4ème colonne)\n");
                        printf("Entrez l'indice de ligne (0-%u) et de colonne (0-%u) et la valeur entière : ", df->colonnes[0]->taille - 1, df->nombre_colonnes - 1);
                        int val;
                        scanf("%d %d %d", &ligne, &col, &val);
                        definir_valeur_cellule(df, ligne, col, &val);
                    } else if (choix == 3) {
                        printf("Entrez une valeur à vérifier : ");
                        scanf("%d", &valeur);
                        printf("Existence : %s\n", verifier_existence_valeur(df, &valeur) ? "Oui" : "Non");
                    } else if (choix == 4) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 5: // Options d'Analyse
                while (1) {
                    afficher_menu_analyse();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        afficher_nombre_lignes(df);
                    } else if (choix == 2) {
                        afficher_nombre_colonnes(df);
                    } else if (choix == 3) {
                        printf("Entrez une valeur pour compter les cellules égales à : ");
                        scanf("%d", &valeur);
                        printf("Cellules égales à %d : %d\n", valeur, compter_cellules_egales_a(df, &valeur));
                    } else if (choix == 4) {
                        printf("Entrez une valeur pour compter les cellules supérieures à : ");
                        scanf("%d", &valeur);
                        printf("Cellules supérieures à %d : %d\n", valeur, compter_cellules_superieures_a(df, &valeur));
                    } else if (choix == 5) {
                        printf("Entrez une valeur pour compter les cellules inférieures à : ");
                        scanf("%d", &valeur);
                        printf("Cellules inférieures à %d : %d\n", valeur, compter_cellules_inferieures_a(df, &valeur));
                    } else if (choix == 6) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 6: // Options de Tri
                while (1) {
                    afficher_menu_tri();
                    scanf("%d", &choix);
                    if (choix == 1) {
                        printf("Entrez l'indice de la colonne à trier (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        printf("Entrez la direction du tri (0 pour ascendant, 1 pour descendant) : ");
                        int direction_tri;
                        scanf("%d", &direction_tri);
                        if (col < df->nombre_colonnes) {
                            trier_colonne(df->colonnes[col], direction_tri);
                            printf("Colonne %d triée avec succès.\n", col);
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 2) {
                        printf("Entrez l'indice de la colonne pour afficher les valeurs triées (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        if (col < df->nombre_colonnes) {
                            imprimer_colonne_par_index(df->colonnes[col]);
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 3) {
                        printf("Entrez l'indice de la colonne pour supprimer l'index (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        if (col < df->nombre_colonnes) {
                            effacer_index(df->colonnes[col]);
                            printf("Index supprimé de la colonne %d.\n", col);
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 4) {
                        printf("Entrez l'indice de la colonne pour vérifier la validité de l'index (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        if (col < df->nombre_colonnes) {
                            int etat_index = verifier_index(df->colonnes[col]);
                            if (etat_index == 1) {
                                printf("L'index de la colonne %d est valide.\n", col);
                            } else if (etat_index == -1) {
                                printf("L'index de la colonne %d est invalide.\n", col);
                            } else {
                                printf("La colonne %d n'a pas d'index.\n");
                            }
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 5) {
                        printf("Entrez l'indice de la colonne pour mettre à jour l'index (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        if (col < df->nombre_colonnes) {
                            mettre_a_jour_index(df->colonnes[col]);
                            printf("Index mis à jour pour la colonne %d.\n", col);
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 6) {
                        printf("Entrez l'indice de la colonne pour rechercher une valeur (0-%u) : ", df->nombre_colonnes - 1);
                        scanf("%d", &col);
                        if (col < df->nombre_colonnes) {
                            int valeur_recherchee;
                            printf("Entrez la valeur à rechercher (type int) : ");
                            scanf("%d", &valeur_recherchee); // Ajuster pour d'autres types de données au besoin
                            int trouve = rechercher_valeur_dans_colonne(df->colonnes[col], &valeur_recherchee);
                            printf("La valeur %s trouvée dans la colonne %d.\n", trouve ? "est" : "n'est pas", col);
                        } else {
                            printf("Indice de colonne invalide.\n");
                        }
                    } else if (choix == 7) {
                        break; // Retour au menu principal
                    } else {
                        printf("Option invalide, veuillez réessayer.\n");
                    }
                }
                break;
            case 7: // Quitter
                en_cours = 0;
                break;
            default:
                printf("Option invalide, veuillez réessayer.\n");
                break;
        }
    }

    // Libération des ressources du dataframe
    liberer_dataframe(df);
    return 0;
}

void afficher_menu_principal() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Options de Dataframe\n");
    printf("2. Options de Ligne\n");
    printf("3. Options de Colonne\n");
    printf("4. Options de Cellule\n");
    printf("5. Options d'Analyse\n");
    printf("6. Options de Tri\n");
    printf("7. Quitter\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_dataframe() {
    printf("\n--- Options de Dataframe ---\n");
    printf("1. Afficher le dataframe complet\n");
    printf("2. Afficher un nombre spécifique de lignes\n");
    printf("3. Afficher un nombre spécifique de colonnes\n");
    printf("4. Sauvegarder le dataframe dans un fichier CSV\n");
    printf("5. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_ligne() {
    printf("\n--- Options de Ligne ---\n");
    printf("1. Ajouter une ligne\n");
    printf("2. Supprimer une ligne\n");
    printf("3. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_colonne() {
    printf("\n--- Options de Colonne ---\n");
    printf("1. Ajouter une colonne\n");
    printf("2. Supprimer une colonne\n");
    printf("3. Renommer une colonne\n");
    printf("4. Afficher les noms des colonnes\n");
    printf("5. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_analyse() {
    printf("\n--- Options d'Analyse ---\n");
    printf("1. Afficher le nombre de lignes\n");
    printf("2. Afficher le nombre de colonnes\n");
    printf("3. Afficher le nombre de cellules égales à une valeur donnée\n");
    printf("4. Afficher le nombre de cellules supérieures à une valeur donnée\n");
    printf("5. Afficher le nombre de cellules inférieures à une valeur donnée\n");
    printf("6. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_tri() {
    printf("\n--- Options de Tri ---\n");
    printf("1. Trier une colonne\n");
    printf("2. Afficher une colonne triée\n");
    printf("3. Supprimer l'index d'une colonne\n");
    printf("4. Vérifier la validité d'un index de colonne\n");
    printf("5. Mettre à jour l'index d'une colonne\n");
    printf("6. Rechercher une valeur dans une colonne triée\n");
    printf("7. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}

void afficher_menu_cellule() {
    printf("\n--- Options de Cellule ---\n");
    printf("1. Obtenir et afficher la valeur d'une cellule spécifique\n");
    printf("2. Définir la valeur d'une cellule spécifique\n");
    printf("3. Vérifier l'existence d'une valeur\n");
    printf("4. Retour au menu principal\n");
    printf("Entrez votre choix : ");
}
