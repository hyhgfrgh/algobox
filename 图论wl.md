# 图论

## 网络流

网络流是一种建好图以后，内置反悔机制的一种贪心策略，不需要我们人为去寻找贪心的反悔策略

容量:当前边最多经过的流量记作C(u,v)

流量:当前边实际经过的流量记作F(u,v)

残量: C(u,v)-F(u,v)

残量网络：把残量为0的边删去，剩下的图称作残量网络，对应边的边权也变为残量

增广路：与二分图中增广路定义不同，增广路指残量网络上源点到汇点的一条简单路径

割:割是一个边的集合，一个边集是割当且仅当去掉这些边后网络不在流通，即最大流为0

最大流最小割定理:一个网格的最大流等于其最小割

我们根据定义发现，只要存在增广路，流量就能不断增大，因此求最大流是一个求增广路的过程,并且这条增广路我们遵循能流满就流满的贪心策略

我们选择一条增广路，假设流量为f,我们正向边容量-f，反向边容量+f，这就是最开始我们说的内置反悔机制,也就是说我们可以通过反边流量进行反悔

由于EK算法不优于dinic,我们只介绍dinic算法

dinic是多路增广+当前弧优化的增广路算法

我们通过bfs给图分层，分层后多路增广,也就是同时累加多条增广路的流量

当前弧优化是指，流量已经流满的边无用，我们没必要对每个点每次都从头开始遍历，而是记录一个当前弧，直接从当前弧开始遍历

贪心的正确性全部基于退流反边对流量不断调整直至最优状态

### 拆点

注意到在流网络中，我们对于限制都是加在边上的。那么对于点的限制我们往往考虑拆点。具体的说，我们把一个点 U 拆成对应的Inu和Outu ，然后连边Inu->Outu，边的流量限制即为该点的流量限制。

### 最大流Dinic

```cpp
template<class T>
struct MaxFlow{
    struct _Edge{
        int to;
        T cap;
        _Edge(int _to,T _cap):to(_to),cap(_cap){};
    };

    int n;
    std::vector<_Edge> e;
    std::vector<std::vector<int>> h;
    std::vector<int> cur,d;

    MaxFlow(){};
    MaxFlow(int _n){
        init(_n);
    }
    void init(int _n){
        n=_n;
        e.clear();
        h.assign(n,{});
        cur.resize(n);
        d.resize(n);
    }
    void addEdge(int u,int v,T c){
        h[u].push_back(e.size());
        e.emplace_back(v,c);
        h[v].push_back(e.size());
        e.emplace_back(u,0);
    }

    bool bfs(int s,int t){
        d.assign(n,0);
        std::queue<int> q;
        d[s]=1;q.push(s);
        while(!q.empty()){
            int u=q.front();q.pop();
            for(int i:h[u]){
                auto [v,c]=e[i];
                if(c>0&&d[v]==0){
                    d[v]=d[u]+1;
                    if(v==t) return true;
                    q.push(v);
                }
            }
        }
        return false;
    }

    T dfs(int u,int t,T f){
        if(u==t) return f;
        T sum=f;
        for(int &i=cur[u];i<(int)h[u].size();i++){
            int j=h[u][i];
            auto [v,c]=e[j];
            if(c>0&&d[v] == d[u]+1){
                T a = dfs(v,t,std::min(sum,c));
                e[j].cap-=a;
                e[j^1].cap+=a;
                sum-=a;
                if(sum==0) return f;
            }
        }
        if(sum==f) d[u]=0;
        return f-sum;
    }

    T Dinic(int s,int t){
        T ans=0;
        while(bfs(s,t)){
            cur.assign(n,0);
            ans+=dfs(s,t,std::numeric_limits<T>::max());
        }
        return ans;
    }

    std::vector<bool> minCut(){
        std::vector<bool> c(n);
        for(int i=1;i<n;i++){
            c[i]=(d[i]!=0);
        }
        return c;
    }

    struct Edge {
        int from,to;
        T cap,flow;
    };
    std::vector<Edge> edges() {
        std::vector<Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }
};
```

\newpage

### 最小费用最大流

