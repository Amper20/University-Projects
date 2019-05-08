#include<vector>
#include<fstream>
#include<set>
#include <tuple>
#include<algorithm>
#include<iostream>

using namespace std;

#define INF 1000000000

int n,m,t;
vector<tuple<int,int,int>> a[300];
int p[120], cost[300], used[10];
int u,v,c,type;
int ans = 1000000000;

int main(){
	ifstream in("p4.in");
	ofstream out("p4.out");
	in >> n >> m >> t;
	for(int i = 0; i < m; i++){
		in >> u >> v >> c >> type;
		a[u].push_back(make_tuple(v,c,type));
		a[v].push_back(make_tuple(u,c,type));		
	}
	for(int i = 1; i <= t; i++) in >> p[i];
        for(int x = 1; x < 1 << t; x++){
                int use[10], x1 = x, costPenalty = 0;
                for(int i = 1; i <= t; i++, x1/=2) {
                        use[i] = x1%2;
                        costPenalty += (use[i] ? p[i] : 0);
                }
                for(int i = 0 ; i <= n; i++) cost[i] = INF;
                set<tuple<int,int>> heap;
                cost[1] = 0;
                heap.insert(make_tuple(cost[1], 1));
                while(heap.size()){
                        int costFather, father;
                        tie(costFather, father) = *(heap.begin());
                        heap.erase(heap.begin());
                        for(int i = 0; i < a[father].size(); i++){
                                int son, costEdge, typeSon;
                                tie(son, costEdge, typeSon) = a[father][i];
                                if(use[typeSon] && cost[son] > costFather + costEdge){
                                        cost[son] = costFather + costEdge;
                                        heap.insert(make_tuple(cost[son], son));
                                }
                        }
                }
                ans = min(ans, cost[n] + costPenalty);
        }
        if(ans == 1000000000) out << "-1";
        else out << ans;
}
