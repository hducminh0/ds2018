#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char const *argv[])
{
	 MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

    // variables declaration
    char file_name[100], *buffer;		
    int filelen, to_rank; 
    FILE *pfile;

    // file transferring 
    printf("Enter file name: ");
    scanf("%s", file_name);
    pfile = fopen(file_name, "rb");
    fseek(pfile, 0, SEEK_END);      // go to the end of the file
    filelen = ftell(pfile);         // length of the file
    rewind(pfile);          // set the pointer back to the start of the file 
	buffer = (char *)malloc((filelen + 1) * sizeof(char));       // create a buffer to 
    // printf("buffer: %lu\n", sizeof(buffer));
    fread(buffer, filelen, 1, pfile);
    fclose (pfile);	

    printf("Enter rank: ");
    scanf("%d", &to_rank);

    MPI_Send(buffer, filelen, MPI_BYTE, to_rank, 3, MPI_COMM_WORLD);

    // Finalize the MPI environment.
    MPI_Finalize();
	return 0;
}