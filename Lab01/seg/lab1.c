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
	int c = atoi(argv[1]);
	int p = atoi(argv[2]);
	int n = atoi(argv[3]);
	int m0[8][8];
	int m1[8][8];
	//Initialisation de la premiere matrice
	int i,j;
	for( i=0; i < 8; i++)
	{
		for(j=0;j < 8; j++)
		{
			m0[i][j] = p;
		}
	}	
	if(c == 1)
	{
		//Calcule de la deuxieme matrice
		for(i=0;i < 8; i++)
		{
			for(j=0;j<8;j++)
			{
				usleep(1000);	
				m1[i][j] = (m0[i][j] + (i+j) * n);
				printf("%i\t", m1[i][j]);	
			}
			printf("\n");
		}	
	}
	if(c == 2)
	{
		//Calcule de la deuxieme matrice
		for(i=0;i < 8; i++)
		{
			for(j=0;j<8;j++)
			{
				usleep(1000);
				if(j==0)
				{
					m1[i][j] = m0[i][j] + (i*n);
				}
				else
				{
					m1[i][j] = m0[i][j] + (m1[i][j-1] * n);
				}
				printf("%i\t", m1[i][j]);
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
