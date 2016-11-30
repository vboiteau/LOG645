/*
 * =====================================================================================
 *
 *       Filename:  printer.c
 *
 *    Description:  Matrix printer functions
 *
 *        Version:  1.0
 *        Created:  11/06/2016 18:01:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vincent Boiteau-Robert (vboiteau), vboiteau94@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

/* #####   HEADER FILE INCLUDES   ################################################### */
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "printer.hpp"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printResult
 *  Description:  Will print matrix
 * =====================================================================================
 */
void printResult (int m, int n, float *matrix, int headStart){
    int i,j;
    for (j = 0; j < n; ++j) {
        for (i = 0; i < m; ++i) {
            printf("%.4f",*((matrix+j*m)+i+headStart));
            if (i<(m-1)) {
                printf((*((matrix+j*m)+i)<100?"\t\t":"\t"));
            }
        }
        printf("\n");
    }
    printf("\n");
};
