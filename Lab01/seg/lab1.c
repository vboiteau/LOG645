#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include "sys/time.h"

int main(int args,char *argv[])
{
	double timeStart, timeEnd, Texec;
	struct timeval tp;
	gettimeofday (&tp, NULL); // Debut du chronometre
	timeStart = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	if(args != 4)
	{
		printf("Il faut absolument avoir 3 arguments\n");
	 	return 1;
	}
	int columns = 8;
	int rows = 8;
	int c = atoi(argv[1]);
	int p = atoi(argv[2]);
	int n = atoi(argv[3]);
	int m1[n+1][8][8];
	//Initialisation de la premiere matrice
	int i,j,k;
	if(c == 1)
	{
	    for (k = 0; k < n+1; k++) {
	        printf("altération %d\n",k);
		//Calcule de la deuxieme matrice
		for(i=0;i < rows; i++)
		{
			for(j=0;j<columns;j++)
			{
				usleep(1000);
				if(k>0){
				    m1[k][i][j] = (m1[k-1][i][j] + (i+j) * k);
				} else {
				    m1[k][i][j]=p;
				}
				printf("%i\t", m1[k][i][j]);	
			}
			printf("\n");
		}
		printf("\n");
	    }
	}
	if(c == 2)
	{
	    for(k=0;k<=n;k++){
		printf("alteration %d\n",k);
		//Calcule de la deuxieme matrice
		for(i=0;i < 8; i++)
		{
			for(j=0;j<8;j++)
			{
				usleep(1000);
				if(k>0){
				if(j==0)
				{
					m1[k][i][j] = m1[k-1][i][j] + (i*k);
				}
				else
				{
					m1[k][i][j] = m1[k-1][i][j] + (m1[k][i][j-1] * k);
				}
				} else {
				    m1[k][i][j]=p;
				}
				printf("%i\t", m1[k][i][j]);
			}
			printf("\n");
		}
		printf("\n");
	    }
	}	
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	printf("%f secs\n", Texec);
	return 0;
}
