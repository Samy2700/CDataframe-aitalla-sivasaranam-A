#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "column.h"

typedef struct {
    COLUMN **columns;       // Dynamic array of pointers to COLUMN
    unsigned int num_cols;  // Number of columns
    unsigned int capacity;  // Current allocated capacity for columns array
} CDATAFRAME;

CDATAFRAME *create_cdataframe(unsigned int initial_capacity);
void delete_cdataframe(CDATAFRAME **cdf_ptr);
void add_column(CDATAFRAME *cdf, COLUMN *col);
void remove_column(CDATAFRAME *cdf, const char *title);
void print_cdataframe(CDATAFRAME *cdf);

#endif 
