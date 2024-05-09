#ifndef COLUMN_H
#define COLUMN_H

#include <stdlib.h>


typedef struct CustomStructure {
    int id;
    double value;
    char description[100];
} CustomStructure;


enum enum_type {
    NULLVAL = 1, UINT, INT, CHAR, FLOAT, DOUBLE, STRING, STRUCTURE
};
typedef enum enum_type ENUM_TYPE;

union column_type {
    unsigned int uint_value;
    unsigned int int_value;
    char char_value;
    float float_value;
    double double_value;
    char* string_value;
    void* struct_value;
};
typedef union column_type COL_TYPE;


struct column {
    char *title;
    unsigned int size;
    unsigned int max_size;
    ENUM_TYPE column_type;
    COL_TYPE **data;
    unsigned long long int *index;
    int valid_index;
    int sort_dir;
};
typedef struct column COLUMN;


COLUMN *create_column(ENUM_TYPE type, char *title);
int insert_value(COLUMN *col, void *value);
void delete_column(COLUMN **col);
void convert_value(COLUMN *col, unsigned long long int index, char *str, int size);
void print_col(COLUMN *col);


int count_occurrences(COLUMN *col, void *value);
void *get_value_at(COLUMN *col, unsigned int index);
int count_greater_than(COLUMN *col, void *value);
int count_less_than(COLUMN *col, void *value);
int count_equal_to(COLUMN *col, void *value);
int compare_values(ENUM_TYPE type, void *data1, void *data2);


#endif // COLUMN_H
