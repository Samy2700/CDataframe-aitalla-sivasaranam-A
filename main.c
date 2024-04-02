#include "column.h"
#include <stdio.h>

int main() {
    COLUMN *mycol = create_column("My Int Column");
    if (!mycol) {
        printf("Failed to create column\n");
        return 1;
    }

    insert_value(mycol, 10);
    insert_value(mycol, 20);
    insert_value(mycol, 10);
    insert_value(mycol, 30);

    print_col(mycol);

    delete_column(&mycol);

    return 0;
}
