\newpage

## 前缀和/差分

```cpp
{
    // 一维部分省略 ...
}

{
    // 用二次差分在区间[l,r]上加上等差数列：首项s，公差d
   auto add_arithmetic = [&](vector<int>& d2, int l, int r, int s, int d) {
        if (l > r) return;
        d2[l] += s;
        if (l + 1 <= n + 1) d2[l + 1] += d - s;
        if (r + 1 <= n + 1) d2[r + 1] += -(s + (r - l + 1) * d);
        if (r + 2 <= n + 1) d2[r + 2] += s + (r - l) * d;
    };
}
{
    #include <iostream>
    #include <vector>

    using namespace std;

    // 定义我们要加的二次函数 f(x) = ax^2 + bx + c
    // 这里 x 是相对位置 (1, 2, 3...)
    long long f(long long x, long long a, long long b, long long c) {
        return a * x * x + b * x + c;
    }

    // 辅助函数：计算在原数组绝对下标 i 处，实际加上的函数值
    // 如果 i 小于起点 l，则无意义（在我们的算法中不会被调用到非法位置）
    long long g(long long i, long long l, long long a, long long b, long long c) {
        long long x = i - l + 1; // 转换为相对位置
        return f(x, a, b, c);
    }

    // 三阶差分数组的区间修改
    void add_quadratic(vector<long long>& d3, int l, int r, long long a, long long b, long long c) {
        // 1. 起点 l 产生的启动变化 (+1 倍)
        d3[l]     += g(l, l, a, b, c);
        d3[l + 1] += g(l + 1, l, a, b, c) - 3 * g(l, l, a, b, c);
        d3[l + 2] += g(l + 2, l, a, b, c) - 3 * g(l + 1, l, a, b, c) + 3 * g(l, l, a, b, c);

        // 2. 终点 r+1 产生的抵消变化 (-1 倍)
        d3[r + 1] -= g(r + 1, l, a, b, c);
        d3[r + 2] -= g(r + 2, l, a, b, c) - 3 * g(r + 1, l, a, b, c);
        d3[r + 3] -= g(r + 3, l, a, b, c) - 3 * g(r + 2, l, a, b, c) + 3 * g(r + 1, l, a, b, c);
    }

    int main() {
        int n = 10; // 数组长度
        // 差分数组需要开大一点，防止 r+3 越界。通常开 N + 5 即可
        vector<long long> d3(n + 5, 0); 
        vector<long long> d2(n + 5, 0);
        vector<long long> d1(n + 5, 0);
        vector<long long> a_arr(n + 5, 0);

        // 示例：在区间 [2, 5] 加上 f(x) = x^2 (a=1, b=0, c=0)
        add_quadratic(d3, 2, 10, 1, 1, 0);

        // 还原原数组：做 3 次前缀和
        for (int i = 1; i <= n; ++i) {
            d2[i] = d2[i - 1] + d3[i];      // 三阶 -> 二阶
            d1[i] = d1[i - 1] + d2[i];      // 二阶 -> 一阶
            a_arr[i] = a_arr[i - 1] + d1[i]; // 一阶 -> 原数组

            cout << a_arr[i] << "  ";
        }
        // 输出预期: 0 1 4 9 16 0 0 0 0 0
        cout << endl;

        return 0;
    }
}
{
    // 三维前缀和  示例
    int a,b,c;cin>>a>>b>>c;
    vector pre(a+1,vector (b+1,vector<int> (c+1)));
    for(int i = 1;i<=a;i++){
        for(int j = 1;j<=b;j++){
            for(int k = 1;k<=c;k++){
                cin>>pre[i][j][k];
            }
        }
    }
    for(int k = 1;k<=a;k++){
        for(int j = 1;j<=b;j++){
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k][j][i-1];
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k][j-1][i];
        }
        for(int j = 1;j<=b;j++){
            for(int i = 1;i<=c;i++)
                pre[k][j][i] += pre[k-1][j][i];
        }
    }
    cout<<pre[a][b][c]<<"\n";
}
{
    ......
}
```

\newpage

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

### 维护区间最值

