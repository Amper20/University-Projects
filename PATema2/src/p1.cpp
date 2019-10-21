#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string.h>
using namespace std;

int main(){
        ofstream out("p1.out");
        ifstream in("p1.in");
        long long n,x;
        vector<long long> a[100500];
        in >> n;
        
        long long zero_found = 0;
        for(long long i = 1; i <= n; i++){
                in >> x;
                a[x].push_back(i);
                if(x <= 0 && i >= 2) 
                        zero_found = -1;
        }
        if(zero_found < 0)
                return out << "-1",0;
        vector<pair<long long,long long>> ans;
        for(long long i = 1; i < 100100;i++){
                if((zero_found && a[i].size()) || zero_found < 0){
                        zero_found = -1;
                        break;
                }
                if(!zero_found && a[i].size() == 0)
                        zero_found = 1;
                for(long long j = 0 ; j < a[i].size(); j++)
                        ans.push_back(make_pair(a[i-1][0], a[i][j]));
                        
        }
        
        if(zero_found < 0)
                return out <<"-1",0;
        out << ans.size() << "\n";
        for(long long i = 0; i < ans.size(); i++){
                out << ans[i].first << " " << ans[i].second << "\n";
        }
}