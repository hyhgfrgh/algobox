#include<bits/stdc++.h>
using namespace std;

// 摩尔投票
// 寻找区间中出现次数大于n/k的元素，k小时适用


mt19937_64 rng(chrono::system_clock::now().time_since_epoch().count());
#define int long long
int k = 2;

vector<vector<int>> pos;

struct Info{
    // 这里用普通数组快很多比vector


    // vector<pair<int,int>> a;
    pair<int,int> a[2];
    Info(){
        // a.resize(k);
        a[0].first = 0;
        a[0].first = 0;
        a[1].second = 0;
        a[1].second = 0;

    }
};
Info operator+( Info aa, Info bb){
    // vector<pair<int,int>> a = aa.a;
    // vector<pair<int,int>> b = bb.a;
    auto a = aa.a;
    auto b = bb.a;
    for(int i = 0;i<k;i++){
        for(int j = 0;j<k;j++){
            if(a[j].first == b[i].first){
                a[j].second += b[i].second;
                b[i].first = 0;b[i].second = 0;
            }
        }
    }
    for(int i = 0;i<k;i++){
        if(b[i].second == 0) continue;
        bool con = false;
        for(int j = 0;j<k;j++){
            if(a[j].second == 0){
                a[j] = b[i];
                con = true;
                break;
            }
        }
        if(con) continue;
        int mn = 1e9,p = 0;
        for(int j = 0;j<k;j++){
            if(a[j].second<mn) p = j,mn = a[j].second;
        }
        if(mn>=b[i].second){
            for(int j = 0;j<k;j++){
                a[j].second -= b[i].second;
                if(a[j].second == 0){
                    a[j].first = 0;
                }
            }
        }else{
            for(int j = 0;j<k;j++){
                a[j].second -= mn;
                if(a[j].second == 0){
                    a[j].first = 0;
                }
            }
            for(int j = 0;j<k;j++){
                if(a[j].second == 0){
                    a[j].first = b[i].first;
                    a[j].second = b[i].second-mn;
                    break;
                }
            }
        }
    }
    Info res;
    res.a[0] = a[0];
    res.a[1] = a[1];
    return res;
}
struct Segment{
    vector<Info> info;
    int n;
    Segment(const vector<int>& a){
        n = a.size()-1;
        info.assign(4*(n+1),Info());
        function<void(int,int,int)> build = [&](int p,int l,int r)->void{

            if(l == r){
                info[p].a[0] = {a[l],1};
                return ;
            }
            int mid = (l+r)/2;
            build(p*2,l,mid);build(p*2+1,mid+1,r);
            info[p] = info[p*2]+info[p*2+1];
            // cout<<"build "<<l<<" "<<r<<"\n";
            // print(info[p]);
        };
        build(1,1,n);
    }
    void print(const Info& a){
        cout<<"-----\n";
        for(int i = 0;i<k;i++){
            cout<<a.a[i].first<<" "<<a.a[i].second<<"===\n";
        }
        cout<<"-----\n";
    }


    Info query(int p,int l,int r,int ql,int qr){
        if(l>=ql and  r<=qr){
            // cout<<"query "<<l<<" "<<r<<"\n";
            // print(info[p]);
            return info[p];
        }
        int mid = (l+r)/2;
        Info res;
        if(qr>=mid+1) res = res+query(p*2+1,mid+1,r,ql,qr);
        if(ql<=mid) res = res+query(p*2,l,mid,ql,qr);
        // cout<<"query "<<l<<" "<<r<<"\n";
        // print(res);
        return res;
    }
    int cnt(int l,int r,int x){
        return upper_bound(pos[x].begin(),pos[x].end(),r)-lower_bound(pos[x].begin(),pos[x].end(),l);
    }
    vector<int> ans(int l,int r){
        Info q = query(1,1,n,l,r);
        // print(q);
        // print(q);
        vector<int> res;
        int m = (r-l+1)/3;
        for(int i = 0;i<k;i++){
            if(cnt(l,r,q.a[i].first)>m){
                res.emplace_back(q.a[i].first);
            }
        }
        return res;
    }
};
void solve(){
    int n,q;cin>>n>>q;
    vector<int> a(n+1);
    for(int i = 1;i<=n;i++){
        cin>>a[i];
    }
    vector<int> b = a;
    sort(b.begin()+1,b.end());
    b.erase(unique(b.begin()+1,b.end()),b.end());
    vector<int> mp(n+1);
    for(int i = 1;i<=n;i++){
        int t = lower_bound(b.begin()+1,b.end(),a[i])-b.begin();
        mp[t] = a[i];a[i] = t;
        // cout<<a[i]<<" ";
        pos[a[i]].emplace_back(i);
    }
    // cout<<"\n";
    Segment tr(a);
    while(q--){
        int l,r;cin>>l>>r;
        vector<int> vec = tr.ans(l,r);
        // sort(vec.begin(),vec.end());
        int nn = vec.size();
        if(nn == 0) cout<<-1<<"\n";
        else if(nn == 1){
            cout<<mp[vec[0]]<<"\n";
        }else{
            cout<<mp[min(vec[0],vec[1])]<<" "<<mp[max(vec[0],vec[1])]<<"\n";
        }
    }
    for(int i = 1;i<=n;i++) pos[i].clear();
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    pos.resize(2e5+10);
    int T = 1;
    cin>>T;
    while(T--) solve();
    return 0;
}
