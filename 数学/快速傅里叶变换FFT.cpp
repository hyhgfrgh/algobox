#include<bits/stdc++.h>
using namespace std;
#define int long long

const double PI = acos((double)-1.0);
struct Complex{
    double r,i;
    Complex():r(0),i(0){};
    Complex(double rr,double ii):r(rr),i(ii){};
    Complex operator+(Complex t){return Complex(r+t.r,i+t.i);}
    Complex operator-(Complex t){return Complex(r-t.r,i-t.i);}
    Complex operator*(Complex t){return Complex(r*t.r - i*t.i,r*t.i + i*t.r);}
};

void FFT(vector<Complex>& T,int n,int op){
    vector<int> R(n);
    for(int i = 0;i<n;i++){
        R[i] = R[i/2]/2+((i&1)?n/2:0);
    }
    for(int i = 0;i<n;i++){
        if(i<R[i]) swap(T[i],T[R[i]]);
    }
    for(int m = 2;m<=n;m<<=1){
        Complex w1(cos(2*PI/m),sin(2*PI/m)*op);
        for(int i = 0;i<n;i+=m){
            Complex wk(1,0);
            for(int j = 0;j<m/2;j++){
                Complex x = T[i+j],y = T[i+j+m/2]*wk;
                T[i+j] = x+y,T[i+j+m/2] = x-y,wk = wk*w1;
            }
        }
    }
}

void solve(){
    int n,m;cin>>n>>m;
    int lim = 1;
    while(lim<n+m+1) lim <<= 1;
    vector<Complex> A(lim),B(lim);
    for(int i = 0;i<=n;i++) cin>>A[i].r;
    for(int i = 0;i<=m;i++) cin>>B[i].r;
    FFT(A,lim,1);FFT(B,lim,1);
    for(int i = 0;i<lim;i++){
        A[i] = A[i]*B[i];
    }
    FFT(A,lim,-1);
    for(int i = 0;i<n+m+1;i++){
        cout<<(int)(A[i].r/(double)lim+0.5)<<" ";
    }
    cout<<"\n";
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
