#include<bits/stdc++.h>
using namespace std;

std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
#define int long long
const long double pi = acosl(-1.0);
struct Complex{
    long double r,i;
    Complex(long double rr = 0,long double ii = 0):r(rr),i(ii){}
    Complex operator+(const Complex& t){return Complex(r+t.r,i+t.i);}
    Complex operator-(const Complex& t){return Complex(r-t.r,i-t.i);}
    Complex operator*(const Complex& t){return Complex(r*t.r-i*t.i,r*t.i+i*t.r);}
};
void FFT(vector<Complex>& a,int n,int op){
    vector<int> r(n);
    for(int i = 0;i<n;i++){
        r[i] = r[i/2]/2+n/2*(i&1);
    }
    for(int i = 0;i<n;i++){
        if(i<r[i]) swap(a[i],a[r[i]]);
    }
    for(int m = 2;m<=n;m<<=1){
        Complex w1(cos(2*pi/m),sin(2*pi/m)*op);
        for(int i = 0;i<n;i+=m){
            Complex wk(1,0);
            for(int j = 0;j<m/2;j++){
                Complex x = a[i+j],y = a[i+j+m/2]*wk;
                a[i+j] = x+y;a[i+j+m/2] = x-y;
                wk = wk*w1;
            }
        }
    }
}

void solve(){
    string n1,n2;cin>>n1>>n2;
    int n = n1.size(),m = n2.size();
    int N = 1<<(__lg(n+m+1)+1);
    vector<Complex> a(N),b(N);
    for(int i = 0,p = n-1;i<n;i++,p--) a[i].r = n1[p]-'0';
    for(int i = 0,p = m-1;i<m;i++,p--) b[i].r = n2[p]-'0';
    FFT(a,N,1);FFT(b,N,1);
    for(int i = 0;i<N;i++) a[i] = a[i]*b[i];
    FFT(a,N,-1);
    string ans;
    for(int i = 0,pre = 0;i<N;i++){
        int t = (int)(1.0*a[i].r/N+0.5)+pre;
        pre = t/10;t %= 10;
        ans.push_back((char)('0'+t));
        // cout<<(int)(1.0*a[i].r/N+0.5)<<" ";
    }
    while(ans.size()>1 and ans.back() == '0') ans.pop_back();
    reverse(ans.begin(),ans.end());
    cout<<ans<<"\n";
}
signed main(){
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    //std::cin>>T;
    while(T--) solve();
    return 0;
}
