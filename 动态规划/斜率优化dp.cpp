#include<bits/stdc++.h>
using namespace std;

#define ull signed long long
#define int long long

// P3195 玩具装箱

#define CINT \
// cin>>T;
void solve(){
    int n,l;cin>>n>>l;l++;
    vector<int> a(n+1);
    for(int i =1;i<=n;i++){
        cin>>a[i];
        a[i] += a[i-1]+1;
    }
    deque<pair<int,int>> q,line;
    auto cmp = [&](pair<int,int> a,pair<int,int> b)->bool{
        return (__int128)a.second*b.first<(__int128)a.first*b.second;
    };
    vector<int> f(n+1);
    q.push_back({0,0});
    for(int i = 1;i<=n;i++){
        int k = 2*(a[i]-l); // b = fi-(a[i]-l)*(a[i]-l) = y - k*x
        while(line.size() and cmp(line.front(),{1,k}))
            line.pop_front(),q.pop_front();
            auto [x,y] = q.front();
            f[i] = y-k*x+(a[i]-l)*(a[i]-l);
            x = a[i],y = f[i]+a[i]*a[i];
            while(line.size() and cmp({x-q.back().first,y-q.back().second},line.back()))
                q.pop_back(),line.pop_back();
                line.push_back({x-q.back().first,y-q.back().second});
                q.push_back({x,y});
    }
    cout<<f[n]<<"\n";

}
signed main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    CINT;
    while(T--) solve();
    return 0;
}
