#include<bits/stdc++.h>
using namespace std;

#define ull signed long long 
#define int long long 

// 洛谷P1776

#define CINT \
// cin>>T;
void solve(){
    int n,W;cin>>n>>W;
    vector<int> v(n+1),w(n+1),cnt(n+1);
    for(int i = 1;i<=n;i++){
        cin>>v[i]>>w[i]>>cnt[i];
    }
    vector<vector<int>> f(2,vector<int> (W+1));
    int st = 0;
    for(int i = 1;i<=n;i++){
        st ^= 1;
        for(int r = 0;r<w[i];r++){
            deque<int> q;
            for(int j = r;j<=W;j+=w[i]){
                int m = j/w[i];
                while(q.size() and f[st^1][q.back()*w[i]+r]-q.back()*v[i]<f[st^1][j]-m*v[i]){
                    q.pop_back();
                }
                while(q.size() and q.front()<m-cnt[i]){
                    q.pop_front();
                }
                q.push_back(m);
                f[st][j] = f[st^1][q.front()*w[i]+r]-q.front()*v[i]+m*v[i];
            }
        }
    }
    cout<<f[st][W]<<"\n";
}
signed main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    CINT;
    while(T--) solve();
    return 0;
}
