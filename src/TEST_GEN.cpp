#include <bits/stdc++.h>
#define MAX 28
using namespace std;

int main(int argc, char *argv[])
{
	FILE *fp;
	char aa[100] = "../testing/eon";
	strcat(aa, argv[1]);
	fp = fopen(aa, "w");
	int num = atoi(argv[1]);
	while(num)
	{
		int a = 1 + (rand() % MAX);
		int b = 1 + (rand() % MAX);
		if(a != b)
			fprintf(fp, "%d %d\n", a, b), num--;
	}

	fclose(fp);
	
}
	
