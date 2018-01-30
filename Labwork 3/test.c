#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, world_rank, world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }	

    char file_name[100], *buffer;       
    int filelen, to_rank; 
    FILE *pfile;
    int number;
  if (world_rank == 0) {
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

    // printf("Enter rank: ");
    // scanf("%d", &to_rank);
    MPI_Send(&filelen, 1, MPI_INT, 1, 2, MPI_COMM_WORLD);
    printf("sent filelen\n");
    MPI_Send(buffer, filelen, MPI_BYTE, 1, 3, MPI_COMM_WORLD);
    printf("sent\n");

  }
  else if (world_rank == 1)
  {
    // file transferring 
    // printf("Enter file name: ");
    // scanf("%s", file_name);
    // printf("Enter rank: ");
    // scanf("%d", &from_rank);
    MPI_Recv(&filelen, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received filelen\n");
    buffer = (char *)malloc((filelen + 1) * sizeof(char));      
    MPI_Recv(buffer, filelen, MPI_BYTE, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Received\n");
    pfile = fopen("Received", "wb");
    // buffer = (char *)malloc((filelen + 1) * sizeof(char));
    while (filelen > 0)
    {
        int written = fwrite(buffer, 1, filelen, pfile);
        buffer += written;
        filelen -= written;
    }
    fclose(pfile);
  }
  MPI_Finalize();
  return 0;
}