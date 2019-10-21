#include<iostream>
#include<vector>
#include<fstream>
#include<set>
#include <tuple>

using namespace std;

#define INF 1000000000

long long n,m,t;
vector<tuple<long long,long long,long long>> a[300];
long long p[120][120], cost[300];
long long u,v,c,type;

int main(){
	ifstream in("p3.in");
	ofstream out("p3.out");
	in >> n >> m >> t;
	for(long long i = 0; i < m; i++){
		in >> u >> v >> c >> type;
		a[u].push_back(make_tuple(v,c,type));
		a[v].push_back(make_tuple(u,c,type));		
	}
	for(long long i = 1; i <= t; i++)
		for(long long j = 1; j<= t; j++)
			in >> p[i][j];
	for(long long i = 0 ; i <= n; i++) cost[i] = INF;
	set<tuple<long long,long long,long long>> heap;
	cost[n] = 0;
	heap.insert(make_tuple(cost[n], n, -1));
	while(heap.size()){
		long long costFather, father, typeFather;
		tie(costFather, father, typeFather) = *(heap.begin());
		heap.erase(heap.begin());
		for(long long i = 0; i < a[father].size(); i++){
			long long son, costEdge, typeSon;
			tie(son, costEdge, typeSon) = a[father][i];
			if(cost[son] > costFather + costEdge + (typeFather != -1 ? p[typeFather][typeSon] : 0)){
				cost[son] = costFather + costEdge + (typeFather != -1 ? p[typeFather][typeSon] : 0);
				heap.insert(make_tuple(cost[son], son, typeSon));
			}
		}
	}
	out << (cost[1] == INF ? -1 : cost[1]);
}