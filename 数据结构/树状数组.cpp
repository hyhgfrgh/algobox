#include<bits/stdc++.h>
using namespace std;




struct tree{
    int n;
    vector<int> tr;
    tree(int nn){
        n = nn;
        tr.assign(n+10,0);
    }
    int lowbit(int x){
        return x&(-x);
    }

    void add(int x,int v){
        while(x<=n) tr[x]+=v,x += lowbit(x);
    }
    int query(int x){
        int res = 0;
        while(x>=1) res += tr[x],x -= lowbit(x);
        return res;
    }
};
