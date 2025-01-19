#include "../include/sparse_matrix.h"
#include <string.h>  // Include for memcpy

CSRMatrix* create_csr_matrix_from_file(const char *file_name)
{
    FILE *file = fopen(file_name, "r");

    char line[256];
    CSRMatrix *csr = NULL;
    int graphInfoLine = 1;  // Flag to indicate when to read the graph dimensions
    int number_of_rows, number_of_columns, number_of_nonzeros;
    // Process the non-zero elements
    int *row_counts;

    // Read non-zero elements from the file
    int row, column;

    // First pass to process the file
    while (fgets(line, sizeof(line), file)) {
        // Skip lines starting with '%' (comment lines)
        if (line[0] == '%') {
            continue;
        }

        // Read the graph dimensions (rows, columns, nonzeros) once
        if (graphInfoLine) {
            sscanf(line, "%d %d %d", &number_of_rows, &number_of_columns, &number_of_nonzeros);
            number_of_nonzeros *= 2;
            row_counts = (int *)calloc(number_of_rows, sizeof(int));  // To count non-zeros per row

            // Allocate memory for the CSR matrix
            csr = (CSRMatrix *)malloc(sizeof(CSRMatrix));
            csr->number_of_rows = number_of_rows;
            csr->number_of_columns = number_of_columns;
            csr->number_of_nonzeros = number_of_nonzeros;

            // Allocate memory for the arrays);
            csr->column_indices = (int *)malloc(number_of_nonzeros * sizeof(int));
            csr->row_pointers = (int *)malloc((number_of_rows + 1) * sizeof(int));

            // Initialize row pointers to 0
            for (int i = 0; i <= number_of_rows + 1; i++) {
                csr->row_pointers[i] = 0;
            }

            graphInfoLine = 0;  // Stop looking for the graph dimension line
        }

        if (fscanf(file, "%d %d", &row, &column) == 2) {
            row--;   // Adjust to 0-based indexing
            column--; // Adjust to 0-based indexing
            row_counts[row]++;
            row_counts[column]++;
        }

        // Update row_pointers based on row_counts
        csr->row_pointers[0] = 0;
        for (int i = 1; i <= csr->number_of_rows; i++)
        {
            csr->row_pointers[i] = csr->row_pointers[i - 1] + row_counts[i - 1];
        }
    }

    // Rewind the file and populate column_indices
    rewind(file);
    int *current_position = (int *)malloc(number_of_rows * sizeof(int));
    memcpy(current_position, csr->row_pointers, number_of_rows * sizeof(int));

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '%') {
            continue;  // Skip comment lines
        }

        int row, column;
        if (fscanf(file, "%d %d", &row, &column) == 2) {
            row--;    // Adjust to 0-based indexing
            column--; // Adjust to 0-based indexing

            // Add (row, column)
            csr->column_indices[current_position[row]++] = column;

            // Add (column, row) for symmetry
            csr->column_indices[current_position[column]++] = row;
        }
    }

    // Free temporary arrays
    free(row_counts);
    free(current_position);


    // Close the file
    fclose(file);
    return csr;
}

void print_csr_matrix(CSRMatrix *csr)
{
    printf("CSR Matrix Representation:\n");
    // Print Row Pointers
    printf("Row Pointers: ");
    for (int i = 0; i <= csr->number_of_rows; i++) {
        printf("%d ", csr->row_pointers[i]);
    }
    printf("\n");
    // Print Column Indices
    printf("Column Indices: ");
    for (int i = 0; i < csr->number_of_nonzeros; i++) {
        printf("%d ", csr->column_indices[i]);
    }
    printf("\n");
}