#include "column.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REALLOC_SIZE 256

COLUMN *create_column(char *title) {
    COLUMN *col = (COLUMN *)malloc(sizeof(COLUMN));
    if (!col) return NULL;
    col->title = strdup(title);
    col->data = NULL;
    col->size = 0;
    col->max_size = 0;
    return col;
}

int insert_value(COLUMN *col, int value) {
    if (col->size == col->max_size) {
        int new_max_size = col->max_size + REALLOC_SIZE;
        int *new_data = realloc(col->data, new_max_size * sizeof(int));
        if (!new_data) return 0;
        col->data = new_data;
        col->max_size = new_max_size;
    }
    col->data[col->size] = value;
    col->size++;
    return 1;
}

void print_col(COLUMN *col) {
    printf("Column: %s\n", col->title);
    for (unsigned int i = 0; i < col->size; i++) {
        printf("[%u] %d\n", i, col->data[i]);
    }
}

void delete_column(COLUMN **col) {
    if (!col || !*col) return;
    free((*col)->title);
    free((*col)->data);
    free(*col);
    *col = NULL;
}

