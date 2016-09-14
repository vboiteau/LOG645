#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int args,char *argv[])
{
	if(args != 4)
	{
		printf("Il faut absolument avoir 3 arguments\n");
	 	return 1;
	}
	int c = atoi(argv[1]);
	int p = atoi(argv[2]);
	int n = atoi(argv[3]);
	int rank;
	int size;
	MPI_Init(&args, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	if(c == 1)
	{
		
	}	
	printf("Process %d of %d\nTest: %i, %i, %i\n\n", rank+1, size, c, p, n);
	MPI_Finalize();
	return 0;
}
