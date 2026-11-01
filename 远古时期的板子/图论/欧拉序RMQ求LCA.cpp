#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve(){
    int n,q,s;cin>>n>>q>>s;
    vector<int> in(n+1),out(n+1);
    vector<vector<int>> e(n+1);
    for(int i = 2;i<=n;i++){
        int u,v;cin>>u>>v;
        e[u].emplace_back(v);
        e[v].emplace_back(u);
    }
    vector<int> euler(n*2+1),dep(n*2+1),first(n+1);
    int tot = 1;
    auto dfs = [&](auto self,int u,int fa,int d)->void{
        first[u] = tot;
        euler[tot] = u;
        dep[tot] = d;
        tot++;
        for(int v:e[u]){
            if(v == fa) continue;
            self(self,v,u,d+1);
            euler[tot] = u;
            dep[tot] = d;
            tot++;
        }
    };
    dfs(dfs,s,0,1);
    vector<vector<int>> st(21,vector<int> (2*n+1));
    for(int i = 1;i<=2*n;i++) st[0][i] = i;
    for(int k = 1;k<=20;k++){
        for(int i = 1;i<=2*n;i++){
            int x = st[k-1][i],y = st[k-1][min(i+(1LL<<(k-1)),2*n)];
            st[k][i] = (dep[x]<dep[y])?x:y;
        }
    }

    auto lca = [&](int u,int v)->int{
        int l = first[u],r = first[v];
        if(l>r) swap(l,r);
        int g = __lg(r-l+1);
        int x = st[g][l],y = st[g][r-(1LL<<g)+1];
        int p = dep[x]<dep[y]?x:y;
        return euler[p];
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
