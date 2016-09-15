#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "sys/time.h"

int main(int args,char *argv[])
{
    double timeStart, timeEnd, Texec;
    struct timeval tp;
    gettimeofday (&tp, NULL); // Debut du chronometre
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    // Start of programme code
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
	int len = 64;
	int i=rank;
	while(i<len){
	    printf("pos %d\n",i);
	    i=i+size;
	}	
    }	
    printf("Process %d of %d\nTest: %i, %i, %i\n\n", rank+1, size, c, p, n);
    MPI_Finalize();
    // End of program code
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    printf ("%f secs\n", Texec);
    return 0;

}