```cpp
template<class T>
struct MinCostFlow {
    int n;

    struct Edge {
        int to;
        T cap, cost;

        Edge(int to, T cap, T cost) : to(to), cap(cap), cost(cost) {}
    };

    std::vector<Edge> e;
    std::vector<std::vector<int>> h;
    std::vector<T> c, dis;
    std::vector<int> pre;

    MinCostFlow() {};

    MinCostFlow(int _n) {
        init(_n);
    }

    void init(int _n) {
        n = _n;
        e.clear();
        h.assign(n, {});
    }

    void addEdge(int u, int v, T cap, T cost) {
        h[u].push_back(e.size());
        e.emplace_back(v, cap, cost);
        h[v].push_back(e.size());
        e.emplace_back(u, 0, -cost);
    }

    bool dijkstra(int s, int t) {
        dis.assign(n, std::numeric_limits<T>::max());
        pre.assign(n, -1);
        std::priority_queue<std::pair<T, int>, std::vector<std::pair<T, int >>, std::greater<std::pair<T, int>>> pq;
        dis[s] = 0, pq.emplace(0, s);
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (dis[u] != d) {
                continue;
            }
            for (int i: h[u]) {
                auto [v, cap, cost] = e[i];
                if (cap > 0 && dis[v] > d + c[u] - c[v] + cost) {
                    dis[v] = d + c[u] - c[v] + cost;
                    pre[v] = i;
                    pq.emplace(dis[v], v);
                }
            }
        }
        return dis[t] != std::numeric_limits<T>::max();
    }

    std::pair<T, T> flow(int s, int t) {
        T flow = 0, cost = 0;
        c.assign(n, 0);
        while (dijkstra(s, t)) {
            for (int i = 0; i < n; i++) {
                c[i] += dis[i];
            }
            T aug = std::numeric_limits<int>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                aug = std::min(aug, e[pre[i]].cap);
            }
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                e[pre[i]].cap -= aug;
                e[pre[i] ^ 1].cap += aug;
            }
            flow += aug;
            cost += aug * c[t];
        }
        return std::make_pair(flow, cost);
    }

    struct _Edge {
        int from;
        int to;
        T cap;
        T cost;
        T flow;
    };

    std::vector<_Edge> edges() {
        std::vector<_Edge> a;
        for (int i = 0; i < e.size(); i += 2) {
            _Edge x;
            x.from = e[i + 1].to;
            x.to = e[i].to;
            x.cap = e[i].cap + e[i + 1].cap;
            x.cost = e[i].cost;
            x.flow = e[i + 1].cap;
            a.push_back(x);
        }
        return a;
    }

};
```

## 最近公共祖先lca

### 欧拉序+RMQ

O(1)求lca , RMQ返回区间dep最小的欧拉序数组下标 p ,那么 lca 就是 euler[p]

```cpp
void eulerRMQ(int n,int root,vector<int> &euler,vector<int> &dep,
                        vector<int> &first,vector<vector<int>>& e,vector<vector<int>> &st){
    euler = vector<int> (2*n+1);
    dep   = vector<int>(2*n+1); 
    first = vector<int>(n + 1);
    st = vector<vector<int>> (21,vector<int> (2*n+1));
    int tot = 1;
    auto dfs = [&](auto self, int u, int fa, int d) -> void {
        first[u] = tot;
        euler[tot] = u;
        dep[tot] = d;
        tot++;
        for (int v : e[u]) {
            if (v == fa) continue;
            self(self, v, u, d + 1);
            euler[tot] = u;
            dep[tot] = d;
            tot++;
        }
    };
    dfs(dfs, root, 0, 1);
    for (int i = 1; i <= 2 * n; i++) st[0][i] = i;
    for (int k = 1; k <= 20; k++) {
        for (int i = 1; i <= 2 * n; i++) {
            int x = st[k - 1][i], y = st[k - 1][min(i + (1LL << (k - 1)), 2 * n)];
            st[k][i] = (dep[x] < dep[y]) ? x : y;
        }
    }
}

void solve() {
    int n, q, root;
    cin >> n >> q >> root;
    vector<vector<int>> e(n + 1);
    for (int i = 2; i <= n; i++) {
        int u, v;
        cin >> u >> v;
        e[u].emplace_back(v);
        e[v].emplace_back(u);
    }
    vector<vector<int>> st;
    vector<int> euler, dep, first;
    eulerRMQ(n,root,euler,dep,first,e,st);
    auto lca = [&](int u, int v) -> int {
        int l = first[u], r = first[v];
        if (l > r) swap(l, r);
        int g = __lg(r - l + 1);
        int x = st[g][l], y = st[g][r - (1LL << g) + 1];
        int p = dep[x] < dep[y] ? x : y;
        return euler[p];
    };
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << "\n";
    }
}
```

\newpage

### 倍增求LCA

O(log(n)) 求 lca ,原理是先把u和v跳到同一个高度，然后再共同上跳知道最近的 lca

```cpp
void faLCA(int n, int root, vector<vector<int>>& adj, vector<vector<int>>& fa, vector<int>& dep) {
    fa = vector<vector<int>>(n + 1, vector<int>(21));
    dep = vector<int>(n + 1);
    auto dfs = [&](auto self, int u, int ffa) -> void {
        fa[u][0] = ffa;
        dep[u] = dep[ffa] + 1;
        for (int v : adj[u]) {
            if (v == ffa) continue;
            self(self, v, u);
        }
    };
    dfs(dfs, root, root);
    for (int k = 1; k <= 20; k++) {
        for (int i = 1; i <= n; i++) { fa[i][k] = fa[fa[i][k - 1]][k - 1]; }
    }
}

void solve() {
    int n, q, root;
    cin >> n >> q >> root;
    vector<vector<int>> adj(n + 1);
    for (int i = 2; i <= n; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].emplace_back(v);
        adj[v].emplace_back(u);
    }
    // static int fa[500010][21]; 洛谷模板题P3379换成这个才不会t
    vector<vector<int>> fa;
    vector<int> dep;
    faLCA(n, root, adj, fa, dep);
    auto lca = [&](int u, int v) -> int {
        if (dep[u] < dep[v]) swap(u, v);
        for (int k = 20; k >= 0; k--) {
            if (dep[fa[u][k]] < dep[v]) continue;
            u = fa[u][k];
        }
        if (u == v) return u;
        for (int k = 20; k >= 0; k--) {
            if (fa[u][k] != fa[v][k]) { u = fa[u][k], v = fa[v][k]; }
        }
        return fa[u][0];
    };
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << "\n";
    }
}
```
