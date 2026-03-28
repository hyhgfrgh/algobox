# 数据结构

## 树状数组

```cpp
template <typename T>
struct Fenwick {
    int n;
    std::vector<T> a;

    Fenwick(int n_ = 0) {
        init(n_);
    }

    void init(int n_) {
        n = n_;
        a.assign(n+5, T{});
    }

    void add(int x, const T &v) {
        for (int i = x; i <= n; i += i & -i) {
            a[i] = a[i] + v;
        }
    }
    T sum(int x) {
        T ans{};
        for (int i = x; i > 0; i -= i & -i) {
            ans = ans + a[i];
        }
        return ans;
    }

    T rangeSum(int l, int r) {
        return sum(r) - sum(l);
    }
    
    int select(const T &k) {
        int x = 0;
        T cur{};
        for (int i = 1 << std::__lg(n); i; i /= 2) {
            if (x + i <= n && cur + a[x + i] <= k) {
                x += i;
                cur = cur + a[x];
            }
        }
        return x;
    }
};
```

\newpage

## 线段树

用数组初始化时数组应该为1-base

若用 `init(int n_, Info v_ = Info())` 初始化且为1-base时，n_记得传n+1

以下板子功能是单点修改维护最大子段和

```cpp
template<class Info>
struct SegmentTree {
    int n;
    std::vector<Info> info;

    SegmentTree() : n(0) {}

    SegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }

    template<class T>
    SegmentTree(std::vector<T> init_) {
        init(init_);
    }

    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }

    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size() - 1;
        info.assign(4 << std::__lg(n), Info());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r == l) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m + 1, r);
            pull(p);
        };
        build(1, 1, n);
    }

    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }

    void modify(int p, int l, int r, int x, const Info &v) {
        if (r == l) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (x <= m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m + 1, r, x, v);
        }
        pull(p);
    }

    void modify(int p, const Info &v) {
        modify(1, 1, n, p, v);
    }

    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        if (y <= m) {
            return rangeQuery(2 * p, l, m, x, y);
        } else if (x > m) {
            return rangeQuery(2 * p + 1, m + 1, r, x, y);
        } else {
            return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m + 1, r, x, y);
        }
    }

    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 1, n, l, r);
    }

    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m + 1, r, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 1, n, l, r, pred);
    }

    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (l == r) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findLast(2 * p + 1, m + 1, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 1, n, l, r, pred);
    }
};

struct Info {
    int pre, suf, sum, res;

    Info(int s = -1e18) : pre(s), suf(s), sum(s), res(s) {}
};

Info operator+(const Info &a, const Info &b) {
    Info c;
    c.sum = a.sum + b.sum;
    c.pre = std::max(a.pre, a.sum + b.pre);
    c.suf = std::max(b.suf, a.suf + b.sum);
    c.res = std::max({a.res, b.res, a.suf + b.pre});
    return c;
}
```

\newpage

## 懒标记线段树

用数组初始化时数组应该为1-base

若用  `init(int n_, Info v_ = Info())` 初始化且为1-base时，n_记得传n+1

+ 关于`findFist` 和 `findLast` 的使用，所查询的值一定要满足单调性才能二分

```cpp
template<class F>
int findFirst(int l, int r, F &&pred) {
    return findFirst(1, 1, n, l, r, pred);
}
```

 $pred$ 是传入的一个Lambda 表达式,表示搜索条件，例如：当线段树维护了**区间最大值**（`info[p].max`）时，你想找到范围内 **最后一个大于等于 $10$** 的数的位置这样使用：

```cpp
int pos = tree.findLast(1, 1, n, 1, n, [&](const Info &v) {
    return v.max >= 10;
});
```

