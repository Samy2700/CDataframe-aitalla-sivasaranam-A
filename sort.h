#ifndef TRI_H
#define TRI_H

#include "column.h"

// Prototypes des fonctions pour le tri des colonnes
void trier_colonne(COLONNE *col, int direction_tri);
void imprimer_colonne_par_index(COLONNE *col);
void effacer_index(COLONNE *col);
int verifier_index(COLONNE *col);
void mettre_a_jour_index(COLONNE *col);
int rechercher_valeur_dans_colonne(COLONNE *col, void *valeur);

// Prototypes des fonctions pour l'algorithme de tri rapide
void echanger_indexes(unsigned long long *a, unsigned long long *b);
void tri_rapide(unsigned long long *index, int bas, int haut, int direction_tri, COLONNE *col);
int partitionner(unsigned long long *index, int bas, int haut, int direction_tri, COLONNE *col);

#endif // TRI_H
