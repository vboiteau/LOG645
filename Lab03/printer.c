#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void printResult (int np, int columns, int rows, float matrix[np][columns][rows]){
    int i,j,k;
    for (k = 0; k <= np; ++k) {
    printf("La plaque du pas %d va etre \n", k);
       for (j = 0; j < columns; ++j) {
           for (i = 0; i < rows; ++i) {
		printf("%.6f\t",matrix[k][j][i]);
	   }
	   printf("\n");
       }
       printf("\n");
    }
};