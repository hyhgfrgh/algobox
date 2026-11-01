#include<bits/stdc++.h>
using namespace std;

std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
#define int long long

struct node{
    int to,cap,rev;
};

void solve(){
    int n,m,s,t;cin>>n>>m>>s>>t;
    vector<vector<node>> e(n+1);
    for(int i = 1;i<=m;i++){
        int u,v,w;cin>>u>>v>>w;
        e[u].emplace_back(v,w,e[v].size());
        e[v].emplace_back(u,0,e[u].size()-1);
    }
    vector<int> d(n+1);
    auto bfs = [&]()->bool{
        for(int i = 1;i<=n;i++) d[i] = 0;
        queue<int> q;q.push(s);
        d[s] = 1;
        while(q.size()){
            int u = q.front();q.pop();
            for(const auto& [v,c,rev]:e[u]){
                if(c and !d[v]){
                    d[v] = d[u]+1;
                    q.push(v);
                    if(v == t) return true;
                }
            }
        }
        return false;
    };
    vector<int> cur(n+1);
    auto dfs = [&](this auto&& dfs,int u,int mf)->int{
        if(u == t || mf == 0) return mf;
        int sum = 0;
        for(int& i = cur[u];i<(int)e[u].size();i++){
            auto& [v,c,rev] = e[u][i];
            if(c and d[v] == d[u]+1){
                int f = dfs(v,min(mf,c));
                if(f){
                    c -= f;e[v][rev].cap += f;
                    sum += f;
                    mf -= f;
                    if(mf == 0) break;
                }
            }
        }
        // if(sum == 0) d[u] = 0;
        return sum;
    };
    int ans = 0;
    while(bfs()){
        for(int i = 1;i<=n;i++) cur[i] = 0;
        ans += dfs(s,1e18);
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
