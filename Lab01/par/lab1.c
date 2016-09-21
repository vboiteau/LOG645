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
		int number[4] = {p,n,1,1};
		if(rank ==0)
		    {
			int i;
			int j;
			for(i=1;i<size;i++)
			{	
				MPI_Send(&number, 4, MPI_INT,i, 0, MPI_COMM_WORLD);
			}
			for(i=1;i<size;i++)
			{	
				 MPI_Recv(&number, 4, MPI_INT,MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);	
				printf("%d\n",number[3]);
			}
			 }
		    else
		    {
			    MPI_Recv(&number, 4, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			    int i;
			int solve[4];
			for(i=0;i<4;i++)
			{
			solve[i] = number[i] * rank;
			}
			    MPI_Send(&solve, 4,MPI_INT, 0, rank,MPI_COMM_WORLD);
		    }
    }
    
    // End of program code
    gettimeofday (&tp, NULL); // Fin du chronometre
    procTimeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = procTimeEnd - procTimeStart; //Temps d'execution en secondes
   // printf("%d\t%f\n", rank+1, Texec);
    MPI_Finalize();
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    printf("program took %f secondes\n", Texec);
    return 0;
}
