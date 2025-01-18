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
        }

        // Update row_pointers based on row_counts
        csr->row_pointers[0] = 0;
        for (int i = 1; i <= csr->number_of_rows; i++)
        {
            csr->row_pointers[i] = csr->row_pointers[i - 1] + row_counts[i - 1];
        }
    }

    // Rewind the file pointer to read edges again and populate column_indices
    rewind(file);
    int *temp_columns = (int *)malloc(csr->number_of_nonzeros * sizeof(int));

    // Skip the graph dimensions line
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '%') {
            continue;
        }
        // Now populate the column_indices based on the edges
        if (fscanf(file, "%d %d", &row, &column) == 2) {
            row--;    // Adjust to 0-based indexing
            column--; // Adjust to 0-based indexing

            // Calculate the index to place the column index
            int index = csr->row_pointers[row] + (row_counts[row] - 1);
            if (index == -1)
                index = 0;
            temp_columns[index] = column;
            // Decrement the count for the current row as we add an element
            row_counts[row]--;
        }
    }

    for (int i = 0; i < csr->number_of_rows; i++) {
        int start = csr->row_pointers[i];
        int end = csr->row_pointers[i + 1] - 1;
        while (start < end) {
            // Swap column indices to reverse the order
            int temp = temp_columns[start];
            temp_columns[start] = temp_columns[end];
            temp_columns[end] = temp;
            start++;
            end--;
        }
    }

    memcpy(csr->column_indices, temp_columns, csr->number_of_nonzeros * sizeof(int));
    // Free row_counts after use
    free(row_counts);

    
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