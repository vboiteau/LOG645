#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "sys/time.h"

int main(int args,char *argv[])
{
    struct timeval tp;
    double procTimeStart, procTimeEnd, timeStart, timeEnd, Texec;
    gettimeofday (&tp, NULL); // Debut du chronometre
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
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
    gettimeofday (&tp, NULL); // Debut du chronometre
    procTimeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    // Start of programme code
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(c == 1)
    {
	int len = 64;
	MPI_Bcast ( &len, 1, MPI_INT, 0, MPI_COMM_WORLD );
	int i=rank;
	while(i<len){
	    printf("from %d pos %d\n",rank, i);
	    i=i+size;
	    usleep(1000);
	}	
    }
    
    // End of program code
    gettimeofday (&tp, NULL); // Fin du chronometre
    procTimeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = procTimeEnd - procTimeStart; //Temps d'execution en secondes
    printf("%d\t%f\n", rank+1, Texec);
    MPI_Finalize();
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    printf("program took %f secondes\n", Texec);
    return 0;
}
