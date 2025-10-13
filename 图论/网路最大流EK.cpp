#include<bits/stdc++.h>
using namespace std;

std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
#define int long long

struct node{
    int v,rev,c;
    node(int vv = 0,int r = 0,int cc = 0):v(vv),rev(r),c(cc){};
};

void solve(){
    int n,m,s,t;cin>>n>>m>>s>>t;
    vector<vector<node>> e(n+1);
    for(int i = 1;i<=m;i++){
        int u,v,w;cin>>u>>v>>w;
        e[u].emplace_back(v,e[v].size(),w);
        e[v].emplace_back(u,e[u].size()-1,0);
    }
    vector<int> mf(n+1);
    vector<pair<int,int>> pre(n+1);
    auto bfs = [&]()->bool{
        for(int i = 1;i<=n;i++) mf[i] = 0;
        queue<int> q;q.push(s);
        mf[s] = 1e18;
        while(q.size()){
            int u = q.front();q.pop();
            for(const auto& [v,rev,c]:e[u]){
                if(mf[v] == 0 and c){
                    mf[v] = min(mf[u],c);
                    q.push(v);
                    pre[v] = {rev,u};
                    if(v == t) return true;
                }
            }
        }
        return false;
    };
    auto EK = [&]()->int{
        int flow = 0;
        while(bfs()){
            int v = t;
            flow += mf[t];
            while(v != s){
                const auto& [rev,u] = pre[v];
                e[v][rev].c += mf[t];
                e[u][e[v][rev].rev].c -= mf[t];
                v = u;
            }

        }
        return flow;
    };
    int ans = EK();
    cout<<ans<<"\n";
}
signed main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    //std::cin>>T;
    while(T--) solve();
    return 0;
}
