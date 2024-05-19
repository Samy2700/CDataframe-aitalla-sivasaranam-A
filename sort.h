#ifndef TRI_H
#define TRI_H

#include "column.h"

// Prototypes des fonctions pour le tri des colonnes
void trier_colonne(COLUMN *col, int direction_tri);
void imprimer_colonne_par_index(COLUMN *col);
void effacer_index(COLUMN *col);
int verifier_index(COLUMN *col);
void mettre_a_jour_index(COLUMN *col);
int rechercher_valeur_dans_colonne(COLUMN *col, void *valeur);

// Prototypes des fonctions pour l'algorithme de tri rapide
void echanger_indexes(unsigned long long *a, unsigned long long *b);
void tri_rapide(unsigned long long *index, int bas, int haut, int direction_tri, COLUMN *col);
int partitionner(unsigned long long *index, int bas, int haut, int direction_tri, COLUMN *col);

#endif // TRI_H
