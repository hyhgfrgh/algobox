#include<bits/stdc++.h>
using namespace std;

// 注意修改node中cnt的逻辑以及son数组的大小
// 本代码为洛谷P8306 ac代码

// #define int long long
struct node{
    array<int,100> son;
    int cnt = 0;
};

struct trie{
    vector<node> tr;
    trie(){
        tr.resize(1);
    };
    void insert(string s){
        int p = 0;
        for(int c:s){
            if(!tr[p].son[c-' ']) {
                tr[p].son[c-' '] = tr.size();
                tr.emplace_back(node());
            }
            p = tr[p].son[c-' '];
            tr[p].cnt++;
        }
    }
    int query(string s){
        int p = 0,res = 0;
        for(int c:s){
            if(tr[p].son[c-' ']) p = tr[p].son[c-' '];
            else{
                tr[p].son[c-' '] = tr.size();
                tr.emplace_back(node());
                p = tr[p].son[c-' '];
            }
        }
        res += tr[p].cnt;
        return res;
    }
};

void solve(){
    int n,q;cin>>n>>q;
    trie tr;
    for(int i = 1;i<=n;i++){
        string s;cin>>s;
        tr.insert(s);
    }
    while(q--){
        string s;cin>>s;
        cout<<tr.query(s)<<"\n";
    }

}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    cin>>T;
    while(T--) solve();
    return 0;
}
