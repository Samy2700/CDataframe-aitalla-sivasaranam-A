#ifndef SORT_H
#define SORT_H

#include "column.h"



void sort_column(COLUMN col, int sort_dir);
void print_col_by_index(COLUMNcol);
void erase_index(COLUMN col);
int check_index(COLUMNcol);
void update_index(COLUMN col);
int search_value_in_column(COLUMNcol, void val);

void swap_indexes(unsigned long longa, unsigned long long b);
void quick_sort(unsigned long longindex, int low, int high, int sort_dir, COLUMN col);
int partition(unsigned long longindex, int low, int high, int sort_dir, COLUMN *col);

#endif // SORT_H
