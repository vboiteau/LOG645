#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "sys/time.h"

int main(int args,char *argv[])
{
    struct timeval tp;
    if(args != 6)
    {
	printf("Il faut absolument avoir 6 arguments (m, n, np, td, h)\n");
	return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int np = atoi(argv[3]);
    float td = atof(argv[4]);
    float h = atof(argv[5]);
    printf("Arguments Values\nm\tn\tnp\ttd\th\n%d\t%d\t%d\t%.2f\t%.2f\n\n",m,n,np,td,h);
    int USeq[np][m][n];
    int UPar[np][m][n];
    int rank;
    int size;
    int TEMPS_ATTENTES=5;
    double timeStart, timeEnd, TSeqExec, TParExec;
    MPI_Init(&args, &argv);
    gettimeofday (&tp, NULL); // Debut du chronometre
    timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    
    // Start of programme code
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // End of program code
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
    if (rank==0){
	printf("program took %f seconds\n", TSeqExec);
    }
    MPI_Finalize();
    return 0;
}
