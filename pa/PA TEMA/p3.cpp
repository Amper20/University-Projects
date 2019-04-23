#include<fstream>
#include<vector>

using namespace std;

ifstream cin("p3.in");
ofstream cout("p3.out");

int main(){
        long long n, sum = 0;
        cin >> n;
        vector<long long> v(n,0);
        vector<vector<long long>> dp(n+5, vector<long long> (n+5, 0));
        
        for(int i = 0; i < n; i++){
                cin >> v[i];
                sum += v[i];
                dp[i][i] = v[i]; // initializam cazurile de baza
        }

        for( int size = 0; size < n; size++) // calculam dp[i,j] conform definitiei date in README
                for(int left = 0, right = size; right < n; left++, right++)
                        dp[left][right] = max(v[left]  + min((left + 2 <= right ? dp[left+2][right]   : 0), (left + 2 <= right ? dp[left+1][right-1] : 0)), 
                                              v[right] + min((left + 2 <= right ? dp[left+1][right-1] : 0), (left + 2 <= right ? dp[left][right-2]   : 0)));

        cout << 2*dp[0][n-1] - sum ;
        return 0;
}