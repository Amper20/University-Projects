#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string.h>
using namespace std;

int main(){
        ofstream out("p1.out");
        ifstream in("p1.in");
        int n,x;
        vector<int> a[100500];
        in >> n;
        for(int i = 1; i <= n; i++){
                in >> x;
                a[x].push_back(i);
        }
        
        int zero_found = 0;
        vector<pair<int,int>> ans;
        for(int i = 1; i < 100100;i++){
                if(zero_found && a[i].size()){
                        zero_found = -1;
                        break;
                }
                if(!zero_found && a[i].size() == 0)
                        zero_found = 1;
                for(int j = 0 ; j < a[i].size(); j++)
                        ans.push_back(make_pair(a[i-1][0], a[i][j]));
                        
        }
        
        if(zero_found < 0)
                return out <<"-1",0;
        out << ans.size() << "\n";
        for(int i = 0; i < ans.size(); i++){
                out << ans[i].first << " " << ans[i].second << "\n";
        }
}