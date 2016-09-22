#include <mpi.h>
#include <math.h>
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
    int columns = 8;
    int rows = 8;
    int rank;
    int size;
    MPI_Init(&args, &argv);
    gettimeofday (&tp, NULL); // Debut du chronometre
    procTimeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    // Start of programme code
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	int number[5];
	if(rank ==0)
	{
	    int i;
	    int j;
	    int k;
	    int matrix[(n+1)][rows][columns];
	    for(k=0;k<=n;k++){
		for(i=0;i<rows;i++){
		    for(j=0;j<columns;j++){
			matrix[k][i][j]=p;
		    }
		}
	    }
	    int process_cursor=0;
	    int suite=0;
	    number[0]=matrix[0][0][0];
	    for(k=1;k<=n;k++){
		suite+=k;
		number[1]=k;
		number[4]=suite;
		for(i=0;i<rows;i++)
		{
		    number[2]=i;
		    for (j = 0; j < columns; j++) {
			number[3]=j;
			MPI_Send(&number, 5, MPI_INT,process_cursor+1, 0, MPI_COMM_WORLD);
			process_cursor++;
			process_cursor %= size-1;
		    }
		}
	    }
	    int out[5];
	    for(i=0;i<n*columns*rows;i++)
	    {
		MPI_Recv(&out, 5, MPI_INT,MPI_ANY_SOURCE , MPI_ANY_TAG, MPI_COMM_WORLD,MPI_STATUS_IGNORE);	
		matrix[out[1]][out[2]][out[3]]=out[0];
	    }
	    /*for (k=0; k<=n; k++){*/
		/*printf("%d alteration matrix\n",k);*/
		/*for (i = 0; i < rows; i++) {*/
		    /*for (j = 0; j< columns; j++) {*/
			/*printf("%d\t",matrix[k][i][j]);*/
		    /*}*/
		    /*printf("\n");*/
		/*}*/
		/*printf("\n");*/
	    /*}*/
	}
	else
	{
	    int counter=rank;
	    while (counter <= n*columns*rows) {
		MPI_Recv(&number, 5, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		if (c==1){
		    number[0] = number[0] + (number[2]+number[3])*number[4];
		} else {
		    number[0] = number[0]+number[2]*number[4];
		    /*number[0]=base*(pow((double)number[1],(double)number[3])+pow((double)number[1]-1,(double)));*/
		}
		MPI_Send(&number, 5,MPI_INT, 0, rank,MPI_COMM_WORLD);
		counter+=(size-1);
	    }
	}

    // End of program code
    gettimeofday (&tp, NULL); // Fin du chronometre
    procTimeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = procTimeEnd - procTimeStart; //Temps d'execution en secondes
     printf("proc %d\t%f\n", rank, Texec);
    MPI_Finalize();
    gettimeofday (&tp, NULL); // Fin du chronometre
    timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
    Texec = timeEnd - timeStart; //Temps d'execution en secondes
    if (rank==0){
	printf("program took %f seconds\n", Texec);
    }
    return 0;
}