```cpp
template<class Info, class Tag>
struct LazySegmentTree {
    int n;
    std::vector<Info> info;
    std::vector<Tag> tag;

    LazySegmentTree() : n(0) {}

    LazySegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }

    template<class T>
    LazySegmentTree(std::vector<T> init_) {
        init(init_);
    }

    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }

    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size() - 1;
        info.assign(4 << std::__lg(n), Info());
        tag.assign(4 << std::__lg(n), Tag());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r == l) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m + 1, r);
            pull(p);
        };
        build(1, 1, n);
    }

    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }

    void apply(int p, const Tag &v, int l = 0, int r = 0) {
        info[p].apply(v, r - l + 1);
        tag[p].apply(v);
    }

    void push(int p, int l = 0, int r = 0) {
        int m = (l + r) >> 1;
        apply(2 * p, tag[p], l, m);
        apply(2 * p + 1, tag[p], m + 1, r);
        tag[p] = Tag();
    }

    void modify(int p, int l, int r, int x, const Info &v) {
        if (l == r) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        push(p, l, r);
        if (x <= m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m + 1, r, x, v);
        }
        pull(p);
    }

    void modify(int p, const Info &v) {
        modify(1, 1, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        push(p, l, r);
        if (y <= m) {
            return rangeQuery(2 * p, l, m, x, y);
        } else if (x > m) {
            return rangeQuery(2 * p + 1, m + 1, r, x, y);
        } else {
            return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m + 1, r, x, y);
        }
    }
    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 1, n, l, r);
    }

    void rangeApply(int p, int l, int r, int x, int y, const Tag &v) {
        if (l > y || r < x) {
            return;
        }
        if (l >= x && r <= y) {
            apply(p, v, l, r);
            return;
        }
        int m = (l + r) / 2;
        push(p, l, r);
        rangeApply(2 * p, l, m, x, y, v);
        rangeApply(2 * p + 1, m + 1, r, x, y, v);
        pull(p);
    }

    void rangeApply(int l, int r, const Tag &v) {
        return rangeApply(1, 1, n, l, r, v);
    }

    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r == l) {
            return l;
        }
        int m = (l + r) / 2;
        push(p,l,r);
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m + 1, r, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findFirst(int l, int r, F &&pred) {
        return findFirst(1, 1, n, l, r, pred);
    }

    template<class F>
    int findLast(int p, int l, int r, int x, int y, F &&pred) {
        if (l > y || r < x) {
            return -1;
        }
        if (l >= x && r <= y && !pred(info[p])) {
            return -1;
        }
        if (r == l) {
            return l;
        }
        int m = (l + r) / 2;
        push(p,l,r);
        int res = findLast(2 * p + 1, m + 1, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }

    template<class F>
    int findLast(int l, int r, F &&pred) {
        return findLast(1, 1, n, l, r, pred);
    }
};

int mod;

struct Tag {
    int add = 0, mul = 1;

    Tag(int add_ = 0, int mul_ = 1) : add(add_), mul(mul_) {};

    void apply(Tag t) {
        add = (add * t.mul + t.add) % mod;
        mul = (mul * t.mul) % mod;
    }
};

struct Info {
    int sum = 0;

    Info(int sum_ = 0) : sum(sum_) {}

    void apply(Tag t, int len) {
        sum = (sum * t.mul + len * t.add) % mod;
    }
};

Info operator+(Info a, Info b) {
    return {(a.sum + b.sum)%mod};
}
```

\newpage

## 平衡树

平衡树是一种$BST$，也就是二叉搜索树。在理想状态下我们的操作复杂度为$O(log(n))$,但是在退化为链为复杂度将退化为$O(log(n))$，因此我们往往通过某些手段来尽可能维护树的平衡性，因此衍生出了不同种类的平衡树

目前我们主流的平衡树有两种，其一是 $FHQ-Treap$ ,其二是 $Splay$ ,其中 $FHQ$ 支持可持久化的操作,$Splay$在维护 $LCT$ 的时候复杂度更优，各自具有其优势。

平衡树同样具有两种用法，维护区间和维护值域，类似于区间线段树和权值线段树

相较于线段树而言，平衡树最大的功能是可以在某些位置进行插入删除操作并同时维护序列的信息，而线段树只具有修改的功能，但不能在序列的中间位置插入和删除.

由于$FHQ$更容易理解且更好写，我们往往只在考虑$LCT$时使用$Splay$。

### 红黑树

在$pbds$库中有内置的红黑树，我们传$std::less<T>$即为$set$,想要实现可重集合有两种方法，其一是传入 $std::less\_equal<T>$ ，其二是传入$std::less<std::pair<T,int>>$,第二维是一个全局自增变量，也就是通过第二维使得相同的数字被判定为不同。

