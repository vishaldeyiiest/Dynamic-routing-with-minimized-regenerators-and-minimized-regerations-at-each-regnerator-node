#include <bits/stdc++.h>
using namespace std;

int main()
{
	ifstream fp("../data/eon2");
	ofstream fo("../data/eon");
	string line;
	getline(fp, line);
	//
	while(getline(fp, line))
	{
		istringstream is(line);
		int a, b, c;
		is >> a, is >> b, is >> c;
		c = rand()/(double)INT_MAX*1200;
		fo << a << " " << b << " " << c;
		fo << endl;
		fo << b << " " << a << " " << c;
		fo << endl;
		//getline(fp, line);
	}
	fp.close();
	fo.close();
	
}
	