```cpp
template <class T>
class RMQSegmentTree
{
#define lc u << 1
#define rc u << 1 | 1
    const T inf = std::numeric_limits<T>::max() / 2;

public:
    struct Node
    {
        int l, r;
        T max, min;
    };
    RMQSegmentTree(const std::vector<T> &a) : n(a.size()), tr(4 * n)
    {
        n--;
        std::function<void(int, int, int)> build = [&](int u, int l, int r)
        {
            tr[u] = {l, r, a[l], a[l]};
            if (l == r)
            {
                return;
            }
            int mid = (l + r) >> 1;
            build(lc, l, mid);
            build(rc, mid + 1, r);
            pushup(u);
        };
        build(1, 1, n);
    }
    void singleChange(int pos, T val)
    {
        singleChange(1, 1, n, pos, val);
    }
    T rangeQueryMax(int l, int r)
    {
        return rangeQueryMax(1, 1, n, l, r);
    }
    T rangeQueryMin(int l, int r)
    {
        return rangeQueryMin(1, 1, n, l, r);
    }

private:
    int n;
    std::vector<Node> tr;
    void pushup(int u)
    {
        tr[u].max = std::max(tr[lc].max, tr[rc].max);
        tr[u].min = std::min(tr[lc].min, tr[rc].min);
    }

    void singleChange(int u, int l, int r, int x, T y)
    {
        if (l == r)
        {
            tr[u].max = tr[u].min = y;
            return;
        }
        int mid = (l + r) >> 1;
        if (x <= mid)
        {
            singleChange(lc, l, mid, x, y);
        }
        else
        {
            singleChange(rc, mid + 1, r, x, y);
        }
        pushup(u);
    }
    T rangeQueryMax(int u, int l, int r, int x, int y)
    {
        if (x <= l && y >= r)
        {
            return tr[u].max;
        }
        int mid = (l + r) >> 1;
        T res = -inf;
        if (x <= mid)
        {
            res = std::max(res, rangeQueryMax(lc, l, mid, x, y));
        }
        if (y > mid)
        {
            res = std::max(res, rangeQueryMax(rc, mid + 1, r, x, y));
        }
        return res;
    }
    T rangeQueryMin(int u, int l, int r, int x, int y)
    {
        if (x <= l && y >= r)
        {
            return tr[u].min;
        }
        int mid = (l + r) >> 1;
        T res = inf;
        if (x <= mid)
        {
            res = std::min(res, rangeQueryMin(lc, l, mid, x, y));
        }
        if (y > mid)
        {
            res = std::min(res, rangeQueryMin(rc, mid + 1, r, x, y));
        }
        return res;
    }
};
```

\newpage

### 维护区间修改

```cpp
template <class T>
class SegmentTree
{
#define lc u << 1
#define rc u << 1 | 1
public:
    struct Node
    {
        int l, r;
        T add, sum;
    };

    SegmentTree(const std::vector<T> &a) : n(a.size()), tr(4 * n)
    {
        n--;
        std::function<void(int, int, int)> build = [&](int u, int l, int r)
        {
            tr[u] = {l, r, 0, a[l]};
            if (l == r)
            {
                return;
            }
            int mid = (l + r) >> 1;
            build(lc, l, mid);
            build(rc, mid + 1, r);
            pushup(u);
        };
        build(1, 1, n);
    }
    void rangeAdd(int l, int r, T val)
    {
        rangeAdd(1, 1, n, l, r, val);
    }
    T rangeQuerySum(int l, int r)
    {
        return rangeQuerySum(1, 1, n, l, r);
    }

private:
    int n;
    std::vector<Node> tr;
    void pushup(int u)
    {
        tr[u].sum = tr[lc].sum + tr[rc].sum;
    }
    void pushdown(Node &u, T add)
    {
        u.sum += add * (u.r - u.l + 1);
        u.add += add;
    }
    void pushdown(int u)
    {
        if (tr[u].add)
        {
            pushdown(tr[lc], tr[u].add);
            pushdown(tr[rc], tr[u].add);
            tr[u].add = 0;
        }
    }
    void rangeAdd(int u, int l, int r, int x, int y, T k)
    {
        if (x <= l && y >= r)
        {
            pushdown(tr[u], k);
            return;
        }
        int mid = (l + r) >> 1;
        pushdown(u);
        if (x <= mid)
        {
            rangeAdd(lc, l, mid, x, y, k);
        }
        if (y > mid)
        {
            rangeAdd(rc, mid + 1, r, x, y, k);
        }
        pushup(u);
    }
    T rangeQuerySum(int u, int l, int r, int x, int y)
    {
        if (x <= l && y >= r)
        {
            return tr[u].sum;
        }
        int mid = (l + r) >> 1;
        pushdown(u);
        T res = 0;
        if (x <= mid)
        {
            res += rangeQuerySum(lc, l, mid, x, y);
        }
        if (y > mid)
        {
            res += rangeQuerySum(rc, mid + 1, r, x, y);
        }
        return res;
    }
};
```