```cpp
#include<bits/extc++.h>
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/tree_policy.hpp>

namespace pb = __gnu_pbds;
template <class T, class Cmp = std::less<T>>
// Cmp传std::less<T>就是Set 传std::less_equal<T>就是multiset
struct RedBlackTree
{
    // order_of_key(x) 返回严格小于x的元素个数
    // find_by_order(x) 返回从小到大第x+1个数的迭代器,从0开始计数
    // split(val,t)把>=val的部分给t
    const T inf = std::numeric_limits<T>::max() / 2;
    using Tree = pb::tree<T, pb::null_type, Cmp, pb::rb_tree_tag, pb::tree_order_statistics_node_update>;
    Tree tree;
    RedBlackTree() {}
    void insert(T x)
    {
        tree.insert(x);
    }
    void eraseOne(T x)
    {
        auto it = tree.find_by_order(tree.order_of_key(x)); // 更可靠的方式找到第一个x
        if (it != tree.end() and *it == x)
        {
            tree.erase(it);
        }
    }
    void eraseAll(T x)
    {
        Tree a, b;
        tree.split(x, a); // 把
        a.split(x + 1, b);
        tree.join(b);
    }
    int getNum(T x)
    {
        return tree.order_of_key(x + 1) - tree.order_of_key(x);
    }
    int allNum()
    {
        return tree.size();
    }
    T nxt(T x)
    {
        insert(x + 1);
        auto it = tree.find_by_order(tree.order_of_key(x + 1));
        ++it;
        if (it == tree.end())
        {
            return inf;
        }
        eraseOne(x + 1);
        return *it;
    }
    T pre(T x)
    {
        auto it = tree.find_by_order(tree.order_of_key(x));
        if (it == tree.begin())
        {
            return -inf;
        }
        --it;
        return *it;
    }
    // 3 2 2 1 1
    // 从大到小排
    int getRank(T x) // 默认最大的为rank1 相同元素我最大
    {
        return tree.size() - tree.order_of_key(x + 1) + 1;
    }
    T getVal(int rank)
    {
        return *tree.find_by_order(tree.size() - rank);
    }
    // 从小到大排
    int getAntiRank(T x)
    {
        return tree.order_of_key(x) + 1;
    }
    T getAntiVal(int rank)
    {
        return *tree.find_by_order(rank - 1);
    }
};
```

\newpage

## 哈希表

考虑使用安全的$gp\_hash\_table$来代替$umap$ ,比umap快很多

如果传入的$Key$ 不被直接支持就利用 $custom\_hash$ 把所有元素糅合一下

不考虑顺序的情况下完全可以代替map和umap,只是占的空间可能有一点大

```cpp
#include<bits/stdc++.h>
#include<bits/extc++.h> 
// 包含pbds库（部分编译器可能需要用#include <ext/pb_ds/hash_policy.hpp>）
// #include <ext/pb_ds/assoc_container.hpp>
// #include <ext/pb_ds/hash_policy.hpp>

// using namespace __gnu_pbds; 可选

#define ull unsigned long long
#define u64 uint64_t;
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // 保证输入分布均匀
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        // 使用 steady_clock 或地址作为种子，防止固定数据 Hack
        // 由于 static 不用担心传入同一个x会返回不同的值
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        // 核心：将输入先与随机盐异或，再跑一遍 SplitMix64
        return splitmix64(x ^ FIXED_RANDOM);
    }
};
// 使用方法
// __gnu_pbds::gp_hash_table<Key, Value, custom_hash> mp;
// 当map,umap用就可以
```

\newpage

## 主席树（可持久化线段树)

**静态区间第 k 小**

本质就是动态开点的（如果不离散化）可持久化权值线段树

操作是注意插入元素的值域，需要满足在 $[1,inf]$ 到范围内，如果出现负数考虑：

+ 离散化（值域很大也要离散化，大一点点无所谓）

+ 把所有数都加 $inf+1$ 使得值域变成 $[1,2*inf+1]$  

！！！（注意不要直接加1e9再-1e9）尤其是最后输出整数结果的时候-1e9,会自动转化为浮点数操作最后，比如像下面这样

```cpp
cout<<1e9<<"\n"<<666-1e9<<"\n";
// 输出
1e+09
-9.99999e+08
```

静态区间第 k 小

```cpp
template<class T>
struct PerSegTree {
    int tot{}, n{}, inf{};
    std::vector<int> rs, ls, cnt, root;
    std::vector<T> sum;

    PerSegTree() = default;

    PerSegTree(int _n, int _inf) {//版本数，最大值
        init(_n, _inf);
    }

    void init(int _n, int _inf) {
        n = _n; inf = _inf; tot = 0;
        rs.resize(1, 0);
        ls.resize(1, 0);
        cnt.resize(1, 0);
        sum.resize(1, 0);
        root.resize(n, 0);
        int estimate = _n * 40; // 预估节点数,留好空位尽量避免整体迁移
        ls.reserve(estimate);
        rs.reserve(estimate);
        cnt.reserve(estimate);
        sum.reserve(estimate);
    }

    // 在旧版本v的基础上，给p所在的区间开点并返回节点编号
    int apply(int v, int l, int r, T p) {
        // 第一层递归是给root[当前版本]动态开点，赋值为++tot,下面就是给当前操作所产生的长log的链依次开点
        int u = ++tot; 
        ls.emplace_back(ls[v]), rs.emplace_back(rs[v]); 
        cnt.emplace_back(cnt[v]+1), sum.emplace_back(sum[v]+p);
        if (l == r) return u;
        int mid = (l + r) >> 1;
        if (p <= mid) ls[u] = apply(ls[v], l, mid, p);
        else rs[u] = apply(rs[v], mid + 1, r, p);
        return u;
    }

    void apply(int v1, int v2, T p) {
        root[v1] = apply(root[v2], 1, inf, p);
    }
    // 二分查找
    // u是当前版本所操作的区间节点编号，v是旧版本对应区间的节点编号
    int query(int u, int v, int l, int r, int k) {
        if (l == r) return l;
        int s = cnt[ls[u]] - cnt[ls[v]];
        int mid = (l + r) >> 1;
        if (s >= k) return query(ls[u], ls[v], l, mid, k);
        else return query(rs[u], rs[v], mid + 1, r, k - s);
    }

    int query(int l, int r, int k) {
        return query(root[r], root[l], 1, inf, k);
    }
};

void solve(){
    int B = 1e9;
    int n,q;cin>>n>>q;
    vector<int> a(n+1);
    PerSegTree<int> tr(n+1,2*B+10);
    for(int i = 1;i<=n;i++){
        cin>>a[i];
        tr.apply(i,i-1,a[i]+B);
    }
    while(q--){
        int l,r,k;cin>>l>>r>>k;
        cout<<tr.query(l-1,r,k)-B<<"\n";
    }
}
```

