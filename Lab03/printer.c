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
#include "printer.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printResult
 *  Description:  Will print matrix
 * =====================================================================================
 */
void printResult (int np, int columns, int rows, float matrix[np][columns][rows]){
    int i,j;
    for (j = 0; j < columns; ++j) {
        for (i = 0; i < rows; ++i) {
            printf("%.3f",matrix[np][j][i]);
            if (i<(rows-1)) {
                printf(",");
            }
        }
        printf("\n");
    }
    printf("\n");
};