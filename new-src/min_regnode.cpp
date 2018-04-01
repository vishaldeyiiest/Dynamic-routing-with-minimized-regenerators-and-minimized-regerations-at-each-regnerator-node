/*Implementation of the rrp problem with 
	minimum stages of regenerations*/

#include <bits/stdc++.h>
#include "helper.cpp"
#define r 109	//optical reach		
#define ncolor 1000
#define N 100	
#define MAXSEG 3
#define REG_MAX 40			
using namespace std;
// ARPANET 
#if 1
#define MAX 21
int reg[] = {3, 4, 11, 9, 13, 20, 6, 16, 8};
//int reg[] = {4, 5, 10, 13, 14, 17};
char file[100] = "arpanet5.paths";
char datafile[100] = "../networks/arpanet.txt";
#endif

//EON
#if 0
#define MAX 28
//int reg[] = {5, 6, 7, 10}; int reg[] = {5, 6, 7, 9}; ***int reg[] = {5, 6, 9, 10};
int reg[] = {5, 9, 14, 16, 21, 23, 24, 25, 28};
char file[100] = "../paths/eon/5.txt";
char datafile[100] = "../data/eon";
#endif

// NSFNET 
#if 0
#define MAX 14
int reg[] = {4, 5, 6, 8, 9, 13};
char file[100] = "nsfnet5.paths";
char datafile[100] = "../networks/nsfnet.txt";
#endif

//NJLATA
#if 0
#define MAX 11
//int reg[] = {5, 6, 7, 10}; int reg[] = {5, 6, 7, 9}; ***int reg[] = {5, 6, 9, 10};
int reg[] = {4, 8, 9, 10};
char file[100] = "../paths/njlata/5.txt";
char datafile[100] = "../data/njlata";
#endif

#if 0
#define MAX 46
int reg[] = {3, 5, 9, 12, 15, 19, 22, 25, 30, 34, 40, 45};
char file[100] = "../paths/usanet/5.txt";
char datafile[100] = "../data/usanet";
#endif

char testfile[100] = "../testing/";
int n[MAX] = {0};
int ct = 0;
int blocked = 0;

bool compare(vi a, vi b)
{
    return (n[*max_element(a.begin(), a.end())] < n[*max_element(b.begin(), b.end())]);
}

vector<int> colors;
vector<vector<int> > colv;

int W[MAX*(MAX-1)];

struct state
{
	int x;
	vector<vector<int> > paths;
	vector<vector<int> > pig;
};
typedef struct state state;
vector<state> createNewStates(vector<vector<int> > c, vector<vector<int> > p, vector<vector<int> > pig);

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
	return v;
}

void display(vvi p);

double dist(int x, int y)
{
	ifstream fp(datafile);
	string line;
	int d = INT_MAX;
	while(getline(fp,line))
	{
		istringstream is(line);
		int a, b, c;
		is >> a, is >> b, is >> c;
		if(a == x && b == y)
		{
			d = c;
			break;
		}
	}
	fp.close();
	return d;
}

int cost(vector<int> p)
{
	int c = 0;
	for(unsigned int i = 0; i < p.size()-1; i++)
		if(dist(p[i], p[i+1]) != INT_MAX)
			c += dist(p[i], p[i+1]);
	return c;
}		

vector<vector<int> > make_pig(vector<vector<int> > path)
{
	string line;
	int a[path.size()][path.size()];
	
	vector<vector<int> > pig;
	for(unsigned int i = 0; i < path.size(); i++)
	{
		a[i][i] = 0;
		vector<int> temp;
		for(unsigned int j = 0; j < path.size(); j++)
		{
			if(j != i)
			{
				a[i][j] = edge_value(path[i], path[j]);
				//cout<<i<<" "<<j<<" "<<a[i][j]<<endl;
			}
			temp.push_back(a[i][j]);	
		}
		pig.push_back(temp);
	}
	return pig;
}

