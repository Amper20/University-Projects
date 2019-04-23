#include<fstream>
#include<set>

using namespace std;

ifstream cin("p1.in");
ofstream cout ("p1.out");

int main(){
        multiset<int> s;
        long long n, x, s1 = 0, u = 0, val;
        cin >> n;
        
        for(;n--;s.insert(x)) // generam multisetul
                cin >> x; 

        for(;!s.empty(); u = !u, s.erase(s.find(val))){ // extragem elementul maxim din multiset
                val = *(--s.end());
                s1 += (u ? -val : val); //updatam diferenta
        }

        cout << s1;
        return 0;
}