\newpage

## 字典树

在 `insert` 和 `query` 中初始p为0,节点p不存信息，其26(小写26个英文字母)个son指向所有字符串的开头，根据需要维护的信息修改 node的变量 和 `insert` 和 `query` 

```cpp
struct Trie {
    struct node {
        std::array<int, 26> son;
        int cnt = 0;
        node() : son{} {}
    };

    std::vector<node> t;

    Trie() {
        init();
    }

    void init() {
        t.assign(1, node());
    }

    int newnode() {
        t.emplace_back();
        return t.size() - 1;
    }

    void insert(const std::string &val) {
        int p = 0;
        for (char c: val) {
            int ne = c - 'a';
            if (t[p].son[ne] == 0) {
                t[p].son[ne] = newnode();
            }
            p = t[p].son[ne];
        }
        t[p].cnt++;
    }

    int query(const std::string &val) {
        int p = 0;
        for (char c: val) {
            int ne = c - 'a';
            if (t[p].son[ne] == 0) {
                return 0;
            }
            p = t[p].son[ne];
        }
        return t[p].cnt;
    }
};
```

\newpage

### 可持久化字典树

**经典应用**:给定一个数 $x$，在区间 $[l,r]$ 中找一个数 $y$ 使得 $x$  ^  $y$  的值最大,下面的模板以 $01Trie$ 举例，$t$数组存树的信息，$ver[i]$表示第$i$个版本根节点的编号

```cpp
template<class T>
struct PerTrie {
    struct node{
        array<int,2> son;
        node():son{}{}
    };
    int tot = 0;
    std::vector<node> t;
    std::vector<int> ver, cnt;
    T inf = std::numeric_limits<T>::max();// 可根据数据范围自行改
    PerTrie() {
        init();
    }
    void init() {
        t.assign(1, {});
        ver.assign(1, {});
        cnt.assign(1, {});
    }
    // y是上一个版本的根节点编号，不要传错参数
    void add(T x, T y = 0) {
        int p = ++tot;
        ver.emplace_back(p);
        for (int i = std::__lg(inf); i >= ; i--) {
            int q = x >> i & 1; // q表示下一个二进制位
            while (p >= t.size())  t.emplace_back(); // 因为可能给某个节点已经分配了编号但是还没有分配空间
            t[p].son[q^1] = t[y].son[q^1];
            t[p].son[q] = ++tot;
            p = t[p].son[q], y = t[y].son[q];
            while (p >= cnt.size()) cnt.emplace_back(); 
            cnt[p] = cnt[y] + 1;
        } 
    }
    // 求x与区间[l+1,r]中某个元素xor的最大值
    // l,r注意要传入对应的根节点，即tr.version[l],tr.version[r]
    T query(int l, int r, T x) {
        T ans = 0;
        for (int i = std::__lg(inf); i >= 0; i--) {
            int q = x >> i & 1;
            // 没有被分配节点的编号为0,cnt[0] = 0
            if (cnt[t[r].son[q^1]] > cnt[t[l].son[q^1]]) {
                r = t[r].son[q^1], l = t[l].son[q^1], ans += 1ll << i;
            } else {
                r = t[r].son[q], l = t[l].son[q];
            }
        }
        return ans;
    }
    int version(int p) {
        return ver[p];
    }
    int size() { return ver.size();}
};
```

\newpage

**例题** 

给定一个非负整数序列 $\{a\}$，初始长度为 $N$ 

有 $M$ 个操作，有以下两种操作类型：  

1. `A x`：添加操作，表示在序列末尾添加一个数 $x$，序列的长度 $N$ 加 $1$ 。
2. `Q l r x`：询问操作，你需要找到一个位置 $p$，满足 $l \le p \le r$，使得：$a[p] \oplus a[p+1] \oplus ... \oplus a[N] \oplus x$ 最大，输出最大值。

