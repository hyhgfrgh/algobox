#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve(){
    int n,m;cin>>n>>m;
    vector<vector<int>> e(n+1);
    vector<int> dep(n+1),fa(n+1),son(n+1),top(n+1),siz(n+1),id(n+1);
    // vector<int> w(n+1),nw(n+1);
    int tot = 0;
    for(int i = 1;i<n;i++){
        int u,v;cin>>u>>v;
        e[u].emplace_back(v);
        e[v].emplace_back(u);
    }
    auto dfs1 = [&](auto self,int u)->void{
        siz[u] = 1,dep[u] = dep[fa[u]]+1;
        for(int v:e[u]){
            if(v == fa[u]) continue;
            fa[v] = u;
            self(self,v);
            siz[u] += siz[v];
            if(siz[son[u]]<siz[v]) son[u] = v;
        }
    };
    dfs1(dfs1,1);
    auto dfs2 = [&](auto self,int u,int t)->void{
        top[u] = t;id[u] = ++tot;
        // nw[tot] = w[u];
        if(!son[u] ) return ;
        self(self,son[u],t);
        for(int v:e[u]){
            if(v == fa[u] || v == son[u]) continue;
            self(self,v,v);
        }
    };
    dfs2(dfs2,1,1);
    auto lca = [&](int u,int v)->int{
        while(top[u] != top[v]){
            if(dep[top[u]]>dep[top[v]]) u = fa[top[u]];
            else v = fa[top[v]];
        }
        return dep[v]<dep[u]?v:u;
    };
    while(m--){
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
