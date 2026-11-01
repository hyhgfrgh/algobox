#include<bits/stdc++.h>
using namespace std;

// https://codeforces.com/contest/896/problem/C
// 适用随机数据，不然最坏能卡到n*q*log



#define int long long

long long qpow(int a,int n,int mod){
    a %= mod;
    long long res = 1;
    while(n){
        if(n&1) res *= a,res %= mod;
        a *= a,a %= mod;
        n >>= 1;
    }
    return res;
}

struct ODT{
    int n;
    map<int,int> odt;
    auto split(int pos){
        auto it = --odt.upper_bound(pos);
        if(it->first == pos) return it;
        odt[pos] = it->second;
        return odt.find(pos);
    }
    void add(int l,int r,int x){
        if(x == 0) return ;
        auto itl = split(l),itr = split(r+1);
        for(auto it = itl;it!=itr;++it){
            it->second += x;
        }
    }
    void assign(int l,int r,int x){
        auto itl = split(l),itr = split(r+1);
        odt.erase(itl,itr);
        odt[l] = x;
    }
    int powsum(int l,int r,int x,int y){
        bool youl = (odt.find(l) != odt.end());
        bool your = (odt.find(r+1) != odt.end());
        auto itl = split(l),itr = split(r+1);
        long long res = 0;
        for(auto it = itl;it != itr;it++){
            res = (res+qpow(it->second,x,y)*(next(it)->first-it->first)%y)%y;
        }
        if(!youl) odt.erase(itl);
        if(!your) odt.erase(itr);
        return res;
    }



    int kth(int l,int r,int k){
        bool youl = (odt.find(l) != odt.end());
        bool your = (odt.find(r+1) != odt.end());
        auto itl = split(l),itr = split(r+1);
        multiset<pair<int,int>> se;
        for(auto it = itl;it != itr;it++){
            se.insert({it->second,next(it)->first-it->first});
        }
        if(!youl) odt.erase(itl);
        if(!your) odt.erase(itr);
        for(auto [p,x]:se){
            k -= x;
            if(k<=0) return p;
        }
        return 0;
    }
};

void solve(){
    int n,m,seed,v;cin>>n>>m>>seed>>v;
    auto rng = [&]()->int{
        int ret = seed;
        seed = (seed*7+13)%(1000000007);
        return ret;
    };
    vector<int> a(n+1);
    ODT odt;
    for(int i = 1;i<=n;i++){
        a[i] = rng()%v+1;
        if(i == 1) odt.odt[i] = a[i];
        else if(a[i]!=a[i-1]) odt.odt[i] = a[i];
    }
    odt.odt[n+1] = 0;
    for(int _ = 1;_<=m;_++){
        int op = rng()%4+1,l = rng()%n+1,r = rng()%n+1;
        if(l>r) swap(l,r);
        int x = 0,y = 0;
        if(op == 3) x = rng()%(r-l+1)+1;
        else x = rng()%v+1;
        if(op == 4) y = rng()%v+1;

        if(op == 1) odt.add(l,r,x);
        else if(op == 2) odt.assign(l,r,x);
        else if(op == 3) cout<<odt.kth(l,r,x)<<"\n";
        else cout<<odt.powsum(l,r,x,y)<<"\n";
    }
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
