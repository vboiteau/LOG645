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
	number[0]=p;
	number[1]=n;
	if (size<16){
	    MPI_Finalize();
	    if(rank==0){printf("Mininum of 16 processors\n");}
	    return 1;
	}
	if (c==1) {
	    if (rank ==0) {
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
	    } else {
		int counter=rank;
		while (counter <= n*columns*rows) {
		    MPI_Recv(&number, 5, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		    if (c==1){
			usleep(1000);
			number[0] = number[0] + (number[2]+number[3])*number[4];
		    } else {
			usleep(1000);
			number[0] = number[0]+number[2]*number[4]*pow(number[1],number[3]);
			/*number[0]=base*(pow((double)number[1],(double)number[3])+pow((double)number[1]-1,(double)));*/
		    }
		    MPI_Send(&number, 5,MPI_INT, 0, rank,MPI_COMM_WORLD);
		    counter+=(size-1);
		}
	    }
	} else if (c== 2) {
	    int rowMessage [columns+2];
	    if (rank == 0) {
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
		for(k=1;k<=n;k++){
		    for(i=0;i<2;i++)
		    {
			memcpy(rowMessage,matrix[k-1][i],columns*sizeof(int));
			rowMessage[columns]=k;
			rowMessage[columns+1]=i;
			MPI_Send(&rowMessage, columns+2, MPI_INT,i+1, 0, MPI_COMM_WORLD);
		    }
		    for (i = 0; i < 2; ++i) {
		        MPI_Recv(&rowMessage, columns+2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			memcpy(matrix[k][rowMessage[columns+1]],rowMessage,columns*sizeof(int));
		    }
		    for (i = 0; i < rows-2; ++i) {
			for (j = 0; j < columns; j++) {
			    number [0] = matrix[k][0][j];
			    number [1] = matrix[k][1][j];
			    number [2] = k;
			    number [3] = i+2;
			    number [4] = j;
			    MPI_Send(&number, 5, MPI_INT, (process_cursor+3), 0, MPI_COMM_WORLD);
			    process_cursor ++;
			    process_cursor%=(size-3);
			}
		    }
		}
		int out[5];
		for(i=0;i<n*columns*(rows-2);i++)
		{
		    MPI_Recv(&out, 5, MPI_INT,MPI_ANY_SOURCE , 1, MPI_COMM_WORLD,MPI_STATUS_IGNORE);	
		    matrix[out[2]][out[3]][out[4]]=out[0];
		}
		for (k=0; k<=n; k++){
		    printf("%d alteration matrix\n",k);
		    for (i = 0; i < rows; i++) {
			for (j = 0; j< columns; j++) {
			    printf("%d\t",matrix[k][i][j]);
			}
			printf("\n");
		    }
		    printf("\n");
		}
	    } else if(rank == 1 || rank == 2){
		int k,i;
		for (k = 1; k <= n ; k++) {
		    MPI_Recv(&rowMessage, columns+2, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    for (i = 0; i < columns; ++i) {
			int modif = rowMessage[columns+1]*rowMessage[columns];
			if (i>0) {
			    modif = rowMessage[i-1]*rowMessage[columns];
			}
			usleep(1000);
			rowMessage[i]=rowMessage[i]+modif;
		    }
		    MPI_Send(&rowMessage, columns+2, MPI_INT, 0, 0, MPI_COMM_WORLD);
		}
	    } else {
		int counter = (rank-3);
		while (counter < n*columns*(rows-2)){
		    MPI_Recv(&number, 5, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		    usleep(1000);
		    number[0]=number[0]+(number[1]-number[0])*number[3];
		    MPI_Send(&number, 5, MPI_INT, 0, 1, MPI_COMM_WORLD);
		    counter+=(size-3);
		}
	    }
	}
	if (rank==0) {
	    gettimeofday (&tp, NULL); // Fin du chronometre
	    procTimeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	    Texec = procTimeEnd - procTimeStart; //Temps d'execution en secondes
	    printf("proc %d\t%f\n", rank, Texec);
	}
	// End of program code
	MPI_Finalize();
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	if (rank==0){
	    printf("program took %f seconds\n", Texec);
	}
	return 0;
}
