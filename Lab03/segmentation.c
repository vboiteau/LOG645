#include <stdio.h>
#include "segmentation.h"
int agglomeration(int m, int n,int buf[m][n],float vecteur[m*n])
{
	   	for(int i = 0;i < m;i++)
		{
			for(int j=0;j<n;j++)
			{
				vecteur[(m*i)+j]=buf[i][j];
				printf("%d\t",vecteur[(m*i)+j]);
			}
		}
return 0;
}