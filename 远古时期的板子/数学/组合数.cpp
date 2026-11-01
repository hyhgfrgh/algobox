
    // mod 须为质数

const int mod = 998244353;
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
vector<int> f,g;
void init(int size){
    f.resize(size+1);g.resize(size+1);
    f[0] = g[0] = 1;
    for(int i = 1;i<=size;i++){
        f[i] = 1LL*f[i-1]*i%mod;
        g[i] = 1LL*g[i-1]*qpow(i,mod-2)%mod;
    }
}
long long C(int a,int b){
    if(b>a || b<0) return 0;
    return 1LL*f[a]*g[b]%mod*g[a-b]%mod;
}
long long A(int a,int b){
    return 1LL*f[a]*g[a-b]%mod;
}
long long lucas(int a,int b){
    if(b>a || b<0) return 0;
    if(a == 0 || b == 0) return 1;
    return lucas(a/mod,b/mod)*C(a%mod,b%mod)%mod;
}


