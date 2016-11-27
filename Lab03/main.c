/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file of lab 3 - Heat propagation
 *
 *        Version:  1.0
 *        Created:  11/06/2016 12:53:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vincent Boiteau-Robert (vboiteau), vboiteau94@gmail.com
 *		    Julien Lemonde
 *   Organization:  ETS
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#include "printer.hpp"
#include "segmentation.h"
    
unsigned int TEMPS_ATTENTES=5;

/* #####   FUNCTION DEFINITIONS  -  LOCAL TO THIS SOURCE FILE   ##################### */
float generateFirstInstanceCell (int m, int n, int i, int j);
float sumImmediateNeighbours (float top, float right, float bottom, float left);
float processTimeEffectOnCell (float mod, float old, float top, float right, float bottom, float left);
float getMod (float td, float h);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  start point of program
 * =====================================================================================
 */
int main(int args,char *argv[]) {
    if (args != 6) {
	printf("Il faut absolument avoir 6 arguments (m, n, np, td, h)\n");
	return 1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int np = atoi(argv[3]);
    float td = atof(argv[4]);
    float h = atof(argv[5]);
    float mod = getMod(td, h);
    float USeq[2][n][m];
    int debug = 0;
    int sequentielle = 0;
    int rank;
    int size;
    struct timeval tseq, tpar;
    double timeStart, timeEnd, TSeqExec, TParExec;
    int i, j, k;
    MPI_Init(&args, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    float input [8] = {0};
    float output [4] = {0};
    /*-----------------------------------------------------------------------------
     *  Sequential code
     *-----------------------------------------------------------------------------*/
    if(rank == 0 && sequentielle) {
        printf("Arguments Values\nm\tn\tnp\ttd\th\n%d\t%d\t%d\t%.2f\t%.2f\n\n",m,n,np,td,h);
        for (j = 0; j < n; ++j) {
            for (i = 0; i < m; ++i) {
                USeq[0][j][i] = generateFirstInstanceCell(m,n,i,j); 
            }
        }
        if (debug) {
            printResult(0, n, m, USeq);
        }
        gettimeofday (&tseq, NULL); // Debut du chronometre
        timeStart = (double) (tseq.tv_sec) + (double) (tseq.tv_usec) / 1e6;
        for (k=1;k<=np;k++){
            for (j = 0; j < n; ++j) {
                for (i = 0; i < m; i++) {
                    USeq[k%2][j][i] = processTimeEffectOnCell(
                            mod,
                            USeq[(k-1)%2][j][i],
                            (j>0?USeq[(k-1)%2][j-1][i]:0),
                            (i<m-1?USeq[(k-1)%2][j][i+1]:0),
                            (j<n-1?USeq[(k-1)%2][j+1][i]:0),
                            (i>0?USeq[(k-1)%2][j][i-1]:0)
                            );
                }
            }
        }
        gettimeofday (&tseq, NULL); // Fin du chronometre
        timeEnd = (double) (tseq.tv_sec) + (double) (tseq.tv_usec) / 1e6;
        TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
        if (debug) {
            printResult(np%2, n, m, USeq);
        }
    }
    /*-----------------------------------------------------------------------------
     *  End of sequential code
     *  Parallel code
     *-----------------------------------------------------------------------------*/
    int partitionType = 0;//checkPartitionType(size, m, n);
    float UPar[2][n][m];
    if (rank == 0) {
        for (j = 0; j < (partitionType<2?n:m); ++j) {
            for (i = 0; i < (partitionType<2?m:n); ++i) {
                UPar[0][j][i] = generateFirstInstanceCell(m,n,i,j); 
            }
        }
        if (debug) {
            printResult(0, n, m, UPar);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank==0) {
	gettimeofday(&tpar, NULL);
	timeStart = (double) (tpar.tv_sec) + (double) (tpar.tv_usec)/1e6;
	int proc_cursor = 0;
	for (k = 1;k <= np;k++) {
	    input[0]=(float)k;
	    for (j = 0; j < (partitionType<2?n:m); j++) {
		input[1]=(float)j;
                if (partitionType==0) {
                    for (i = 0; i < (partitionType<2?m:n); i++) {
                        input[2]=(float)i;
                        input[3]=UPar[(k-1)%2][j][i];
                        input[4]=(j>0?UPar[(k-1)%2][j-1][i]:0);
                        input[5]=(i<m-1?UPar[(k-1)%2][j][i+1]:0);
                        input[6]=(j<n-1?UPar[(k-1)%2][j+1][i]:0);
                        input[7]=(i>0?UPar[(k-1)%2][j][i-1]:0);
                        MPI_Send(&input, 8, MPI_FLOAT, (proc_cursor+1), 0, MPI_COMM_WORLD);
                        proc_cursor++;
                        proc_cursor%=(size-1);
                    }
                }
	    }
            if (partitionType == 0) {
                for (i = 0; i < n*m; ++i) {
                    MPI_Recv(&output, 4, MPI_FLOAT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    UPar[((int) output[0])%2][(int) output[1]][(int) output[2]]=output[3];
                }
            }
	}
    } else {
	if (partitionType == 0) {
            for (i = rank; i < (np*m*n+1); i=i+(size-1)) {
                MPI_Recv(&input, 8, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                output[0] = input[0];
                output[1] = input[1];
                output[2] = input[2];
                if (input[0]>0) {
                    output[3] = processTimeEffectOnCell(mod, input[3], input[4], input[5], input[6], input[7]);
                } else {
                    output[3] = generateFirstInstanceCell(m, n, input[2], input[1]);
                }
                MPI_Send(&output, 4, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
            }
	}
    }
    if (rank==0) {
	gettimeofday (&tpar, NULL); // Fin du chronometre
	timeEnd = (double) (tpar.tv_sec) + (double) (tpar.tv_usec) / 1e6;
	TParExec = timeEnd - timeStart; //Temps d'execution en secondes
        if (debug) {
            printResult(np%2, n, m, UPar);
        }
	float S = TSeqExec/TParExec;
	float E = S/size;
	printf("T1\t%.6lf sec\nTP\t%.6lf sec\nS\t%.2f\nE\t%.2f %%\n\n", (double)TSeqExec, (double)TParExec, S,E*100);
	FILE *json = fopen("stats.json","w");
	if(json == NULL) {
	    printf("stats file not created!");
	} else {
	    fprintf(json, "{\"input\":{\"nbproc\":%d,\"m\":%d,\"n\":%d,\"np\":%d,\"td\":%.4f,\"h\":%.4f},", size, m, n, np, td, h);
            fprintf(json, "\"output\":{\"T1\":%.2lf,\"TP\":%.2lf,\"S\":%.2f,\"E\":%.2f}}", TSeqExec*1000, TParExec*1000, S, E);
	    fclose(json);
	}

    }
    /*-----------------------------------------------------------------------------
     *  End Parallel code
     *-----------------------------------------------------------------------------*/
    MPI_Finalize();
    return 0;
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  generateFirstInstanceCell
 *  Description:  Will generate first instance of matrix cell since time 0 can't be
 *	found by dependencies.
 * =====================================================================================
 */
float generateFirstInstanceCell (int m, int n, int i, int j) { 
    float value = (float)(i * (m - i - 1) * j * (n - j -1));
    return value;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sumImmediateNeighbours
 *  Description:  Will sum neighbours of cell {top, right, bottom, left}, from last time
 *	instance.
 * =====================================================================================
 */
float sumImmediateNeighbours(float top, float right, float bottom, float left) {
    return top + right + bottom + left;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  processTimeEffectOnCell
 *  Description:  Calculation relative to the time effect on heat propagation
 * =====================================================================================
 */
float processTimeEffectOnCell (float mod, float old, float top, float right, float bottom, float left) {
    usleep(TEMPS_ATTENTES);
    return (1-(4*mod))*old+mod*(sumImmediateNeighbours(top, right, bottom, left));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getMod
 *  Description:  Return the mod, obtain by operation on decritisized time and cell side
 * =====================================================================================
 */
float getMod (float td, float h) {
    return (td/(h*h));
}
