#include<bits/stdc++.h>
using namespace std;

std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
#define int long long

// 匈牙利算法 二分图最大匹配

void solve(){
    int n,m,k;cin>>n>>m>>k;
    vector<vector<int>> e(n+1);
    vector<int> fa(m+1),vis(m+1);
    for(int i = 1;i<=k;i++){
        int u,v;cin>>u>>v;
        e[u].emplace_back(v);
    }
    auto dfs = [&](this auto&& dfs,int u)->bool{
        for(int v:e[u]){
            if(vis[v]) continue;
            vis[v] = 1;
            if(!fa[v] || dfs(fa[v])){
                fa[v] = u;
                return true;
            }
        }
        return false;
    };
    int ans = 0;
    for(int i = 1;i<=n;i++){
        for(int j = 1;j<=m;j++) vis[j] = 0;
        if(dfs(i)) ans++;
    }
    cout<<ans<<"\n";
}
signed main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    //std::cin>>T;
    while(T--) solve();
    return 0;
}
