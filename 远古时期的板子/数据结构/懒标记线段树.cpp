#include<bits/stdc++.h>
using namespace std;

#define int long long

struct Info{
    int sum = 0;
    Info(int sum_ = 0):sum(sum_){}
};
struct Tag{
    int add,mul;
    Tag(int add_ = 0,int mul_ = 1):add(add_),mul(mul_){}
};

Info operator+(Info aa,Info bb){
    Info res;
    res.sum = aa.sum+bb.sum;
    return res;
}

struct tree{
    vector<Info> info;
    vector<Tag> tag;
    vector<int> ll,rr;
    tree(){}
    tree(vector<int> init_){
        int n = init_.size()-1;
        info.resize(4*n);tag.resize(4*n);
        ll.resize(4*n);rr.resize(4*n);
        auto build = [&](auto self,int p,int l,int r){
            ll[p] = l,rr[p] = r;
            if(l == r){
                info[p] = Info(init_[l]);
                return ;
            }
            int m = (l+r)/2;
            self(self,p*2,l,m);
            self(self,p*2+1,m+1,r);
            info[p] = info[p*2]+info[p*2+1];
        };
        build(build,1,1,n);
    }
    void apply(int p,Tag t){
        info[p].sum = info[p].sum*t.mul + (rr[p]-ll[p]+1)*t.add;
        tag[p].add = tag[p].add*t.mul+t.add;
        tag[p].mul *= t.mul;

    }
    void push(int p){
        apply(p*2,tag[p]);apply(p*2+1,tag[p]);
        tag[p] = Tag();
    }
    Info query(int p,int l,int r){
        if(ll[p]>=l and rr[p]<=r){
            return info[p];
        }
        int m = (ll[p]+rr[p])/2;
        push(p);
        Info res;
        if(l<=m) res = res+query(p*2,l,r);
        if(r>=m+1) res = res+query(p*2+1,l,r);
        return res;
    }
    Info query(int l,int r){
        return query(1,l,r);
    }
    void modify(int p,int l,int r,Tag t){
        if(ll[p]>=l and rr[p]<=r){
            apply(p,t);
            return ;
        }
        push(p);
        int m = (ll[p]+rr[p])/2;
        if(l<=m) modify(p*2,l,r,t);
        if(r>=m+1) modify(p*2+1,l,r,t);
        info[p] = info[p*2]+info[p*2+1];
    }
    void modify(int l,int r,Tag t){
        modify(1,l,r,t);
    }

};

void solve(){
    int n,q;cin>>n>>q;
    vector<int> a(n+1);
    for(int i = 1;i<=n;i++){
        cin>>a[i];
    }
    tree tr(a);
    while(q--){
        int op,x,y;cin>>op>>x>>y;
        if(op == 2){
            cout<<tr.query(x,y).sum<<"\n";
        }else{
            int k;cin>>k;
            tr.modify(x,y,Tag(k,1));
        }
    }
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    // cin>>T;
    while(T--) solve();
    return 0;
}