乘标记和加标记的处理方法：

区间乘和区间加两个操作，关键在于维护乘法标记和加法标记之间的关系

具体地说，我们在下传标记时，采用的计算和的方法为$sum=sum*mul+add*len$，那么我们就要保证下传标记的时候正确操作，即$u.add=u.add*mul+add$, $u.mul=u.mul*mul$

覆盖标记和加标记的处理方法：

区间覆盖和区间加，关键在于懒标记之间的清除关系，在我们操作$cover$标记时，要清空对应位置的$add$标记，这样才能保证信息维护的正确性。具体来讲，什么叫清空对应位置的$add$标记呢？也就是说，一旦我们更新了$u$位置的$cover$信息，我们就要立刻把$u$位置的$add$信息清除掉，因为显然他不会再起到作用，而且在$pushdown$的时候我们要先处理$cover$标记，原因仍然是我们可能会清除$add$标记

\newpage

### 维护最大子段和

支持单点修改和区间查询最大子段和，不支持区间修改操作。

```cpp
template <class T>
class SegmentTree
{
#define lc u << 1
#define rc u << 1 | 1

public:
    struct Node
    {
        int l, r;
        T sum, lmax, rmax, max;
    };
    SegmentTree(const std::vector<T> &a) : n(a.size()), tr(n * 4)
    {
        n--;
        std::function<void(int, int, int)> build = [&](int u, int l, int r)
        {
            tr[u] = {l, r, a[l], a[l], a[l], a[l]};
            if (l == r)
            {
                return;
            }
            int mid = l + r >> 1;
            build(lc, l, mid);
            build(rc, mid + 1, r);
            pushup(tr[u], tr[lc], tr[rc]);
        };
        build(1, 1, n);
    }
    T rangeQuery(int x, int y)
    {
        Node ans = rangeQuery(1, 1, n, x, y);
        return ans.max;
    }

private:
    int n;
    std::vector<Node> tr;

    void pushup(Node &u, Node l, Node r)
    {
        u.sum = l.sum + r.sum;
        u.lmax = std::max(l.lmax, l.sum + r.lmax);
        u.rmax = std::max(r.rmax, r.sum + l.rmax);
        u.max = std::max({l.max, r.max, l.rmax + r.lmax});
    }

    Node rangeQuery(int u, int l, int r, int x, int y)
    {
        if (x <= l && y >= r)
        {
            return tr[u];
        }
        int mid = l + r >> 1;
        if (y <= mid)
        {
            return rangeQuery(lc, l, mid, x, y);
        }
        if (x > mid)
        {
            return rangeQuery(rc, mid + 1, r, x, y);
        }
        Node t;
        pushup(t, rangeQuery(lc, l, mid, x, y), rangeQuery(rc, mid + 1, r, x, y));
        return t;
    }
};
```

### 动态开点

往往我们的序列非常大，但大多数为空，我们并不需要维护整个序列的信息，因此我们可以采用动态开空间的方法，于是有动态开点线段树，具体的说，就是用到哪里的空间就开哪里的空间,我们每需要访问一个节点$u$，假如节点$u$尚未分配空间，我们手动为其开空间即可，因为操作次数往往有限，每次操作我们最多开$logn$的空间，所以空间复杂度是$qlogn$的