```cpp
void solve(){
    int n,m;cin>>n>>m;
    PerTrie<int> tr;
    tr.add(0,0);// 此时根节点1已经被这个占用了，所有后面的都要偏移一位
    vector<int> a(n+1);
    for(int i = 1;i<=n;i++){
        cin>>a[i];a[i] ^= a[i-1];
        tr.add(a[i],tr.version(i));
    }
    while(m--){
        char o;cin>>o;
        if(o == 'A'){
            int x;cin>>x;
            a.emplace_back(x^a.back());
            n++;
            tr.add(a[n],tr.version(n));
        }else{
            int l,r,x;cin>>l>>r>>x;
            cout<<tr.query(tr.version(l-1), tr.version(r), a[n]^x)<<"\n";
        }
    }
}
```

\newpage

## 并查集

### 普通并查集

只维护了fa以及每个集合的元素数量，维护其他信息可以手搓

```cpp
struct DSU {
    std::vector<int> fa, siz;

    DSU() {}

    DSU(int n) {
        init(n);
    }

    void init(int n) {
        fa.resize(n);
        std::iota(fa.begin(), fa.end(), 0);
        siz.assign(n, 1);
    }

    int find(int x) {
        while (x != fa[x]) {
            x = fa[x] = fa[fa[x]];
        }
        return x;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
    // 把 y 所在集合的 fa 都指向 x 所在集合的 fa
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        siz[x] += siz[y];
        fa[y] = x;
        return true;
    }

    int size(int x) {
        return siz[find(x)];
    }
};
```

\newpage

### 带权并查集

```cpp
template<class T>
struct DSU {
    std::vector<int> fa, siz;
    std::vector<T> dis;
    DSU() {}
    DSU(int n) {
        init(n);
    }
    void init(int n) {
        fa.resize(n);
        dis.assign(n, 0);
        std::iota(fa.begin(), fa.end(), 0);
        siz.assign(n, 1);
    }

    int find(int x) {
        if (fa[x] == x) return x;
        int tmp = find(fa[x]);
        dis[x] += dis[fa[x]];
        return fa[x] = tmp;
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }
    // 把所有的y集合的元素接到x后面并维护到达x集合根节点的距离
    // 如果用到加法一定要注意负数取模 ！！！
    bool merge(int x, int y) {
        int fx = find(x);
        int fy = find(y);
        if (fx == fy) {
            return false;
        }
        // 维护先对位置一般d[fy] = d[x]+v-d[y];
        dis[fy] = siz[fx];
        siz[fx] += siz[fy];
        fa[fy] = fx;
        return true;
    }

    int size(int x) {
        return siz[find(x)];
    }

    T dist(int x, int y) {
        if (find(x) != find(y)) return -1;
        return dis[y] - dis[x];
    }

    T dist(int x) {
        return find(x), dis[x];
    }
};
```

**例题**:

Alice 和 Bob 在玩一个游戏：他写一个由 0 和 1 组成的序列。Alice 选其中的一段（比如第 3 位到第 5 位），问他这段里面有奇数个 1 还是偶数个 1。Bob 回答你的问题，然后 Alice 继续问。Alice 要检查 Bob 的答案，指出在 Bob 的第几个回答一定有问题。有问题的意思就是存在一个 01 序列满足这个回答前的所有回答，而且不存在序列满足这个回答前的所有回答及这个回答。

解：只需要维护d[i]表示二进制字符串中第i个位置前1的个数，给定l,r，就merge(l-1,r)，每次检查d[r]-d[l-1] mod 2 是不是1或者0就可以，此时merge操作中用d[fy] = d[x]+v-d[y]维护相对距离

\newpage

### 可撤销并查集

```cpp
struct DSU {
    int n;
    std::vector<int> siz, f;
    std::vector<std::array<int, 2>> history;
    DSU(int _n = 1) {
        init(_n);
    }
    void init(int _n) {
        this->n = _n;
        siz.assign(n, 1);
        f.resize(n);
        std::iota(f.begin(), f.end(), 0);
    }
    int find(int x) {
        while (f[x] != x) {
            x = f[x];
        }
        return x;
    }
    bool merge(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        if (siz[x] < siz[y]) {
            std::swap(x, y);
        }
        history.push_back({x, y});
        siz[x] += siz[y];
        f[y] = x;
        return true;
    }
    int time() {
        return history.size();
    }
    void rollback(int tm) {
        while (history.size() > tm) {
            auto [x, y] = history.back();
            history.pop_back();
            f[y] = y;
            siz[x] -= siz[y];
        }
    }
    int size(int p) {
        return siz[find(p)];
    }
    bool same(int u, int v) {
        return find(u) == find(v);
    }
};
```

