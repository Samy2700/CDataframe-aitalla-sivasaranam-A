#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "column.h"

// Structure pour un dataframe
typedef struct dataframe {
    COLONNE **colonnes;         // Tableau de pointeurs vers COLUMN
    unsigned int nombre_colonnes;  // Nombre de colonnes dans le dataframe
    unsigned int max_colonnes;   // Capacité maximale du tableau de colonnes
} DATAFRAME;

// Prototypes des fonctions pour gérer le dataframe
DATAFRAME *creer_dataframe();
void remplir_dataframe_par_utilisateur(DATAFRAME *df);
void remplir_dataframe_statique(DATAFRAME *df);
TYPE_ENUM analyser_type(const char *typeStr);
void *lire_donnee_par_type(TYPE_ENUM type);
void liberer_dataframe(DATAFRAME *df);

// Prototypes des fonctions pour afficher le dataframe
void afficher_dataframe_complete(DATAFRAME *df);
void afficher_lignes_dataframe(DATAFRAME *df, unsigned int lignes);
void afficher_colonnes_dataframe(DATAFRAME *df, unsigned int colonnes);

// Prototypes des fonctions pour les opérations usuelles
void ajouter_ligne_dataframe(DATAFRAME *df, void **donnees_ligne);
void supprimer_ligne_dataframe(DATAFRAME *df, unsigned int indice_ligne);
int ajouter_colonne_dataframe(DATAFRAME *df, COLONNE *col);
void supprimer_colonne_dataframe(DATAFRAME *df, unsigned int index);
void renommer_titre_colonne(DATAFRAME *df, unsigned int indice_colonne, const char *nouveau_titre);
int verifier_existence_valeur(DATAFRAME *df, void *valeur);
void *obtenir_valeur_cellule(DATAFRAME *df, unsigned int ligne, unsigned int colonne);
void definir_valeur_cellule(DATAFRAME *df, unsigned int ligne, unsigned int colonne, void *valeur);
void afficher_noms_colonnes(DATAFRAME *df);

// Prototypes des fonctions pour l'analyse et les statistiques
void afficher_nombre_lignes(DATAFRAME *df);
void afficher_nombre_colonnes(DATAFRAME *df);
int compter_cellules_egales_a(DATAFRAME *df, void *valeur);
int compter_cellules_superieures_a(DATAFRAME *df, void *valeur);
int compter_cellules_inferieures_a(DATAFRAME *df, void *valeur);

// Prototypes des nouvelles fonctions pour la gestion des fichiers CSV
DATAFRAME *charger_depuis_csv(char *nom_fichier, TYPE_ENUM *dftype, int taille);
void sauvegarder_dans_csv(DATAFRAME *cdf, char *nom_fichier);

#endif // CDATAFRAME_H