```cpp
template <class T>
class SegmentTree
{
public:
    SegmentTree(int capacity, int min, int max) : max(max), min(min)
    {
        lc.assign(capacity + 1, 0);
        rc.assign(capacity + 1, 0);
        sum.assign(capacity + 1, 0);
        add.assign(capacity + 1, 0);
    }

    void rangeAdd(int l, int r, T val)
    {
        rangeAdd(root, min, max, l, r, val);
    }
    T rangeQuerySum(int l, int r)
    {
        return rangeQuerySum(root, min, max, l, r);
    }

private:
    int min,
        max;
    int tot = 0;
    int root = 0;
    std::vector<int> lc, rc;
    std::vector<T> sum, add;
    void pushup(int u)
    {
        sum[u] = sum[lc[u]] + sum[rc[u]];
    }
    void pushdown(int u, int l, int r)
    {
        if (add[u])
        {
            lc[u] = (lc[u] ? lc[u] : ++tot);
            rc[u] = (rc[u] ? rc[u] : ++tot);
            int mid = (l + r) >> 1;
            sum[lc[u]] += (mid - l + 1) * add[u];
            sum[rc[u]] += (r - mid) * add[u];
            add[lc[u]] += add[u];
            add[rc[u]] += add[u];
            add[u] = 0;
        }
    }
    void rangeAdd(int &u, int l, int r, int x, int y, T val)
    { // 区修
        if (!u)
        {
            u = ++tot; // 动态开点
        }
        if (x <= l && r <= y)
        {
            sum[u] += val * (r - l + 1);
            add[u] += val;
            return;
        }
        pushdown(u, l, r);
        int mid = (l + r) >> 1;
        if (x <= mid)
        {
            rangeAdd(lc[u], l, mid, x, y, val);
        }
        if (y > mid)
        {
            rangeAdd(rc[u], mid + 1, r, x, y, val);
        }
        pushup(u);
    }
    T rangeQuerySum(int u, int l, int r, int x, int y)
    {
        if (x <= l and y >= r)
        {
            return (u ? sum[u] : 0);
        }
        int mid = (l + r) >> 1;
        pushdown(u, l, r);
        T res = 0;
        if (x <= mid)
        {
            res += rangeQuerySum(lc[u], l, mid, x, y);
        }
        if (y > mid)
        {
            res += rangeQuerySum(rc[u], mid + 1, r, x, y);
        }
        return res;
    }
};
```

\newpage

## 平衡树

平衡树是一种BST，也就是二叉搜索树。在理想状态下我们的操作复杂度为O(log(n)),但是在退化为链为复杂度将退化为O(log(n))，因此我们往往通过某些手段来尽可能维护树的平衡性，因此衍生出了不同种类的平衡树

目前我们主流的平衡树有两种，其一是 FHQ-Treap ,其二是 Splay ,其中 FHQ 支持可持久化的操作,Splay在维护 LCT 的时候复杂度更优，各自具有其优势。

平衡树同样具有两种用法，维护区间和维护值域，类似于区间线段树和权值线段树

相较于线段树而言，平衡树最大的功能是可以在某些位置进行插入删除操作并同时维护序列的信息，而线段树只具有修改的功能，但不能在序列的中间位置插入和删除.

由于FHQ更容易理解且更好写，我们往往只在考虑LCT时使用Splay。

### 红黑树

在pbds库中有内置的红黑树，我们传std::less即为set,想要实现可重集合有两种方法，其一是传入 std::less\_equal ，其二是传入std::less>,第二维是一个全局自增变量，也就是通过第二维使得相同的数字被判定为不同。

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

考虑使用安全的gp\_hash\_table来代替umap ,比umap快很多

如果传入的Key 不被直接支持就利用 custom\_hash 把所有元素糅合一下

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

操作是注意插入元素的值域，需要满足在 [1,inf] 到范围内，如果出现负数考虑：

- 离散化（值域很大也要离散化，大一点点无所谓）

- 把所有数都加 inf+1 使得值域变成 [1,2*inf+1]

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

### 字典树

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

**经典应用**:给定一个数 x，在区间 [l,r] 中找一个数 y 使得 x ^ y 的值最大,下面的模板以 01Trie 举例，t数组存树的信息，ver[i]表示第i个版本根节点的编号

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

给定一个非负整数序列 \{a\}，初始长度为 N

有 M 个操作，有以下两种操作类型：

