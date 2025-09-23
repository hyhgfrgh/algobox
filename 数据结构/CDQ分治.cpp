#include<bits/stdc++.h>
using namespace std;

// 三维偏序   P3810


mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
#define int long long

struct fenwick{
    vector<int> tr;
    int n;
    fenwick(int nn){
        tr.resize(nn+10);
        n = nn;
    }
    void add(int p,int v){
        while(p<=n){
            tr[p] += v,p += p&-p;
        }
    }
    int sum(int p){
        int res = 0;
        while(p>=1) {
            res += tr[p],p -= p&-p;
        }
        return res;
    }
};

void solve(){
    int n,kk;cin>>n>>kk;
    vector<array<int,5>> a(n+2),b(n+2);
    map<array<int,3>,vector<int>> pos;
    vector<int> ans(n+1);
    for(int i = 1;i<=n;i++){ // unique
        cin>>b[i][0]>>b[i][1]>>b[i][2];
        pos[{b[i][0],b[i][1],b[i][2]}].emplace_back(i);
    }
    sort(b.begin()+1,b.end()-1);
    int idx = 0;
    for(int i = 1,cnt = 1;i<=n;i++,cnt++){
        if(b[i][0] != b[i+1][0] || b[i][1] != b[i+1][1] || b[i][2] != b[i+1][2]){
            a[++idx] = {b[i][0],b[i][1],b[i][2],cnt,0};
            cnt = 0;
        }
    }
    n = idx;
    // for(int i = 1;i<=n;i++){
    //     auto [x,y,z,cnt,tt] = a[i];
    //     cout<<x<<" "<<y<<" "<<z<<" "<<cnt<<" "<<tt<<"\n";
    // }
    // cout<<"====\n";

    fenwick tr(kk+1);
    auto cdq = [&](auto self,int l,int r)->void{
        if(l == r) return ;
        int mid = (l+r)/2;
        self(self,l,mid);self(self,mid+1,r);
        sort(a.begin()+l,a.begin()+mid+1,[&](array<int,5>& aa,array<int,5>& bb){
            return aa[1]<bb[1];
        });
        sort(a.begin()+mid+1,a.begin()+r+1,[&](array<int,5>& aa,array<int,5>& bb){
            return aa[1]<bb[1];
        });
        int i = l,j = mid+1;
        for(;j<=r;j++){
            for(;i<=mid and a[i][1]<=a[j][1];i++){
                tr.add(a[i][2],a[i][3]);
            }
            a[j][4] += tr.sum(a[j][2]);
        }

        for(int k = l;k<i;k++) tr.add(a[k][2],-a[k][3]);
    };
        cdq(cdq,1,n);
        for(int i = 1;i<=n;i++){
            ans[a[i][4]+a[i][3]-1]+=a[i][3];
        }
        for(int i = 0;i<(int)ans.size()-1;i++){
            cout<<ans[i]<<"\n";
        }

}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
