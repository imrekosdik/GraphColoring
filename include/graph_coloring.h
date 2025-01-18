#include "mpi.h"
#include <stdbool.h>

#include "../include/sparse_matrix.h"

#define INVALID_COLOR -1

int* initialize_colored_graph(int number_of_rows);

void partition_graph(int number_of_nodes, int rank, int size, int* start_vertex, int* end_vertex);

int find_smallest_legal_color(CSRMatrix* csr, bool* available, int* colored_graph, int vertex, int start_vertex, int end_vertex);

void color_csr_matrix(CSRMatrix* csr, int rank, int size, int* colored_graph, int start_vertex, int end_vertex);

void print_colored_graph(int* colored_graph, int number_of_rows);

void detect_conflicts(CSRMatrix *csr, int rank, int size, int* colored_graph, int* conflict_table, int start_vertex, int end_vertex);