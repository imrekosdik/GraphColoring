#include "../include/graph_coloring.h"
#include "mpi.h"

int main(int argc, char *argv[])
{
    // Initialize MPI
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    CSRMatrix *csr = create_csr_matrix_from_file("sparse_matrix_examples/G25.mtx");

    int start_vertex, end_vertex;
    partition_graph(csr->number_of_rows, rank, size, &start_vertex, &end_vertex);

    int* colored_graph = initialize_colored_graph(csr->number_of_rows);
    int* global_colored_graph = initialize_colored_graph(csr->number_of_rows);

    color_csr_matrix(csr, rank, size, colored_graph, start_vertex, end_vertex);

    int* send_count = (int*)malloc(size * sizeof(int));
    int* displs = (int*)malloc(size * sizeof(int));

    send_count[rank] = end_vertex - start_vertex + 1;
    MPI_Allgather(&send_count[rank], 1, MPI_INT, send_count, 1, MPI_INT, MPI_COMM_WORLD);
    displs[0] = 0;
    for (int i = 1; i < size; i++) {
        displs[i] = displs[i - 1] + send_count[i - 1];
    }
    MPI_Allgatherv(colored_graph + start_vertex, end_vertex - start_vertex + 1, MPI_INT, global_colored_graph, send_count, displs, MPI_INT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    // Conflict table (local for each process)
    int* conflict_table = (int*)calloc(csr->number_of_rows, sizeof(int));

    create_conflict_table(csr, rank, size, colored_graph, global_colored_graph, conflict_table, start_vertex, end_vertex);

    if (rank == 0) {
        printf("Final sequential coloring after conflict resolution:\n");
        print_colored_graph(global_colored_graph, csr->number_of_rows);
    }

    free(global_colored_graph);

    MPI_Finalize();
    return 0;
}