/*
 * =====================================================================================
 *
 *       Filename:  segmentation.c
 *
 *    Description:  Segmentation evaluation
 *
 *        Version:  1.0
 *        Created:  11/09/2016 14:33:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vincent Boiteau-Robert (vboiteau), vboiteau94@gmail.com
 *                  Julien Lemonde
 *   Organization:  ETS
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <stdio.h>
#include "segmentation.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  agglomeration
 *  Description:  Task to agglomerate matrix
 * =====================================================================================
 */
int agglomeration(int m, int n,int buf[m][n],float vecteur[m*n]) {
    for(int i = 0;i < m;i++)
    {
        for(int j=0;j<n;j++)
        {
            vecteur[(m*i)+j]=buf[i][j];
            printf("%.4f\t",vecteur[(m*i)+j]);
        }
    }
    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  checkPartitionType
 *  Description:  Return the partition type detected by the matrix geometry.
 * =====================================================================================
 */
int checkPartitionType ( int size, int m, int n ) {
    int partitionType = 0;
    int poolAvailableForProcessing = size - 1;
    float nbOfLoopForCol = m / poolAvailableForProcessing;
    float nbOfLoopForRow = n / poolAvailableForProcessing;
    if (nbOfLoopForCol>=1||nbOfLoopForRow>=1) {
        if (nbOfLoopForCol<=nbOfLoopForRow) {
            partitionType = 1;
        } else {
            partitionType = 2;
        }
    }
    return partitionType;
}		/* -----  end of function checkPartitionType  ----- */
