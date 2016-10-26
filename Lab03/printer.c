#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "printer.h"
static void printTime (int np, int columns, int rows, float matrix[columns][rows]);

void printResult (int np, int columns, int rows, float matrix[np][columns][rows]){
    printTime(0, columns, rows, matrix[0]);
    printTime(np, columns, rows, matrix[np]);
};

static void printTime (int np, int columns, int rows, float matrix[columns][rows]) {
    int i,j;
    printf("La plaque du pas %d va etre \n", np);
    for (j = 0; j < columns; ++j) {
	for (i = 0; i < rows; ++i) {
	    printf("%.3f\t",matrix[j][i]);
	}
	printf("\n");
    }
    printf("\n");
};