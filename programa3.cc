#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int MPI_Allreduce(const void* send_buffer,
                  void* receive_buffer,
                  int count,
                  MPI_Datatype datatype,
                  MPI_Op operation,
                  MPI_Comm communicator);

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
 
    // Get the size of the communicator
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size != 3)
    {
        printf("This application is meant to be run with 3 MPI processes.\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    // Get my rank
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 
    // Each MPI process sends its rank to reduction, root MPI process collects the result
    int reduction_result = 0;
    MPI_Allreduce(&my_rank, &reduction_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
 
    printf("[MPI Process %d] The sum of all ranks is %d.\n", my_rank, reduction_result);
 
    MPI_Finalize();
 
    return EXIT_SUCCESS;
}
