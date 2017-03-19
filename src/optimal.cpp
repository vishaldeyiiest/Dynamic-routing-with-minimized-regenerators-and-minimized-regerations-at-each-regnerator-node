#include <bits/stdc++.h>
#define v 14
#define s 4
#define p 10
#define r 1000
using namespace std;
int adj[v+1][v+1];
int X[v+1][v+1][s] = {0};
int del[v+1] = {0};
int W[p][s];
int w[v][v][p];
char *datafile = "../data/nsfnet";
char *pathfile = "../paths/nsfnet/5.txt";
char *testfile = "../testing/nsfnet100/1.txt";	

void fillwithzero()
{
	for(int i = 1; i <= v; i++)
		for(int j = 1; j <= v; j++)
			for(int k = 0; k < s; k++)
				X[i][j][k] = 0;
}

void fillX(int S, int D)
{
	fillwithzero();
	ifstream fp(pathfile);
	vector<vector<int> > a;
	vector<int> path;
	string line;
	while(getline(fp,line))
	{
		istringstream is(line);
		vector<int> path;
		int n;
		while(is >> n)
			path.push_back(n);
		if(path.front() == S && path.back() == D)
			a.push_back(path);
	}
	for(unsigned int i = 0; i < a.size(); i++)
	{
		for(unsigned int j = 0; j < a[i].size() - 1; j++)
			X[a[i][j]][a[i][j+1]][i] = 1;
	}
}

int calc_reg(int S, int D)
{
	int count = 0, min = INT_MAX;
	for(int k = 0; k < s; k++)
	{
		for(int i = 1; i <= v; i++)
			for(int j = 1; j <= v; j++)
				count += X[i][j][k] * del[i];
		if(count < min)
			min = count;
	}
	return min;			
}

bool satisfy_flowbal(int S, int D)
{
	int sum1, sum2, sum3, sum4, sum5, sum6, sum7, sum8;
	sum1 = sum2 = sum3 = sum4 = sum5 = sum6 = sum7 =sum8 = 0;
	for(int j = 1; j <= v; j++)
			sum1 += X[S][j][0];
	for(int k = 0; k < s; k++)	
		for(int j = 1; j <= v; j++)
			sum2 += X[j][S][k];
	
	for(int j = 1; j <= v; j++)
		for(int k = 0; k < s; k++)
			sum3 += X[j][D][k];
	
	for(int j = 1; j <= v; j++)
		for(int k = 0; k < s; k++)
			sum4 += X[D][j][k];
	
	for(int i = 1; i <= v; i++)
	{
		if(del[i] == 0)
		{
			for(int j = 1; j <= v; j++)
			{
				for(int k = 0; k < s; k++)
				{
					sum5 += X[i][j][k];
					sum6 += X[j][i][k];
				}
			}
		}
		else
		{
			for(int j = 1; j <= v; j++)
			{
				for(int k = 0; k < s-1; k++)
					sum7 += X[i][j][k+1];
				for(int k = 0; k < s; k++)
					sum8 += X[j][i][k];
				
			}
		}
	}
	int dist = 0;
	for(int k = 0; k < s; k++)
		for(int i = 1; i <= v; i++)
			for(int j = 1; j <= v; j++)
				if(adj[i][j] > 0)
				dist += X[i][j][k] * adj[i][j];
	//cout << sum1 << sum2 <<sum3<<sum4<<sum5<<sum6<<sum7<<sum8<<" "<<dist<<endl;
	if(sum1 == 1 || sum2 == 0)
		return true;
	return false;
}

int main()
{
	for(int i = 1; i <= v; i++)
		for(int j = 1; j <= v; j++)
			adj[i][j] = INT_MIN;
			
	ifstream fp(datafile);
	string line;
	getline(fp, line);
	while(getline(fp, line))
	{
		istringstream is(line);
		int a, b, c;
		is >> a >> b >> c;
		adj[a][b] = adj[b][a] = c;
	}
	fp.close();
	del[4] = del[7] = del[10] = del[12] = 1;
	int S, D;
	ifstream fd(testfile);
	int sum = 0;
	while(getline(fd, line))
	{
		istringstream is(line);
		//int S, D;
		is >> S >> D;
		//S = 1, D = 14;
		fillX(S, D);
		if(satisfy_flowbal(S, D)){
		int c = calc_reg(S, D);
		sum += c;
		cout << c << endl;}
	}
	fd.close();
	cout <<"Total : "<<sum <<endl;
}
