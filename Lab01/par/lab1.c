#include <stdlib.h>
#include <stdio.h>
 
int main(int args,char *argv[])
{
	if(args != 4)
	{
		printf("Il faut absolument avoir 3 arguments\n");
	 	return 1;
	}
	int c = atoi(argv[1]);
	int p = atoi(argv[2]);
	int n = atoi(argv[3]);

	if(c == 1)
	{
		
	}	
	printf("Test: %i, %i, %i\n",c,p,n);
	return 0;
}
