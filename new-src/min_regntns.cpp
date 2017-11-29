/*Implementation of the rrp problem with 
	minimum number of regenerator nodes with minimum regenerations at each node*/

#include <bits/stdc++.h>
#include "helper.cpp"
#include <vector>
#define r 1500	//optical reach
#define ncolor 1000		
#define N 100	
#define MAXSEG 4	
#define REG_MAX 40			
using namespace std;
char testfile[100] = "../testing/";
//NJLATA
#if 0
#define MAX 11
//int reg[] = {5, 6, 7, 10}; int reg[] = {5, 6, 7, 9}; ***int reg[] = {5, 6, 9, 10};
int reg[] = {5, 6, 9, 10};
char file[100] = "../paths/njlata/5.txt";
char datafile[100] = "../data/njlata";
#endif

#if 1
#define MAX 20
int reg[] = {3, 6, 8, 9, 11, 14, 17};
char file[100] = "../paths/arpanet/5.txt";
char datafile[100] = "../data/arpanet";
#endif

// NSFNET 
#if 0
#define MAX 14
int reg[] = {3, 4, 8, 12};
char file[100] = "../paths/nsfnet/5.txt";
char datafile[100] = "../data/nsfnet";
#endif

#if 0
#define MAX 46
int reg[] = {7, 9, 12, 15, 18, 21, 25, 30, 34, 38, 42};
char file[100] = "../paths/usanet/5.txt";
char datafile[100] = "../data/usanet";
#endif

int n[MAX] = {0};
int ct = 0;
int blocked = 0;
int W[MAX*(MAX-1)];

bool compare(vi a, vi b)
{
    return (n[*max_element(a.begin(), a.end())] < n[*max_element(b.begin(), b.end())]);
}

vector<int> colors;
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

int dist(int x, int y)
{
	if(x == y)
		return 0;
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


void display(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size(); i++)
	{
		for(unsigned int j = 0; j < p[i].size(); j++)
			cout<< p[i][j] <<" ";
		cout << "Cost: " << cost(p[i]) << endl;
	}
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
		}
	}
	return true;
}

pair<state,int> removeBest(vector<state> st, int d)
{
	unsigned int min = 0, maxel;
	int mincost = INT_MAX;
	vector<state> S;
	for(unsigned int i = 0; i < st.size(); i++)
	{
		int c = ceil(dist(st[i].x, d)/(double)r);
		unsigned int s = st[i].paths.size() -1 + c;
		if(s <= mincost)
			mincost = s;
	}
	//cout<<mincost<<endl;
	//adding states to S with min cost i.e min regenerations
	for(unsigned int i = 0; i < st.size(); i++)
	{
		int c = ceil(dist(st[i].x, d)/(double)r);
		//printf("DIST: %d %d %d\n", st[i].x, d, dist(st[i].x, d));
		unsigned int s = st[i].paths.size() -1 + c;
		if(s == mincost)
			S.push_back(st[i]);// display(st[i].paths);
	}

	//S.erase(S.begin() + min);
	//return make_pair(s, min);
	//cout << S.size() << endl;
	min = 0;
	vector<pair<int, int> > count;
	int flag = 0;

	for(int i = 0; i < MAX; i++)
		if(n[i] != 0){
			flag = 1;
			break;
		}
	int len = sizeof(reg)/sizeof(reg[0]);
	for(int i = 0; i < MAX; i++)
		if(n[i] != 0 || flag && contains(reg, len, i+1))
			count.pb(mk(n[i], i));

	if(flag == 0)
		for(int i = 0; i < len; i++)
			count.pb(mk(reg[i]-1, 0));
	//vi indices = find_k_min(count, count.size());
	sort(count.begin(), count.end());
/*
	for(int i = 0; i < count.size(); i++)
		cout << count[i].second << " ";
	cout << endl;

	int sim = INT_MAX;
	for(uint i = 0; i < S.size(); i++)
	{
		int temp = 0;
		for(int k = 0; k < MAX; k++)
			temp += abs(n[k] - a[i][k]);
		if(temp < sim)
			sim = temp, min = i;
	}*/
	
	int temp = 0;
	vi nodes_count;
	for(uint j = 0; j < count.size(); j++)
			nodes_count.pb(count[j].second);

	seq.clear();
	for(uint ss = 1; ss < MAXSEG; ss++)	
		Combination(nodes_count, count.size(), ss);
	//display(seq);
	sort(seq.begin(), seq.end(), compare);

	for(unsigned int i = 0; i < S.size(); i++)
	{
		vi nodes;
		temp = S[i].paths.size();
		for(uint j = 0; j < temp; j++)
			nodes.pb(S[i].paths[j].back()-1);
		
		for(uint j = 0; j < seq.size(); j++)
		{
			if(nodes == seq[j]){
				min = i; break;
			}
			
		}
	}
	
	state s = S[min];
	//S.erase(S.begin() + maxelin);
	return make_pair(s, min);
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
		if(ob.first.paths.size() > MAXSEG  && ob.first.x == d)
		{
			blocked ++;
			cout<<"Request: "<< s <<" to " << d << "blocked" << endl;
			return;
		}
		state obj = ob.first;
		//cout<<obj.x<<endl;
		states.erase(states.begin() + ob.second);
		//cout<<obj.x<<endl;
		if(obj.x == d)
		{
			//cout<<"Segments for "<< s<<" to "<<d<<endl;
			//display(obj.paths);
			if(!count_wav(obj.paths)){	blocked++; return; }
			ct += obj.paths.size() - 1;
			if(!update_count(obj.paths))	{ blocked++; return; } 
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

vector<int> assignListColors(int v)
{
	vector<int> x;
	for(unsigned int i = 0; i < colors.size(); i++)
		x.push_back(colors[i]);
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
		for(unsigned int j = 0; j < p.size(); j++)
			pnew.push_back(p[j]);
		pnew.push_back(c[i]);
		vector<vector<int> > pignew;
		pignew = make_pig(pnew);		//augment graph by path pnew
		vector<vector<int> > W;
		if(pnew.empty())
			break;
		
		for(unsigned int j = 0; j < pnew.size(); j++)
			W.push_back(colors);	
		if(listColors(pignew, W))
		{
			state t;
			t.x = c[i].back();
			//cout<<t.x<<endl;
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
	for(int i = 1; i <= 15; i++)
		colors.push_back(i);
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
	cout<<"TOTAL : " <<sum<<endl;
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

