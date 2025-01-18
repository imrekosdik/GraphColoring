#include "../include/graph_coloring.h"


void color_csr_matrix(CSRMatrix* csr, int rank, int size, int* colored_graph, int start_vertex, int end_vertex)
{
    bool available[csr->number_of_rows];
    for (int vertex = start_vertex; vertex <= end_vertex; vertex++) {
        for (int color = 0; color < csr->number_of_rows; color++) {
            available[color] = true;
        }
        int smallest_legal_color = find_smallest_legal_color(csr, available, colored_graph, vertex, start_vertex, end_vertex);
        colored_graph[vertex] = smallest_legal_color;
    }

    // Barrier synchronization after all processes have assigned their colors
    MPI_Barrier(MPI_COMM_WORLD);
    // print_colored_graph(colored_graph, csr->number_of_rows);
}


void print_colored_graph(int* colored_graph, int number_of_rows)
{
    for (int i = 0; i < number_of_rows; i++) {
        printf("Vertex %d: Color %d\n", i, colored_graph[i]);
    }
}


int* initialize_colored_graph(int number_of_rows)
{
    int* colored_graph = (int*)malloc(sizeof(int) * number_of_rows);
    for (int i = 0; i < number_of_rows; i++)
    {
        colored_graph[i] = INVALID_COLOR;
    }
    return colored_graph;
}


void partition_graph(int number_of_nodes, int rank, int size, int* start_vertex, int* end_vertex)
{
    int vertices_per_process = number_of_nodes / size;
    *start_vertex = rank * vertices_per_process;
    *end_vertex = *start_vertex + vertices_per_process - 1; 
    if (rank == size - 1)
    {
        *end_vertex = *start_vertex + vertices_per_process + (number_of_nodes % size) - 1;
    }
}


int find_smallest_legal_color(CSRMatrix* csr, bool* available, int* colored_graph, int vertex, int start_vertex, int end_vertex)
{
    int start_neighbor = csr->row_pointers[vertex];
    int end_neighbor = csr->row_pointers[vertex + 1];
    int smallest_legal_color = INVALID_COLOR;

    // Mark unavailable colors based on neighbors' colors
    for (int neighbor_index = start_neighbor; neighbor_index < end_neighbor; neighbor_index++) {
        int neighbor = csr->column_indices[neighbor_index];

        // Ensure neighbor is valid and colored
        if (neighbor >= 0 && neighbor < csr->number_of_rows && colored_graph[neighbor] != INVALID_COLOR) {
            available[colored_graph[neighbor]] = false;
        }
    }

    // Find the smallest available color
    for (int i = 0; i < csr->number_of_rows; i++) {
        if (available[i]) {
            smallest_legal_color = i;
            break;
        }
    }
    return smallest_legal_color;
}