vector<vector<int> > make_path(int s, int x)
{
		ifstream fp(file);
		string line;
		vector<vector<int> > a;
		while(getline(fp,line))
		{
			istringstream is(line);
			vector<int> path;
			int n;
			while(is >> n)
				path.push_back(n);
			if(path.front() == s && path.back() == x && cost(path) <= r )
				a.push_back(path);
		}
		return a;
}

vector<vector<int> > paths_to_reg(int x)
{
		ifstream fp(file);
		string line;
		vector<vector<int> > c;
		while(getline(fp,line))
		{
			istringstream is(line);
			vector<int> path;
			int n;
			while(is >> n)
				path.push_back(n);
			if(path.front() == x && cost(path) <= r)
			{
				for(unsigned int i = 0; i < sizeof(reg); i++)
					if(path.back() == reg[i])
						c.push_back(path);
			}
		}
		return c;
}

pair<state,int> removeBest(vector<state> st, int d)
{
	unsigned int min = 0, maxel;
	int mincost = INT_MAX;
	for(unsigned int i = 0; i < st.size(); i++)
	{
		int c = ceil(dist(st[i].x, d)/(double)r);
		unsigned int s = st[i].paths.size() -1 + c;
		if(s <= mincost)
			mincost = s, min = i;
	}
	state s = st[min];
	st.erase(st.begin() + min);
	return make_pair(s, min);
}

void display(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size(); i++)
	{
		for(unsigned int j = 0; j < p[i].size(); j++)
			cout<< p[i][j] <<" ";
		cout << "Cost: " << cost(p[i]) << endl;
	}
}

bool count_wav(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size(); i++)
	{
		for(unsigned int j = 0; j < p[i].size(); j++)
		{
			if(W[p[i][j]*(MAX-1)+p[i][j]] == ncolor)
				return false;
		}
	}
	for(unsigned int i = 0; i < p.size(); i++)
	{
		for(unsigned int j = 0; j < p[i].size(); j++)
		{
			W[p[i][j]*(MAX-1)+p[i][j]] ++;
			//if(W[p[i][j]*(MAX-1)+p[i][j]] > ncolor)
			//	blocked++;
		}
	}
	return true;
}

bool update_count(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size()-1; i++)
	{
		//n[p[i].front()-1]++;
		if(n[p[i].back()-1] == REG_MAX)
			return false;
		n[p[i].back()-1]++;
	}
	return true;
}

void routing(int s, int d)
{
	vector<state> states;
	vector<vector<int> > c;
	state S;
	S.x = s;
	states.push_back(S);
	while(!states.empty())
	{
		pair<state,int> ob = removeBest(states, d);
		state obj = ob.first;
		//cout<<obj.x<<endl;
		states.erase(states.begin() + ob.second);
		//cout<<obj.x<<endl;
		if(obj.x == d)
		{
			//cout<<"Segments for "<< s<<" to "<<d<<endl;
			display(obj.paths);
			if(obj.paths.size() - 1 > MAXSEG) {blocked++; cout << "Blocked for S"<<endl; return;}
			if(!count_wav(obj.paths)){	blocked++; cout << "Blocked for W"<<endl; return; }
			ct += obj.paths.size() - 1;
			if(!update_count(obj.paths))	{ blocked++; cout << "Blocked for RE"<<endl; return; } 
			return;
		}
		else
		{
			if(dist(obj.x, d) > r)
			{
				c.clear();
				c = make_path(obj.x, d);
				vector<vector<int> > t = paths_to_reg(obj.x);
				for(unsigned int i = 0; i < t.size(); i++)
					c.push_back(t[i]);
			}
			else
			{
				c.clear();
				c = make_path(obj.x, d);
			}
			vector<state> t = createNewStates(c, obj.paths, obj.pig);
			for(unsigned int i = 0; i < t.size(); i++)
				states.push_back(t[i]);
		}
	}
}

vector<int> assignListColors(vector<int> p)
{
	vector<int> x;
	for(unsigned int i = 0; i < p.size(); i++)
	{
		//if()
		x.push_back(colors[i]);
	}
	return x;
}

