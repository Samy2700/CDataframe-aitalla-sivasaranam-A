#ifndef CDATAFRAME_H
#define CDATAFRAME_H

#include "column.h"

typedef struct {
    COLUMN **columns;
    unsigned int num_cols;
    unsigned int capacity;
} CDATAFRAME;

CDATAFRAME *create_cdataframe(unsigned int initial_capacity);
void delete_cdataframe(CDATAFRAME **cdf_ptr);
void add_column(CDATAFRAME *cdf, COLUMN *col);
void remove_column(CDATAFRAME *cdf, const char *title);
void print_cdataframe(CDATAFRAME *cdf);

#endif
