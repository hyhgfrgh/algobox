#include<bits/stdc++.h>
using namespace std;

// #define int long long

// 注意512MB,1e8范围的数组能开int开不了long long


vector<int> prim,minp;

void sieve(int n){
    minp.resize(n+10);
    for(int i = 2;i<=n;i++){
        if(!minp[i]) prim.emplace_back(i);
        for(int p:prim){
            if(p*i<=n){
                minp[p*i] = true;
            }else break;
        }
    }
}


void solve(){
    int n,q;cin>>n>>q;
    sieve(n);
    while(q--){
        int k;cin>>k;
        cout<<prim[k-1]<<"\n";
    }
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
