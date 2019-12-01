#include<vector>
#include<fstream>
#include<set>
#include <tuple>
#include<algorithm>
#include<iostream>

using namespace std;

#define INF 9000000000

long long n,m,t;
vector<tuple<long long,long long,long long>> a[300];
long long p[120], cost[300], used[10];
long long u,v,c,type;
long long ans = 9000000000;

int main(){
	ifstream in("p4.in");
	ofstream out("p4.out");
	in >> n >> m >> t;
	for(long long i = 0; i < m; i++){
		in >> u >> v >> c >> type;
		a[u].push_back(make_tuple(v,c,type));
		a[v].push_back(make_tuple(u,c,type));		
	}
	for(long long i = 1; i <= t; i++) in >> p[i];
        for(long long x = 1; x < 1 << t; x++){
                long long use[10], x1 = x, costPenalty = 0;
                for(long long i = 1; i <= t; i++, x1/=2) {
                        use[i] = x1%2;
                        costPenalty += (use[i] ? p[i] : 0);
                }
                for(long long i = 0 ; i <= n; i++) cost[i] = INF;
                set<tuple<long long,long long>> heap;
                cost[1] = 0;
                heap.insert(make_tuple(cost[1], 1));
                while(heap.size()){
                        long long costFather, father;
                        tie(costFather, father) = *(heap.begin());
                        heap.erase(heap.begin());
                        for(long long i = 0; i < a[father].size(); i++){
                                long long son, costEdge, typeSon;
                                tie(son, costEdge, typeSon) = a[father][i];
                                if(use[typeSon] && cost[son] > costFather + costEdge){
                                        cost[son] = costFather + costEdge;
                                        heap.insert(make_tuple(cost[son], son));
                                }
                        }
                }
                ans = min(ans, cost[n] + costPenalty);
        }
        if(ans == 9000000000) out << "-1";
        else out << ans;
}
