#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int *column_indices;
    int *row_pointers;
    int number_of_rows;
    int number_of_columns;
    int number_of_nonzeros;
} CSRMatrix;


CSRMatrix* create_csr_matrix_from_file(const char *file_name);

void print_csr_matrix(CSRMatrix *csr);