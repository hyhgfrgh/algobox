#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve(){
    string a,b;cin>>a>>b;
    int na = a.size(),nb = b.size();
    a = " "+a,b = " "+b;
    vector<int> ne(nb+1);
    for(int i = 2,j = 0;i<=nb;i++){
        while(j>0 and b[j+1] != b[i]) j = ne[j];
        if(b[i] == b[j+1]) j++;
        ne[i] = j;
    }
    for(int i = 1,j = 0;i<=na;i++){
        while(j>0 and a[i] != b[j+1]) j = ne[j];
        if(a[i] == b[j+1]) j++;
        if(j == nb){
            cout<<i-j+1<<"\n";
        }
    }
    for(int i = 1;i<=nb;i++){
        cout<<ne[i]<<" ";
    }
    cout<<"\n";
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
