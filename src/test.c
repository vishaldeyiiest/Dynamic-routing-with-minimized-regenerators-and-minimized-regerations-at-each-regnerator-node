#include <stdio.h>
int main()
{
	int i, j, a[10][10];
	for(i = 0; i < 10; i++)
		for(j = 0; j < 10; j++)
			a[i][j] = i*10+j;
	printf("%d", *a[i+9]);
}
