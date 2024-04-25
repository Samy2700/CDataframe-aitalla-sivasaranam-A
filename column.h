#ifndef COLUMN_H
#define COLUMN_H

typedef enum {
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
} ENUM_TYPE;

typedef struct {
    char *title;
    int *data;
    unsigned int size;
    unsigned int max_size;
} COLUMN;

COLUMN *create_column(char *title);
int insert_value(COLUMN *col, int value);
void print_col(COLUMN *col);
void delete_column(COLUMN **col);


#endif // COLUMN_H
