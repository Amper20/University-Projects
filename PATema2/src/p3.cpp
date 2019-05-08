#include<iostream>
#include<vector>
#include<fstream>
#include<set>
#include <tuple>

using namespace std;

#define INF 1000000000

int n,m,t;
vector<tuple<int,int,int>> a[300];
int p[120][120], cost[300];
int u,v,c,type;

int main(){
	ifstream in("p3.in");
	ofstream out("p3.out");
	in >> n >> m >> t;
	for(int i = 0; i < m; i++){
		in >> u >> v >> c >> type;
		a[u].push_back(make_tuple(v,c,type));
		a[v].push_back(make_tuple(u,c,type));		
	}
	for(int i = 1; i <= t; i++)
		for(int j = 1; j<= t; j++)
			in >> p[i][j];
	for(int i = 0 ; i <= n; i++) cost[i] = INF;
	set<tuple<int,int,int>> heap;
	cost[n] = 0;
	heap.insert(make_tuple(cost[n], n, -1));
	while(heap.size()){
		int costFather, father, typeFather;
		tie(costFather, father, typeFather) = *(heap.begin());
		heap.erase(heap.begin());
		for(int i = 0; i < a[father].size(); i++){
			int son, costEdge, typeSon;
			tie(son, costEdge, typeSon) = a[father][i];
			if(cost[son] > costFather + costEdge + (typeFather != -1 ? p[typeFather][typeSon] : 0)){
				cost[son] = costFather + costEdge + (typeFather != -1 ? p[typeFather][typeSon] : 0);
				heap.insert(make_tuple(cost[son], son, typeSon));
			}
		}
	}
	out << (cost[1] == INF ? -1 : cost[1]);
}