#include<fstream>
#include<vector>
#include<utility>
#include<algorithm>
using namespace std;

ifstream cin("p2.in");
ofstream cout("p2.out");

int main(){
        int n, k, arr[1010];
        cin >> n >> k;
        vector<vector<int>> dp(n+5, vector<int> (n+5,0));
        
        if(n == k)
                return cout << "0",0;

        for(int i = 1; i <= n; i++)
                cin >> arr[i];

        sort(arr + 1, arr + 1 + n);   //sortam vectorul
        reverse(arr + 1, arr + 1 + n); // descrescator

        for(int i = 1; i <= n; i++)
                for(int j = 1; j <= min(n-k, i); j++)
                        dp[i][j] = max( dp[i-1][j], dp[i-1][j-1] + (!(j %2) ? -arr[i] : arr[i])); // calculam dp[i][j] conform definitiei date in README
        
        for(int i = 1; i <= n; i++)
                for(int j = 1; j <= n-k; j++)
                        cout << dp[i][j] << (j == n-k ? "\n" : " ");

        cout << dp[n][n-k];
        return 0;
}