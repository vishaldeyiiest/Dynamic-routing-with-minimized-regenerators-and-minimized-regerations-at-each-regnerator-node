/*Implementation of the rrp problem with 
	minimum number of regenerations at each node */
	
#include <bits/stdc++.h>
#include <vector>
#define MAX 14
#define r 2000							//optical reach
using namespace std;
int reg[] = {4, 7, 10, 12};
int n[MAX];
int ct = 0;

vector<int> colors;
struct state
{
	int x;
	vector<vector<int> > paths;
	vector<vector<int> > pig;
};
typedef struct state state;

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
	ifstream fp("../data/nsfnet");
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
		ifstream fp("../paths/nsfnet/5.txt");
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
		ifstream fp("../paths/nsfnet/5.txt");
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

unsigned int calc_reg(vector<vector<int> > path)
{
	int a[14];
	memcpy(a, n, sizeof(n));
	unsigned int s = 0;
	for(unsigned int i = 0; i < path.size(); i++)
	{
		a[path[i].back() - 1]++;
		s += a[path[i].back() - 1];
	}
	return s;		
}

pair<state,int> removeBest_minStates(vector<state> S)
{
	unsigned int min = 0, maxel;
	int a[S.size()][MAX] = {0};
	
	for(unsigned int i = 0; i < S.size(); i++)
	{
		for(int j = 0; j < MAX; j++)
			a[i][j] = n[j];
	}
	for(unsigned int i = 0; i < S.size(); i++)
	{
		vector<vector<int> > p = S[i].paths;
		for(unsigned int j = 0; j < p.size(); j++)
			a[i][p[j].back()-1]++;
	}

	int row_sum = INT_MAX;
	for(unsigned int i = 0; i < S.size(); i++)
	{
		int sum = 0, maxel = 0;
		for(int j = 0; j < MAX; j++)
			maxel = max(a[i][j], maxel);
		for(int j = 0; j < MAX; j++)
			 sum += maxel-a[i][j];
		if(sum < row_sum)
			row_sum = sum, min = i;
	}
	state s = S[min];
	//S.erase(S.begin() + min);
	return make_pair(s, min);
}

pair<state,int> removeBest(vector<state> S)
{
	vector<int> min_states;
	int min = 0;
	
	for(unsigned int i = 0; i < S.size(); i++)
	{
		for(unsigned int j = 0; j < S[i].paths.size(); j++)
		{
			if(calc_reg(S[i].paths) <= calc_reg(S[min].paths))
				min = i;
		}
		min_states.push_back(min);
	}
	min = removeBest_minStates(S, min_states)
	state s = S[min];
	//S.erase(S.begin() + min);
	return make_pair(s, min);
}

void display(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size(); i++)
	{
		for(unsigned int j = 0; j < p[i].size(); j++)
			cout<< p[i][j] <<" ";
		cout<<endl;
	}
}
 
void update_count(vector<vector<int> > p)
{
	for(unsigned int i = 0; i < p.size()-1; i++)
	{
		//n[p[i].front()-1]++;
		n[p[i].back()-1]++;
	
	}
	cout << "Reg" ;
	for(unsigned int i = 0; i < sizeof(n)/sizeof(n[0]); i++)
		cout<< n[i]<<" ";
}

vector<state> createNewStates(vector<vector<int> > c, vector<vector<int> > p, vector<vector<int> > pig);
void routing(int s, int d)
{
	vector<state> states;
	vector<vector<int> > c;
	state S;
	S.x = s;
	states.push_back(S);
	while(!states.empty())
	{
		pair<state,int> ob = removeBest(states);
		state obj = ob.first;
		if(!obj.paths.empty())
		{
			for(unsigned int i = 0; i < obj.paths.size()-1; i++)
				n[obj.paths[i].back() - 1]++;
		}
		states.erase(states.begin() + ob.second);
		//cout<<obj.x<<endl;
		if(obj.x == d)
		{
			display(obj.paths);
			ct += obj.paths.size() - 1;
			update_count(obj.paths);
			return;
		}
		else
		{
			if(dist(obj.x, d) <= r)
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
				c = paths_to_reg(obj.x);
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
		if(!pnew.empty()){
		for(unsigned int j = 0; j < pnew.size(); j++)
			W.push_back(assignListColors(j));
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
	}
	
	return st;
}
			
int main()
{
	for(int i = 1; i <= 15; i++)
		colors.push_back(i);
	for(unsigned int i = 0; i < sizeof(n)/sizeof(n[0]); i++)
		n[i] = 0;
	ifstream fp("../testing/nsfnet100/2.txt");
	string line;
	while(getline(fp, line))
	{
		istringstream is(line);
		int s, d;
		is >> s, is >> d;
		cout<<"Segments for "<< s<<" to "<<d<<endl;
		routing(s, d);
		cout<<ct<<endl;
	}
	fp.close();
	int sum = 0;
	for(unsigned int i = 0; i < sizeof(n)/sizeof(n[0]); i++)
		sum += n[i], cout<<"Reg: " << n[i]<<" ";
	cout<<"Total: "<<sum<<endl;
}

