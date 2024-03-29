#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <omp.h>
#include "sys/time.h"

int columns = 10;
int rows = 10;

void printXY(int m[rows][columns]){
    int i,j;
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < columns; j++) {
            printf("%d\t",m[i][j]);
        }
	printf("\n");
    }
    printf("\n");
}

int main(int args,char *argv[])
{
    double TSeqExec, timeStart, timeEnd, Texec;
    struct timeval tp;
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
    int m0[n+1][rows][columns];
    int m1[n+1][rows][columns];
    m0[0][0][0]=p;
    m1[0][0][0]=p;
    //Initialisation de la premiere matrice
    int i,j,k;
    if(c == 1)
    {
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	for (k = 0; k < n+1; k++) {
	    /*printf("altération %d\n",k);*/
	    //Calcule de la deuxieme matrice
	    for(i=0;i < rows; i++)
	    {
		for(j=0;j<columns;j++)
		{
		    usleep(50000);
		    if(k>0){
			m0[k][i][j] = p + (i + j)*k;
		    } else {
			m0[k][i][j]=p;
		    }
		}
	    }
	}
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
#pragma omp parallel
	{
#pragma omp for collapse(3) 
	    for (k = 0; k < n+1; k++) {
		/*printf("altération %d\n",k);*/
		//Calcule de la deuxieme matrice
		for(i=0;i < rows; i++)
		{
		    for(j=0;j<columns;j++)
		    {
			usleep(50000);
			if(k>0){
			    m1[k][i][j] = p + (i + j)*k;
			} else {
			    m1[k][i][j]=p;
			}
		    }
		}
	    }
	}
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	printf("%s\n","first of seq");
	printXY(m0[0]);
	printf("%s\n","last of seq");
	printXY(m0[n]);
	printf("%s\n","last of par");
	printXY(m1[n]);
    }
    if(c == 2)
    {
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	for(k=0;k<=n;k++){
	    //Calcule de la deuxieme matrice
	    for(i=0;i < rows; i++)
	    {
		for(j=columns-1;j>=0;j--)
		{
		    usleep(50000);
		    if(k>0){
			if(j==9)
			{
			    m0[k][i][j] = m0[k-1][i][j] +i;
			}
			else
			{
			    m0[k][i][j] = m0[k-1][i][j] + m0[k][i][j+1];
			}
		    } else {
			m0[k][i][j]=p;
		    }
		}
	    }
	}
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	TSeqExec = timeEnd - timeStart; //Temps d'execution en secondes
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	for (k = 0; k <= n; k++) {
	    for (i=0;i<rows;i++) {
		#pragma omp parallel for
		for (j=(columns -1-i);j>=(0-i);j--) {
		    usleep(50000);
	            if (k>0) {
	                if (j==9) {
	                    m1[k][i][j+i]=m1[k-1][i][j+i]+i;
	                } else {
			    m1[k][i][j+i]=m1[k-1][i][j+i]+m1[k][i][j+1+i];
			}
	            } else {
			m1[k][i][j+i]=p;
		    }
	        }
	    }
	}
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	printf("%s\n","first of seq");
	printXY(m0[0]);
	printf("%s\n","last of seq");
	printXY(m0[n]);
	printf("%s\n","last of par");
	printXY(m1[n]);
    }	
    float S = TSeqExec/Texec;
    printf("seq: %f secs, par: %f secs, S:%f, E:%f\n", TSeqExec, Texec,S,(S/64));
    return 0;
}
