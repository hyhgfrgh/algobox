#include<bits/stdc++.h>
using namespace std;

#define int long long
const int mod = 998244353;
struct martix{
    int n,m;vector<vector<int>> mar;
    martix(int nn = 0,int mm = 0):n(nn),m(mm),mar(nn,vector<int> (mm,0)){};
    friend martix operator*(const martix& aa,const martix& bb){
        int a = aa.n,b = bb.m,c = aa.m;
        martix res(a,b);
        for(int i = 0;i<a;i++){
            for(int j = 0;j<c;j++){
                int t = aa.mar[i][j];
                for(int k = 0;k<b;k++){
                    res.mar[i][k] += t*bb.mar[j][k]%mod;
                    res.mar[i][k] = (res.mar[i][k]+mod)%mod;
                }
            }
        }
        return res;
    }
    martix& operator*=(const martix& bb){
        *this = (*this)*bb;
        return *this;
    }
    void print(){
        for(int i = 0;i<n;i++){
            for(int j = 0;j<m;j++){
                cout<<mar[i][j]<<" ";
            }
            cout<<"\n";
        }
        cout<<"===\n";
    }


};

martix qpow(martix a,long long n){
    martix res(a.n,a.m);
    for(int i = 0;i<a.n;i++) res.mar[i][i] = 1;
    while(n){
        if(n&1) res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}
long long qpow(long long a,long long n){
    a %= mod;
    long long res = 1;
    while(n){
        if(n&1) res *= a,res %= mod;
        a *= a,a %= mod;
        n >>= 1;
    }
    return res;
}

void solve(){



}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    cin>>T;
    while(T--) solve();
    return 0;
}