\newpage

## 线性基

### 普通线性基

线性基是通过一系列的线性变换/异或变换把一系列的向量转化为一组线性无关的基$Base$ 

在整数的异或线性基中，向量即为每个数的二进制表示，每个二进制位对应向量的一个位

假如说原本的数组通过异或得到的所有数字的集合为$S$ ,通过导出的$Base$我们可以贪心解决异或最大值，$k$大值等问题。

当使用的G为bitset时稍微改改就行，比如:

`(!Base[i]) -> (Base[i] == 0) (此时0会转换成bitset<>(0)`

`(x & (1ll << i)) -> ((x>>i)[0])` 

其中查询第k小复杂度是 $log(inf)*log(inf)$ ，其他操作都是$log(inf)$ 

```cpp
/**
 * 线性基模板
 * T: 线性基的位数
 * G: 元素类型，通常为int或long long
 */
template <int T, class G> struct LineBase {
    std::vector<G> Base; // 存储线性基的数组
    bool flag = false;   // 标记是否存在无法插入的元素（即是否存在异或和为0的子集）
    // 从数组初始化线性基
    LineBase(const std::vector<G>& a) : Base(T, 0) {
        for (int i = 1; i < a.size(); ++i) { insert(a[i]); }
    }
    // 默认构造函数
    LineBase() : Base(T, 0) {}
    // 插入元素x到线性基中
    void insert(G x) {
        for (int i = T - 1; ~i; i--) {
            if (x & (1ll << i)) // 如果x的第i位为1
            {
                if (!Base[i]) // 如果线性基的第i位为空
                {
                    Base[i] = x; // 插入x
                    return;
                } else {
                    x ^= Base[i]; // 否则消去x的第i位
                }
            }
        }
        flag = true; // 执行到这里说明x被消为0，存在异或和为0的子集
    }
    // 判断x能否由线性基中的元素异或得到
    bool check(G x) {
        for (int i = T - 1; ~i; --i) {
            if (x & (1ll << i)) // 如果x的第i位为1
            {
                if (!Base[i]) // 线性基的第i位为空，无法表示
                {
                    return false;
                } else {
                    x ^= Base[i]; // 消元操作
                }
            }
        }
        return true; // 能消为0，说明可以表示
    }

    // 查询线性基能表示的最大异或和
    G qmax() {
        G res = 0;
        for (int i = T - 1; ~i; --i) {
            res = std::max(res, res ^ Base[i]); // 贪心选择能使结果更大的元素
        }
        return res;
    }

    // 查询线性基能表示的最小异或和
    G qmin() {
        if (flag) // 存在异或和为0的子集
        {
            return 0;
        }
        for (int i = 0; i < T; ++i) {
            if (Base[i]) // 找到第一个非零元素即为最小值
            {
                return Base[i];
            }
        }
        return 0; // 线性基为空的情况
    }

    // 查询第k小的异或和,复杂度 T^2
    G val(G k) {
        std::vector<G> tmp(T);
        G res = 0;
        G cnt = 0;
        k -= flag; // 减去0的情况
        if (!k) { return 0; }

        // 重构线性基，使其变为上三角形式
        for (int i = 0; i < T; ++i) {
            G now = Base[i];
            for (int j = i - 1; ~j; j--) {
                if (now & (1ll << j)) { now ^= Base[j]; }
            }
            if (now) { tmp[cnt++] = now; }
        }

        if (k >= (1ll << cnt)) // 超出范围
        {
            return -1;
        }

        // 根据k的二进制位选择对应的基向量
        for (int i = 0; i < cnt; ++i) {
            if (k & (1ll << i)) { res ^= tmp[i]; }
        }
        return res;
    }

    // 查询val的排名（即比val小的异或和数量+1）,复杂度 T
    G rank(int val) {
        if(!check(val)){
            return -1;
        }
        if (val > qmax()) // 超出最大值
        {
            return -1;
        }

        G res = 0;
        if (flag) // 存在0的情况
        {
            res++;
        }

        std::vector<G> st(T);
        int cnt = 0;

        // 收集所有非零基向量的位置
        for (int i = 0; i < T; ++i) {
            if (Base[i]) { st[cnt++] = i; }
        }

        // 计算排名
        for (int i = 0; i < cnt; ++i) {
            if (val >> st[i] & 1) {
                res += 1ll << i; // 累加贡献
            }
        }
        return res;
    }

    // 查询线性基能表示的不同异或和的数量
    G num() {
        return rank(qmax()); // 即最大值的排名
    }
};
```

### 前缀线性基

