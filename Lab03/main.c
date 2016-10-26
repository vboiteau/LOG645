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
    /*float UPar[np+1][n][m];*/
    int rank;
    int size;
    int TEMPS_ATTENTES=5;
    double timeStart, timeEnd, TSeqExec/*, TParExec*/;
    int i, j, k;
    MPI_Init(&args, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
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
			    (i<n-1?USeq[k-1][j][i+1]:0),
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
	printf("program took %f seconds\n", TSeqExec);
	/*float vec[m*n];*/
	/*agglomeration(m,n,USeq,vec);*/
	/*printf("%d\n",vec[12]);*/
	// End of program code
	// SEQUENTIAL END
    }
    MPI_Finalize();
    return 0;
}
