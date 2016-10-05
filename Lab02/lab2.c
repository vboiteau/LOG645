#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <omp.h>
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
	int columns = 10;
	int rows = 10;
	int c = atoi(argv[1]);
	int p = atoi(argv[2]);
	int n = atoi(argv[3]);
	int m1[n+1][rows][columns];
	//Initialisation de la premiere matrice
	int i,j,k,suite;
	if(c == 1)
	{
		suite = 0;
		for (k = 0; k < n+1; k++) {
			suite+=k;
			/*printf("altération %d\n",k);*/
			//Calcule de la deuxieme matrice
			#pragma omp parallel
			{
				#pragma omp  for collapse(2) 
				for(i=0;i < rows; i++)
				{
					for(j=0;j<columns;j++)
					{
						usleep(1000);
						if(k>0){
							m1[k][i][j] = p + (i + j)*k;
						} else {
							m1[k][i][j]=p;
						}
					}
				}
				#pragma omp barrier
				}
				for(i=0;i < rows; i++)
				{
					for(j=0;j< columns;j++)
					{
						printf("%d\t",m1[k][i][j]);
					}
					printf("\n");

			}
		}
	}
	if(c == 2)
	{
		for(k=0;k<=n;k++){
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
				}
			}
		}
	}	
	gettimeofday (&tp, NULL); // Fin du chronometre
	timeEnd = (double) (tp.tv_sec) + (double) (tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'execution en secondes
	printf("%f secs\n", Texec);
	return 0;
}
