#include <stdio.h>

float input(float a[][10],int n)
{
	int i, j;
	for(i = 1; i <= n; i++)
	{
		for (j = 0; j <= n+1; j++)
		scanf("%f",&a[i][j]);
		return a[i][j];
	}

}
void output(float a[][10], int n)
{
	int i, j;
	for( i = 1, i <= n; i++)
	{
	    for( j = 0; j <= n+1; j++) 
	{	
	       printf("%4f",a[i][j]);
	}
        printf("\n"); 
	}
}
float change(float a[][10], int n)
{
	int i, j, k;
	for ( i=0; i <= n-1; i++)
	{
		if (a[i][i] == 0)
		{
			for( j = i+1; j <= n; j++)
			{
				if( a[j][i] != 0)
				a[i][i]=a[j][i];
				
			
			}
		}
	}
	
}

