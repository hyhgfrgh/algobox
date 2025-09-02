#include<bits/stdc++.h>
using namespace std;

#define int long long

struct DSU{
    vector<int> fa;
    DSU(int n){
        fa.resize(n+10);
        iota(fa.begin(),fa.end(),0);
    }
    int find(int u){
        while(u != fa[u]) u = fa[u] = fa[fa[u]];
        return u;
    }
    void merge(int u,int v){
        u = find(u),v = find(v);
        if(u<v) fa[u] = v;
        else fa[v] = u;
    }
};


void solve(){
    int n,m;cin>>n>>m;
    DSU dsu(n+1);
    vector<int> a(n+1);
    while(m--){
        int ll,rr,x;cin>>ll>>rr>>x;
        for(int i = ll;i<=rr;i=dsu.find(i)+1){
            if(a[i] and a[i-1]) dsu.merge(i,i-1);
            if(a[i] and a[i+1]) dsu.merge(i,i+1);
            if(i!=x and !a[i]) a[i] = x;
        }

    }
    for(int i = 1;i<=n;i++){
        cout<<a[i]<<" ";
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