1. `A x`：添加操作，表示在序列末尾添加一个数 x，序列的长度 N 加 1 。
2. `Q l r x`：询问操作，你需要找到一个位置 p，满足$l \le p \le r$，使得：$a[p] \oplus a[p+1] \oplus ... \oplus a[N] \oplus x$ 最大，输出最大值。

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
    bool merge(int x, int y,int v) {
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

\newpage

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

线性基是通过一系列的线性变换/异或变换把一系列的向量转化为一组线性无关的基Base

在整数的异或线性基中，向量即为每个数的二进制表示，每个二进制位对应向量的一个位

假如说原本的数组通过异或得到的所有数字的集合为S ,通过导出的Base我们可以贪心解决异或最大值，k大值等问题。

当使用的G为bitset时稍微改改就行，比如:

`(!Base[i]) -> (Base[i] == 0) (此时0会转换成bitset<>(0)`

`(x & (1ll << i)) -> ((x>>i)[0])`

其中查询第k小复杂度是 log(inf)*log(inf) ，其他操作都是log(inf)

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

\newpage

### 前缀线性基

可以理解为可持久化线性基，本质就是建立了不同版本的线性基。我们用p[idx][i]表示第idx个版本的线性基第i个位的基，用pos[idx][i]表示该基最晚出现的位置（贪心），如果新加入的数可以导出某个基且出现位置更晚，我们就和原本的基贪心的交换位置。因为我们对于区间询问[l,r]，我们会查r版本的线性基，如果某个位置$i$的基的最晚出现位置$\geq l$，就说明这个基是可用的。

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
        for (int i = 0; i < Z; ++i){
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
        for (int i = Z - 1; i >= 0; --i){
            if (pos[r][i] >= l){
                cnt++;
            }
        }
        // 有一个元素没有贡献就说明被消掉了
        return cnt<r-l+1;
    }
};
```

\newpage

## ST表

#### 返回权值（返回区间最大值,需要最小值就改max）

```cpp
template <class T>
class ST
{
    std::vector<std::vector<T>> st;

public:
    ST(const std::vector<T> &a)
    {
        int n = a.size() - 1, logn = std::__lg(n);
        st.assign(logn + 1, std::vector<int>(n + 1));
        for (int i = 1; i <= n; ++i)
        {
            st[0][i] = a[i];
        }
        for (int j = 1; j <= logn; ++j)
        {
            for (int i = 1; i + (1 << j) - 1 <= n; ++i) // n-i+1=(1<<j)是边界
            {
                st[j][i] = std::max(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            } // i是当前位置 j是向右扩展(1<<j)个数字
        }
    }

    T operator()(int l, int r)
    {
        int log = std::__lg(r - l + 1);
        return std::max(st[log][l], st[log][r - (1 << log) + 1]);
    }
};
```

\newpage

#### 返回位置（返回权值最大的下标）

```cpp
template <class T>
class ST
{
    std::vector<std::vector<int>> st;
    std::vector<T> A;

public:
    ST(const std::vector<T> &a)
    {
        A = a;
        int n = a.size() - 1, logn = std::__lg(n);
        st.assign(logn + 1, std::vector<int>(n + 1));
        for (int i = 1; i <= n; ++i)
        {
            st[0][i] = i;
        }
        for (int j = 1; j <= logn; ++j)
        {
            for (int i = 1; i + (1 << j) - 1 <= n; ++i)
            {
                st[j][i] = (a[st[j - 1][i]] > a[st[j - 1][i + (1 << (j - 1))]] 
                    ? st[j - 1][i] : st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    int operator()(int l, int r)
    {
        int log = std::__lg(r - l + 1);
        return (A[st[log][l]] > A[st[log][r - (1 << log) + 1]] 
                    ? st[log][l] : st[log][r - (1 << log) + 1]);
    }
};
```

\newpage

## 堆

对于 std::priority\_queue 不再过多介绍

### 可删堆

（功能完全可以由 multiset 取代，但是set的常数很大，如果被卡可以这样优化一下）

用两个 std::priority\_queue 来实现，其中一个用来存要删除的数字，每次取top()时判断是否需要弹出即可。

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

动态维护序列中第 k 大的数字，使用一个大根堆和一个小根堆动态维护即可。

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

multiset实现，额外支持删数字

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

请注意大根堆是std::less，小根堆是std::greater 洛谷p3377

使用教程：

我们在 solve 函数内自己开一个迭代器数组，当加上某个数，删除某个数，修改某个数，需要根据返回的 it 自动更改迭代器为新迭代器或者 nullptr ，如果是删除堆顶，我们一开始传入数据时传入每个数字的下标，删除堆顶时手动把堆顶的下标对应的迭代器置为 nullptr ，这样可以任意操作堆中的任一元素，而不是只能访问堆顶

| **操作**       | **二项堆 (Binomial)** | **配对堆 (Pairing)** | **备注**                    |
| ------------ | ------------------ | ----------------- | ------------------------- |
| **`push`**   | $O(\log n)$ (最坏)   | **O(1)**          | 配对堆直接挂载，二项堆可能要多次“进位”      |
| **`join`**   | $O(\log n)$ (最坏)   | **O(1)**          | 配对堆的神技，也是你模板里最核心的区别       |
| **`top`**    | $O(\log n)$ 或 O(1) | **O(1)**          | 二项堆需要扫描森林中所有树的根（除非维护额外指针） |
| **`pop`**    | $O(\log n)$        | $O(\log n)$ (均摊)  | 性能瓶颈都在这里                  |
| **`modify`** | $O(\log n)$        | $O(\log n)$ (均摊)  | 配对堆实际常数极小                 |

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

## 其他

### 珂朵莉树

珂朵莉树（ODT）用于维护序列，其核心是一个$Node(l,r,val)$的结构体$set$容器,其中![image](https://cdn.nlark.com/yuque/__latex/8597b8ad64ac82614635dd0459956516.svg)是$mutable$类型使得在$set$中该值可以被修改。那么$set$会自动按照左端点升序排序，每个节点都代表一个区间，该区间内每个位置的值相同，都为对应的$val$。

其核心操作为$split$和$assign$,分裂和覆盖，在数据随机的情况下$assign$可以保证区间数量不会很多进而保证复杂度。

```cpp
template <class T>
class ODT
{

private:
    const T inf = std::numeric_limits<T>::max() / 2;
    struct Node
    {
        int l, r;      // 区间左右端点（闭区间 [l, r]）
        mutable T val; // 值（mutable 允许在 set 中修改值）

        // 用于 set 排序的运算符：按左端点 l 从小到大排序
        bool operator<(const Node &a) const
        {
            return l < a.l;
        }

        // 构造函数：默认构造一个单点区间或指定区间
        Node(int L, int R, T Val) : l(L), r(R), val(Val) {}
    };
    std::set<Node> s;                             // 有序集合存储所有区间节点，自动按左端点排序
    using IT = typename std::set<Node>::iterator; // 使用using代替#define

public:
    ODT()
    {
        // 初始化时插入一个虚拟节点作为哨兵（防止越界）
        // 哨兵节点的右边界设为极大值，确保所有合法区间都在其左侧
        s.insert(Node(0, inf, 0)); // 假设数据范围为 0~1e9，可根据实际需求调整
    }

    IT split(int pos)
    {
        /*
         * 功能：将区间在 pos 处分裂为 [l, pos-1] 和 [pos, r] 两段
         * 返回值：后半段 [pos, r] 的迭代器
         * 原理：利用 set 的有序性找到第一个左端点 >= pos 的节点，
         *       若该节点左端点等于 pos，说明已存在分裂点，直接返回；
         *       否则分裂前一个节点，生成两个新区间
         */
        IT it = s.lower_bound(Node(pos)); // 找到第一个左端点 >= pos 的节点
        if (it != s.end() && it->l == pos)
        {
            // 若存在左端点为 pos 的节点，直接返回（已分裂过）
            return it;
        }
        // 否则需要分裂前一个节点
        it--;                     // 移动到前一个节点（该节点的 l < pos <= r）
        int L = it->l, R = it->r; // 原节点的左右边界
        T Val = it->val;          // 原节点的值
        s.erase(it);              // 删除原节点
        // 插入左半段 [L, pos-1]
        s.insert(Node(L, pos - 1, Val));
        // 插入右半段 [pos, R] 并返回其迭代器
        return s.insert(Node(pos, R, Val)).first;
    }

    void assign(int l, int r, T val)
    {
        /*
         * 功能：将区间 [l, r] 内的所有值覆盖为 val
         * 步骤：
         * 1. 分裂右端点 r+1，得到后半段迭代器 itr
         * 2. 分裂左端点 l，得到前半段迭代器 itl
         * 3. 删除 [itl, itr) 区间内的所有节点（即原 [l, r] 区间的所有分段）
         * 4. 插入新节点 [l, r, val] 表示覆盖后的区间
         */
        IT itr = split(r + 1);     // 分裂出 [r+1, ...]
        IT itl = split(l);         // 分裂出 [l, ...]
        s.erase(itl, itr);         // 删除 [l, r] 区间内的所有旧分段
        s.insert(Node(l, r, val)); // 插入新的覆盖区间
    }

    void add(int l, int r, T val)
    {
        /*
         * 功能：将区间 [l, r] 内的所有值增加 val
         * 步骤：
         * 1. 分裂区间两端点，确保 [l, r] 是若干连续节点的集合
         * 2. 遍历 [itl, itr) 区间内的所有节点，逐个增加 val
         */
        IT itr = split(r + 1); // 分裂右端点
        IT itl = split(l);     // 分裂左端点
        for (; itl != itr; ++itl)
        {
            itl->val += val; // 对每个节点的值进行加法操作
        }
    }

    T kth(int l, int r, int k)
    {
        /*
         * 功能：查询区间 [l, r] 内的第 k 小值（k 从 1 开始）
         * 步骤：
         * 1. 分裂区间两端点，获取所有包含在 [l, r] 内的节点
         * 2. 收集这些节点的值和长度，按值排序
         * 3. 遍历排序后的节点，累加长度直到找到第 k 小值
         */
        std::vector<std::pair<T, int>> vp; // 存储（值，区间长度）
        IT itr = split(r + 1);             // 分裂右端点
        IT itl = split(l);                 // 分裂左端点
        for (; itl != itr; ++itl)
        {
            vp.push_back({itl->val, itl->r - itl->l + 1});
        }
        std::sort(vp.begin(), vp.end()); // 按值从小到大排序
        for (auto [val, len] : vp)
        {
            k -= len; // 扣除当前区间的长度
            if (k <= 0)
            { // 找到第 k 小值
                return val;
            }
        }
        return -1; // 若未找到（理论上不应发生）
    }

    T RangeQuery(int l, int r)
    {
        /*
         * 功能：查询区间 [l, r] 内所有值的和
         * 步骤：
         * 1. 分裂区间两端点，获取所有包含在 [l, r] 内的节点
         * 2. 遍历节点，计算每个节点的区间长度 × 值，累加求和
         */
        IT itr = split(r + 1); // 分裂右端点
        IT itl = split(l);     // 分裂左端点
        T res = 0;
        for (; itl != itr; ++itl)
        {
            res += (itl->r - itl->l + 1) * itl->val; // 区间长度 × 值
        }
        return res;
    }
};
```

### 笛卡尔树

笛卡尔树是把一个序列建立成一颗二叉搜索树，整棵树的中序遍为长度为$n$的顺序排列（即$(1,2,3,4...n)$,同时满足堆的性质，以小根堆为例,也就是$val[x]\leq val[lc[x]],val[x] \leq val[rc[x]]$

性质：笛卡尔树每个点的子树下标连续,同时子树的根是子树中值最小/最大的节点。

构建过程（小根堆笛卡尔树）：我们按元素的下标插入，设当前插入的下标为![image](https://cdn.nlark.com/yuque/__latex/2443fbcfeb7e85e1d62b6f5e4f27207e.svg),显然新插入的元素应该在树的最右边，那么我们用一个栈维护最右边的树链，通过不断弹出栈顶来从链底开始向上遍历直到找到一个![image](https://cdn.nlark.com/yuque/__latex/036441a335dd85c838f76d63a3db2363.svg)满足![image](https://cdn.nlark.com/yuque/__latex/8d306d131bed8365b2bf332a93081831.svg)，那么我们把![image](https://cdn.nlark.com/yuque/__latex/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)置为![image](https://cdn.nlark.com/yuque/__latex/036441a335dd85c838f76d63a3db2363.svg)的右儿子，把链底到![image](https://cdn.nlark.com/yuque/__latex/036441a335dd85c838f76d63a3db2363.svg)通过栈弹出的这条链作为![image](https://cdn.nlark.com/yuque/__latex/2443fbcfeb7e85e1d62b6f5e4f27207e.svg)的左儿子。

显然该过程可以保证笛卡尔树的两个性质，由于每个点只会入栈一次和出栈一次，复杂度为![image](https://cdn.nlark.com/yuque/__latex/e65a67ac353abeeff44c359310d05c02.svg)

请注意:只有在权值两两不同时建出的笛卡尔树才唯一

```cpp
class DescartesTree
{
public:
    int n, root;
    std::vector<int> lc, rc;
    DescartesTree(std::vector<int> &a) : n(a.size) - 1, lc(n + 1), rc(n + 1)
    {
        root = std::min_element(begin(a) + 1, end(a)) - begin(a);
        std::vector<int> stk(n + 1, 0);
        int top = 0;
        for (int i = 1; i <= n; ++i)
        {
            int k = top;
            while (k and a[stk[k]] > a[i])
                --k;
            if (k)
                rc[stk[k]] = i;
            if (k < top)
                lc[i] = stk[k + 1];
            stk[++k] = i;
            top = k;
        }
    }
};
```
