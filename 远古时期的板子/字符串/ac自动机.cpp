#include<bits/stdc++.h>
using namespace std;

#define int long long

// P3796 AC自动机(简单版2)

struct node{
    array<int,26> son;
    int cnt = 0;
};

struct trie{
    vector<node> tr;
    vector<int> ne;
    trie(){
        tr.emplace_back(node());
    }
    int insert(string s){
        int p = 0;
        for(char c:s){
            if(!tr[p].son[c-'a']){
                tr[p].son[c-'a'] = tr.size();
                tr.emplace_back(node());
            }
            p = tr[p].son[c-'a'];
        }
        tr[p].cnt++;
        return p;
    }
    void build(){
        ne.resize(tr.size()+10);
        queue<int> q;
        for(int i = 0;i<26;i++){
            if(tr[0].son[i]) q.push(tr[0].son[i]);
        }
        while(q.size()){
            int u = q.front();q.pop();
            for(int i = 0;i<26;i++){
                int v = tr[u].son[i];
                if(v) ne[v] = tr[ne[u]].son[i],q.push(v);
                else tr[u].son[i] = tr[ne[u]].son[i];
            }
        }
    }
};

void solve(int n){
    trie tr;
    map<int,int> cnt;
    map<int,string> mp;
    while(n--){
        string s;cin>>s;
        mp[tr.insert(s)] = s;
    }
    tr.build();
    string s;cin>>s;
    n = s.size();
    s = " "+s;
    for(int i = 1,p = 0;i<=n;i++){
        p = tr.tr[p].son[s[i]-'a'];
        int tp = p;
        while(tp){
            if(tr.tr[tp].cnt)
                cnt[tp]+=tr.tr[tp].cnt;
            tp = tr.ne[tp];
        }
    }
    int ans = 0;
    for(auto [tp,siz]:cnt){
        ans = max(ans,siz);
    }
    cout<<ans<<"\n";
    for(auto [tp,siz]:cnt){
        if(siz == ans) cout<<mp[tp]<<"\n";
    }
}
signed main(){
    cin.tie(nullptr)->sync_with_stdio(false);
    int T = 1;
    cin>>T;
    while(T!=0) {solve(T);cin>>T;}
    return 0;
}