bool pick_one(vector<vector<int> > c, int x, int y)
{
	for(unsigned int i = 0; i < c[x].size(); i++)
	{
		for(unsigned int j = 0; j < c[y].size(); j++)
		{
			if(c[x][i] == c[x][j])
			{
				if(find(colors.begin(), colors.end(), c[x][i]) != colors.end())
					remove(colors.begin(), colors.end(), c[x][i]);
				return true;
			}	
		}
	}
	return false;
}
	
bool listColors(vector<vector<int> >pig, vector<vector<int> > c)
{	
	int flag = 0;
	for(unsigned int i = 0; i < pig.size(); i++)
	{
		for(unsigned int j = 1; j < pig[i].size(); j++)
		{
			if(pig[i][j] != 0)
			{
				//colors.erase(colors.begin());
				if(pick_one(c, i, j))
					flag = 0;
				else
					flag = 1;
			}
		}
	}
	if(flag == 0)
		return true;
	
	return false;		
}

vector<state> createNewStates(vector<vector<int> > c, vector<vector<int> > p, vector<vector<int> > pig)
{
	vector<state> st;
	for(unsigned int i = 0; i < c.size(); i++)
	{
		vector<vector<int> > pnew;
		// pnew = P U c[i]
		for(unsigned int j = 0; j < p.size(); j++)
			pnew.push_back(p[j]);
		pnew.push_back(c[i]);
		vector<vector<int> > pignew;
		pignew = make_pig(pnew);		//augment graph by path pnew
		vector<vector<int> > W;
		if(pnew.empty())
			break;
		
		for(unsigned int j = 0; j < pnew.size(); j++)
			W.push_back(assignListColors(pnew[j]));	
		if(listColors(pnew, W))
		{
			state t;
			t.x = c[i].back();
			//cout<<"Assigned Wavelength" <<endl;
			t.paths = pnew;
			t.pig = pignew;
			st.push_back(t);
		}
	}
	return st;
}
			
int main(int argc, char *argv[])
{
	strcat(testfile, argv[1]);
	ct = 0;
	for(unsigned int i = 0; i < sizeof(n)/sizeof(n[0]); i++)
		n[i] = 0;
	for(int i = 1; i <= ncolor; i++)
		colors.push_back(i);
	memset(W, 0, sizeof(W));
	/*
	uint edges = sizeof(n)*(sizeof(n)-1);
	for(int i = 0; i < ncolor; i++)
		for(uint j = 0; j < edges; j++)
			colv[i][j] = 0;*/
	//display(paths_to_reg(1));
	ifstream fp(testfile);
	string line;
	clock_t start = clock();
	while(getline(fp, line))
	{
		istringstream is(line);
		int s, d;
		is >> s, is >> d;
		routing(s, d);
		//cout<<":= " << ct<<endl;
	}
	fp.close();
	int sum = 0;
	for(unsigned int i = 0; i < sizeof(n)/sizeof(n[0]); i++)
		sum += n[i], cout<<n[i]<<" ";
	cout<<endl;
	cout<<"TOTAL: " << sum<<endl;
	double mean = (double)sum/(sizeof(reg)/sizeof(reg[0])), sd = 0;
	for(unsigned int i = 0; i < sizeof(reg)/sizeof(reg[0]); i++)
		sd += pow((n[reg[i]-1]-mean), 2);
	cout << "MEAN: " << mean << endl;
	cout << "SD: " << sqrt(sd/(sizeof(reg)/sizeof(reg[0]))) << endl;
	cout << "Blocked requests: " << blocked<<endl;
	cout<<"Wavelengths: " << *max_element(W, W+MAX*(MAX-1))<<endl;
	cout<<"Time: "<<(clock() - start)/(double)CLOCKS_PER_SEC<<endl;
	/*for(int i = 0; i < MAX*(MAX-1); i++)
		cout << W[i] << " ";
	cout << endl;*/
}

