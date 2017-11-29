#include <queue>
#include <vector>
//#include <functional>
//#include <bits/stdc++.h>

#define mk make_pair
#define pb push_back
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int> > vvi;

vvi seq;
bool contains(int arr[], int l, int v)
{
    for(int i = 0; i < l; i++)
      if(arr[i] == v)
          return true;
    return false;

}

void combinationUtil(vi arr, int n, int r, int index, int data[], int i, int k)
{
    // Current cobination is ready, print it
    if (index == r)
    {
        vi t;
        for (int j=0; j<r; j++)
            t.pb(data[j]);
        if(find(seq.begin(), seq.end(), t) == seq.end())
          seq.pb(t);
        /*
        for(uint i = 0; i < t.size(); i++)
            cout << t[i] << " ";
        cout<<endl;*/
        return; 
    }
 
    // When no more elements are there to put in data[]
    if (i >= n)
        return;
 
    // current is included, put next at next location
    data[index] = arr[i];
    combinationUtil(arr, n, r, index+1, data, i+1, k);
 
    // current is excluded, replace it with next (Note that
    // i+1 is passed, but index is not changed)
    combinationUtil(arr, n, r, index, data, i+1, k);
}

void Combination(vi arr, int n, int r)
{
    // A temporary array to store all combination one by one
    int data[r];
    //vvi seq;
    // Print all combination using temprary array 'data[]'
    combinationUtil(arr, n, r, 0, data, 0, 0);
    //return seq;
}
 
/* arr[]  ---> Input Array
   n      ---> Size of input array
   r      ---> Size of a combination to be printed
   index  ---> Current index in data[]
   data[] ---> Temporary array to store current combination
   i      ---> index of current element in arr[]     */

/*
int main()
{
  vi a; a.pb(22); a.pb(10); a.pb(33); a.pb(12);
  sort(a.begin(), a.end());
  Combination(a, 4, 2);
  sort(seq.begin(), seq.end(), compare);
  for(uint i = 0; i < seq.size(); i++)
      {
        for(uint j = 0; j < seq[i].size(); j++)
          cout << seq[i][j] << " ";
        cout << endl;
      }
  
}*/