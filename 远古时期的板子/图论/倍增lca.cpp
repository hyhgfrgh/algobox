#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve(){
    int n,q,s;cin>>n>>q>>s;
    vector<vector<int>> adj(n+1);
    for(int i = 2;i<=n;i++){
        int u,v;cin>>u>>v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    int fa[500000+10][21];
    // vector 卡常...
    vector<int> dep(n+1);
    auto dfs = [&](auto self,int u,int ffa)->void{
        fa[u][0] = ffa;dep[u] = dep[ffa]+1;
        for(int v:adj[u]){
            if(v == ffa) continue;
            self(self,v,u);
        }
    };
    dfs(dfs,s,s);
    for(int k = 1;k<=20;k++){
        for(int i = 1;i<=n;i++){
            fa[i][k] = fa[fa[i][k-1]][k-1];
        }
    }
    auto lca = [&](int u,int v)->int{
        if(dep[u]<dep[v]) swap(u,v);
        for(int k = 20;k>=0;k--){
            if(dep[fa[u][k]]<dep[v]) continue;
            u = fa[u][k];
        }
        if(u ==  v) return u;
        for(int k = 20;k>=0;k--){
            if(fa[u][k] != fa[v][k]){
                u = fa[u][k],v = fa[v][k];
            }
        }
        return fa[u][0];
    };


    while(q--){
        int u,v;cin>>u>>v;
        cout<<lca(u,v)<<"\n";
    }




}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