可以理解为可持久化线性基，本质就是建立了不同版本的线性基。我们用$p[idx][i]$表示第$idx$个版本的线性基第$i$个位的基，用$pos[idx][i]$表示该基最晚出现的位置（贪心），如果新加入的数可以导出某个基且出现位置更晚，我们就和原本的基贪心的交换位置。因为我们对于区间询问$[l,r]$，我们会查$r$版本的线性基，如果某个位置![image](https://cdn.nlark.com/yuque/__latex/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)的基的最晚出现位置$\geq l$，就说明这个基是可用的。

注意构造函数传入的参数数组是1-base的

```cpp
/**
 * 可持久化线性基模板
 * Z: 线性基的位数
 * G: 元素类型，通常为int或long long
 */
template <int Z, class G>
struct PreLineBase
{
    // p[i][j]: 第i个版本中第j位的线性基元素
    // pos[i][j]: 第i个版本中第j位线性基元素的插入位置
    std::vector<std::array<int, Z>> p;
    std::vector<std::array<int, Z>> pos;

    // 构造函数，从数组a初始化可持久化线性基
    // 数组a为 1-base
    PreLineBase(const std::vector<G> &a) : p(a.size()), pos(a.size())
    {
        // 初始化第0个版本
        for (int i = 0; i < Z; ++i)
        {
            p[0][i] = pos[0][i] = 0;
        }
        // 逐个插入元素，生成新版本
        for (int i = 1; i < a.size(); ++i)
        {
            insert(a[i], i);
        }
    }

    // 插入元素x，生成新版本idx
    void insert(G x, int idx)
    {
        // 继承前一个版本的数据
        p[idx] = p[idx - 1];
        pos[idx] = pos[idx - 1];
        int cur = idx;  // 当前元素的插入位置

        // 从高位到低位处理每一位
        for (int i = Z - 1; i >= 0; --i)
        {
            if (x >> i & 1)  // 如果x的第i位为1
            {
                if (!p[idx][i])  // 如果线性基的第i位为空
                {
                    p[idx][i] = x;     // 插入x
                    pos[idx][i] = cur; // 记录插入位置
                    break;
                }

                // 贪心策略：保留插入位置较后的元素
                if (pos[idx][i] < cur)
                {
                    std::swap(p[idx][i], x);
                    std::swap(pos[idx][i], cur);
                }

                // 消元操作
                x ^= p[idx][i];
            }
        }
    }

    // 查询区间[l,r]内元素能组成的最大异或和
    G querymax(int l, int r)
    {
        G ans = 0;
        // 从高位到低位贪心选择
        for (int i = Z - 1; i >= 0; --i)
        {
            // 只考虑插入位置在[l,r]内的元素
            if (pos[r][i] >= l)
            {
                ans = std::max(ans, ans ^ p[r][i]);
            }
        }
        return ans;
    }

    // 判断区间[l,r]内元素能否异或得到x
    bool isexist(int l, int r, G x)
    {
        // 从高位到低位处理
        for (int i = Z - 1; i >= 0; --i)
        {
            if ((x >> i & 1) and pos[r][i] < l)
            {
                // 如果x的第i位为1，但对应线性基元素的插入位置小于l，无法生成
                return false;
            }
            else if (x >> i & 1)
            {
                // 消元操作
                x ^= p[r][i];
            }
        }
        // 如果能消为0，则可以生成
        return x == 0;
    }
    // 判断区间[l,r]内元素能否异或得到0
    bool isexist0(int l, int r)
    {
        int cnt = 0;
        // 从高位到低位处理
        for (int i = Z - 1; i >= 0; --i)
        {
            if (pos[r][i] >= l)
            {
                cnt++;
            }
        }
        // 有一个元素没有贡献就说明被消掉了
        return cnt<r-l+1;
    }
};
```

\newpage

## 堆

对于 $std::priority\_queue$ 不再过多介绍

### 可删堆

（功能完全可以由 $multiset$ 取代，但是set的常数很大，如果被卡可以这样优化一下）

用两个 $std::priority\_queue$ 来实现，其中一个用来存要删除的数字，每次取$top()$时判断是否需要弹出即可。

```cpp
template<class T>
struct Heap
{
    Heap() {}
    std::priority_queue<T> h, del;
    void erase(T x)
    {
        del.push(x);
    }
    void add(T x)
    {
        h.push(x);
    }
    int size()
    {
        return h.size() - del.size();
    }
    void pop()
    {
        assert(size());
        while (del.size() and del.top() == h.top())
        {
            h.pop();
            del.pop();
        }
        h.pop();
    }
    T top()
    {
        assert(size());
        while (del.size() and del.top() == h.top())
        {
            h.pop();
            del.pop();
        }
        return h.top();
    }
};
```

\newpage

### 对顶堆

动态维护序列中第 $k$ 大的数字，使用一个大根堆和一个小根堆动态维护即可。

堆实现 只支持加数字

```cpp
template <class T>
struct Heap
{
    std::priority_queue<T> a;
    std::priority_queue<T, std::vector<T>, std::greater<T>> b;

    void add(T x)
    {
        if (b.empty() or x >= b.top())
        {
            b.push(x);
        }
        else
        {
            a.push(x);
        }
    }
    T query(int k)
    {
        while (b.size() > k)
        {
            a.push(b.top());
            b.pop();
        }
        while (b.size() < k)
        {
            b.push(a.top());
            a.pop();
        }
        return b.top();
    }
};
```

\newpage

$multiset$实现，额外支持删数字

```cpp
template <class T>
struct Heap
{
    std::multiset<T> a;
    std::multiset<T> b;
    void add(T x)
    {
        if (b.empty() || x >= *b.begin()){
            b.insert(x);
        }
        else{
            a.insert(x);
        }
    }
    // 删除元素（如果存在）
    bool remove(T x)
    {
        // 先检查是否在b中
        auto it = b.find(x);
        if (it != b.end()){
            b.erase(it);
            return true;
        }
        // 再检查是否在a中
        it = a.find(x);
        if (it != a.end()){
            a.erase(it);
            return true;
        }
        // 元素不存在
        return false;
    }
    T query(int k)
    {
        while (b.size() > k){
            auto val = *b.begin();
            b.erase(b.begin());
            a.insert(val);
        }
        while (b.size() < k){
            auto val = *a.rbegin();
            a.erase(std::prev(a.end()));
            b.insert(val);
        }
        return *b.begin();
    }
    int size() const
    {
        return a.size() + b.size();
    }
};
```

\newpage

### 可并堆（pbds实现）

请注意大根堆是$std::less$，小根堆是$std::greater$  洛谷p3377

使用教程：

我们在 $solve$ 函数内自己开一个迭代器数组，当加上某个数，删除某个数，修改某个数，需要根据返回的 $it$ 自动更改迭代器为新迭代器或者 $nullptr$ ，如果是删除堆顶，我们一开始传入数据时传入每个数字的下标，删除堆顶时手动把堆顶的下标对应的迭代器置为 $nullptr$ ，这样可以任意操作堆中的任一元素，而不是只能访问堆顶 

| **操作**       | **二项堆 (Binomial)**   | **配对堆 (Pairing)** | **备注**                    |
| ------------ | -------------------- | ----------------- | ------------------------- |
| **`push`**   | $O(\log n)$ (最坏)     | **$O(1)$**        | 配对堆直接挂载，二项堆可能要多次“进位”      |
| **`join`**   | $O(\log n)$ (最坏)     | **$O(1)$**        | 配对堆的神技，也是你模板里最核心的区别       |
| **`top`**    | $O(\log n)$ 或 $O(1)$ | **$O(1)$**        | 二项堆需要扫描森林中所有树的根（除非维护额外指针） |
| **`pop`**    | $O(\log n)$          | $O(\log n)$ (均摊)  | 性能瓶颈都在这里                  |
| **`modify`** | $O(\log n)$          | $O(\log n)$ (均摊)  | 配对堆实际常数极小                 |

```cpp
#include <bits/stdc++.h>
#include <bits/extc++.h> // 包含pbds库（部分编译器可能需要用#include <ext/pb_ds/priority_queue.hpp>）
namespace pbds = __gnu_pbds;
template <typename T, typename Compare = std::less<T>>
class mergeHeap {
private:
    // 也可以用 pbds::binomial_heap_tag，具体区别看表格
    using Heap = pbds::priority_queue<T, Compare, pbds::pairing_heap_tag>;
    Heap heap_;
public:
    using it = typename Heap::point_iterator;
    mergeHeap() = default;
    // 返回插入元素位置的迭代器
    it push(const T &value) { return heap_.push(value); }
    const T &top() const { return heap_.top(); }
    void pop() { heap_.pop(); }
    void erase(it t) { heap_.erase(t); }
    void merge(mergeHeap &other) {// 把other堆合并到自身，other变成空堆
        heap_.join(other.heap_); // pb_ds 内部会处理 other 是否为空
    }
    bool empty() const { return heap_.empty(); }
    size_t size() const { return heap_.size(); }
    void clear() { heap_.clear(); }
    // 使用内置的 modify 性能更好
    // modify操作并不会更改这个元素位置的迭代器，只是改变指向关系
    it modify(it t, const T &new_value) {
        heap_.modify(t, new_value);
        return t;
    }
};
template <typename T>
using MinBinomialHeap = mergeHeap<T, std::greater<T>>;
template <typename T>
using MaxBinomialHeap = mergeHeap<T, std::less<T>>;
```
