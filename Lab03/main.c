#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "printer.h"
#include "sys/time.h"
#include "segmentation.h"
#include "mod.c"
float getFirst (int m, int n, int i, int j) { 
    float value = (float)(i * (m - i - 1) * j * (n - j -1));
    return value;
}

float addSides(float top, float right, float bottom, float left) {
    return top + right + bottom + left;
}

float getInstantValue (float mod, float old, float top, float right, float bottom, float left) {
    return (1-(4*mod))*old+mod*(addSides(top, right, bottom, left));
}

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
    float mod = getMod(td, h);
    float USeq[np+1][n][m];
    float UPar[np+1][n][m];
    int rank;
    int size;
    int TEMPS_ATTENTES=5;
    double timeStart, timeEnd, TSeqExec, TParExec;
    int i, j, k;
    MPI_Init(&args, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    float input [8] = {0.0};
    float output [4] = {0.0};
    if(rank == 0) {
	// SEQUENTIAL BEGIN
	printf("Arguments Values\nm\tn\tnp\ttd\th\n%d\t%d\t%d\t%.2f\t%.2f\n\n",m,n,np,td,h);
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	for (k=0;k<=np;k++){
	    for (j = 0; j < n; ++j) {
		for (i = 0; i < m; i++) {
		    usleep(TEMPS_ATTENTES);
		    if(k>0) {
			USeq[k][j][i] = getInstantValue(
			    mod,
			    USeq[k-1][j][i],
			    (j>0?USeq[k-1][j-1][i]:0),
			    (i<m-1?USeq[k-1][j][i+1]:0),
			    (j<n-1?USeq[k-1][j+1][i]:0),
			    (i>0?USeq[k-1][j][i-1]:0)
			);
		    } else {
			USeq[k][j][i] = getFirst(m,n,i,j); 
		    } 
		}
	    }
	}
	printResult(np, n, m, USeq);
	// Start of programme code
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
	gettimeofday(&tp, NULL);
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec)/1e6;
	int proc_cursor = 0;
	for (k = 0;k <= np;k++) {
	    input[0]=(float)k;
	    for (j = 0; j < n; j++) {
		input[1]=(float)j;
		for (i = 0; i < m; i++) {
		    input[2]=(float)i;
		    if (k>0) {
		        input[3]=UPar[k-1][j][i];
			input[4]=(j>0?UPar[k-1][j-1][i]:0);
			input[5]=(i<m-1?UPar[k-1][j][i+1]:0);
			input[6]=(j<n-1?UPar[k-1][j+1][i]:0);
			input[7]=(i>0?UPar[k-1][j][i-1]:0);
		    }
		    MPI_Send(&input, 8, MPI_FLOAT, (proc_cursor+1), 0, MPI_COMM_WORLD);
		    proc_cursor++;
		    proc_cursor%=(size-1);
		}
	    }

	    for (i = 0; i < n*m; ++i) {
	        MPI_Recv(&output, 4, MPI_FLOAT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		UPar[(int) output[0]][(int) output[1]][(int) output[2]]=output[3];
	    }
	}
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	TParExec = timeEnd - timeStart; //Temps d'execution en secondes
	printResult(np, n, m, UPar);
	printf("T1\t%.2f ms\nTP\t%.2f ms\nS\t%.2f\nE\t%.2f %\n\n", TSeqExec*1000, TParExec*1000, (TSeqExec/TParExec),((TSeqExec/TParExec)/size)*100);
	/*float vec[m*n];*/
	/*agglomeration(m,n,USeq,vec);*/
	/*printf("%d\n",vec[12]);*/
	// End of program code
	// SEQUENTIAL END
    } else {
	for (i = rank; i < ((np+1)*m*n+1); i=i+(size-1)) {
	    MPI_Recv(&input, 8, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    output[0] = input[0];
	    output[1] = input[1];
	    output[2] = input[2];
	    usleep(TEMPS_ATTENTES);
	    if (input[0]>0) {
		output[3] = getInstantValue(mod, input[3], input[4], input[5], input[6], input[7]);
	    } else {
		output[3] = getFirst(m, n, input[2], input[1]);
	    }
	    MPI_Send(&output, 4, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
	}
    }
    MPI_Finalize();
    return 0;
}
