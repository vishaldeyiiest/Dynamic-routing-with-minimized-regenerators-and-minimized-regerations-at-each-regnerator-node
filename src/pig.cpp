#include <bits/stdc++.h>
using namespace std;

int edge_value(vector<int> a, vector<int> b)
{
	int v = 0;
	for(unsigned int i = 0; i < a.size()-1; i++)
	{
		for(unsigned int j = 0; j < b.size()-1; j++)
		{
			if((b[j] == a[i] && b[j+1] == a[i+1]))
				v++;
		}
	}
	//cout<<gain<<endl;
	return v;
}
	
void make_pig()
{
	
	//ofstream fo("../pig.txt");
	string line;
	ifstream fp("../paths/nsfnet/gstream/5.txt");			//optimal pig
	//ifstream fp("../bpi.txt");
	vector<vector<int> > a;
	while(getline(fp,line))
	{
		istringstream is(line);
		vector<int> path;
		int n;
		while(is >> n)
			path.push_back(n);

		a.push_back(path);
	}
	
	for(unsigned int i = 0; i < a.size(); i++)
	{
		for(unsigned int j = 0; j < a.size(); j++)
		{
			if(j != i)
				cout<<i<<" "<<j<<" "<<edge_value(a[i], a[j])<<endl;
		}
	}
	//fp.close();		
	//fo.close();
}

int main()
{
	clock_t t;
	t = clock();
	make_pig();
	t = clock() - t;
	cout<<"Time: "<<((double)t)/CLOCKS_PER_SEC<<endl;
}
