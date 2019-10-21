#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
using namespace std;
long long n, m, k, a[120][120], mini, ans, viz[120][120], ans_aux;
long long dx[4] = {0,  0,  1, -1},dy[4] = {1, -1,  0,  0};
void dfs(long long i, long long j){
        viz[i][j] = 1;
        if(a[i][j] < mini || a[i][j] > mini + k || a[i][j] < 0){
                ans_aux--;
                return;
        }
        for(long long k = 0 ; k < 4; k++)
                if (!viz[i + dx[k]][j + dy[k]]){
                        ans_aux++;
                        dfs(i + dx[k], j + dy[k]);
                }
}

int main(){
        ifstream in("p2.in");
        ofstream out("p2.out");
        in >> n >> m >> k; 
        for(long long i = 0; i <= n + 1; i++)
                for(long long j = 0; j <= m + 1; j++)
                        a[i][j] = -1;
        for(long long i = 1; i <= n; i++)
                for(long long j = 1; j <= m; j++)
                        in >> a[i][j];
        for(long long i = 1; i <= n; i++)
                for(long long j = 1; j <= m; j++){
                        for(long long q = 1;  q <= n; q++) for(long long b = 1; b <= m; b++) viz[q][b] = 0;
                        mini = a[i][j];
                        ans_aux = 0;
                        dfs(i,j);
                        ans = max(ans_aux, ans);
                }
        out << ans + 1;
}