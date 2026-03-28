\newpage

# 字符串

## 字符串哈希

```cpp
using i64 = long long;
std::vector<i64> M = {1610612741, 0, 805306457, 402653189, 201326611, 100663319, 50331653, 1000000007, 1000000009};

struct HASH {
    i64 h1 = 0, h2 = 0;

    HASH operator-(const HASH &x) const { return {h1 - x.h1, h2 - x.h2}; }

    HASH operator+(const HASH &x) const { return {h1 + x.h1, h2 + x.h2}; }

    HASH operator+(const std::pair<int, int> &x) const { return {h1 + x.first, h2 + x.second}; }

    HASH operator%(const std::pair<int, int> &x) const { return {h1 % x.first, h2 % x.second}; }

    bool operator==(const HASH &x) const { return h1 == x.h1 && h2 == x.h2; }

    bool operator<(const HASH &x) const { return h1 < x.h1 || (h1 == x.h1 && h2 < x.h2); }

    bool operator>(const HASH &x) const { return h1 > x.h1 || (h1 == x.h1 && h2 > x.h2); }
};

template<class T>
struct Hash {
    int n{};
    std::vector<i64> P1, P2;
    std::vector<HASH> H;
    i64 p1 = 131, p2 = 13331;

    Hash() {};

    Hash(std::string &s) { Init(s); }

    Hash(std::vector<T> &s) { Init(s); }
    // 改变模数
    void change() {
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        while (M[1] == 0 || M[2] == 0) shuffle(M.begin(), M.end(), rng);
    }
    // 处理哈希
    void deal(int st, int len, std::vector<T> &s) {
        P1.resize(len + 1);
        P2.resize(len + 1);
        H.resize(len + 1);
        P1[0] = 1;
        P2[0] = 1;
        for (int i = st; i <= len; i++) {
            P1[i] = (P1[i - 1] * p1) % M[1];
            P2[i] = (P2[i - 1] * p2) % M[2];
            H[i].h1 = (H[i - 1].h1 * p1 + s[i - st + 1]) % M[1];
            H[i].h2 = (H[i - 1].h2 * p2 + s[i - st + 1]) % M[2];
        }
    }

    void Init(std::string &s) {
        std::vector<char> str(1, ' ');
        for (char &c: s)str.push_back(c);
        Init(str);
    }

    void Init(std::vector<T> &s) {
        if (M[1] == 0) change();
        n = s.size() - 1;
        deal(1, n, s);
    }
    // 获取子串串l到r的哈希值
    HASH get(int l, int r) {
        HASH res;
        res.h1 = ((H[r].h1 - H[l - 1].h1 * P1[r - l + 1]) % M[1] + M[1]) % M[1];
        res.h2 = ((H[r].h2 - H[l - 1].h2 * P2[r - l + 1]) % M[2] + M[2]) % M[2];
        return res;
    }
    
    HASH mul(int l1, int r1, int l2, int r2) {
        HASH it1 = get(l1, r1), it2 = get(l2, r2), it3;
        it1.h1 *= P1[r2 - l2 + 1] %= M[1];
        it1.h2 *= P2[r2 - l2 + 1] %= M[2];
        it3 = (it1 + it2) % getMod();
        return it3;
    }

    HASH mul(HASH l, HASH r, int len) {
        HASH res;
        l.h1 *= P1[len] %= M[1];
        l.h2 *= P2[len] %= M[2];
        res = (l + r) % getMod();
        return res;
    }

    void push_back(std::string s) { push_back(std::vector<char>(s.begin(), s.end())); }

    void push_back(std::vector<T> s) { deal(n + 1, s.size() + n, s), n += s.size(); }

    std::pair<i64, i64> getMod() { return std::make_pair(M[1], M[2]); }
};
```

\newpage

## 字符串哈希（二维）

```cpp
//改成ull会快很多
using i64 = long long;
std::vector<i64> M = {1610612741, 0, 805306457, 402653189, 201326611, 100663319, 50331653, 1000000007, 1000000009};

template<class T>
struct Hash {
    int n{}, m{};
    std::vector<i64> P1, P2;
    std::vector<std::vector<i64>> H;
    i64 p1 = 131, p2 = 13331;

    Hash() {};

    Hash(std::vector<std::vector<T>> &s) { Init(s); }

    void change() {
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        while (M[1] == 0) shuffle(M.begin(), M.end(), rng);
    }

    void Init(std::vector<std::vector<T>> &s) {
        if (M[1] == 0) change();
        n = s.size(), m = s[0].size();
        P1.resize(n + 1);
        P2.resize(m + 1);
        H.resize(n + 1);
        P1[0] = 1;
        P2[0] = 1;
        H[0].assign(m + 1, {});
        for (int i = 1; i <= n; i++) P1[i] = P1[i - 1] * p1 % M[1];
        for (int i = 1; i <= m; i++) P2[i] = P2[i - 1] * p2 % M[1];

        for (int i = 1; i <= n; i++) {
            H[i].assign(m + 1, {});
            for (int j = 1; j <= m; j++) {
                H[i][j] = (((((H[i - 1][j] * p1) % M[1]) + ((H[i][j - 1] * p2) % M[1])) % M[1] + s[i - 1][j - 1] -
                            ((H[i - 1][j - 1] * p1 % M[1]) * p2) % M[1]) % M[1] + M[1]) % M[1];

            }
        }
    }

    i64 get(int x1, int y1, int x2, int y2) {
        return ((H[x2][y2] - (H[x1 - 1][y2] * P1[x2 - x1 + 1] % M[1]) - (H[x2][y1 - 1] * P2[y2 - y1 + 1] % M[1])
                 + (((H[x1 - 1][y1 - 1] * P1[x2 - x1 + 1] % M[1]) * P2[y2 - y1 + 1] % M[1]) % M[1])) % M[1] + M[1]) %
               M[1];
    }
};
```

\newpage

## 最小表示法

返回一个字符串字典序最小的循环同构串。

```cpp
// 字符串可以先转化为元素类型为char的vector数组，
std::vector<int> minimalString(std::vector<int>& a) {
    int n = a.size();
    int i = 0, j = 1, k = 0;
    while (k < n and i < n and j < n) {
        if (a[(i + k) % n] == a[(j + k) % n]) {
            k++;
        } else {
            (a[(i + k) % n] > a[(j + k) % n] ? i : j) += k + 1;
            i += (i == j);
            k = 0;
        }
    }
    k = std::min(i, j);
    std::vector<int> ans(n);
    for (int i = 0; i < n; i++) { ans[i] = a[(i + k) % n]; }
    return ans;
}
// 直接返回字典序最小循环同构串
```

## 最小循环节

判断一个串是否有长度为$l$的循环节，我们只需要判断$[l,r-d]$和$[l+d,r]$处的串是否相同即可，其中$l$表示左端点，$r$表示右端点。

如果要找到最小的循环节，我们考虑到答案一定是$len$的一个因数，并且由唯一分解定理一定可以表示为 $p_1^{a_1}*p_2^{a_2}...$ ，那么我们对$len$不断除去它的最小质因数（用欧拉筛处理），同时判断 $ans$ 除去该质因数是否满足即可，在实现哈希函数后，复杂度为$O(\sum a)$，即所有质因数的幂之和

```cpp
// minPrimeFactor()是通过欧拉筛sieve获取的最小质因数minp
int findMinimalCycle(std::string s)
{
    int len = s.length();
    s = ' ' + s;
    int l = 1, r = len;
    StringHash hash(s, 13331, 998244353);
    int ans = len;
    while (len != 1)
    {
        int d = ans / minPrimeFactor(len);
        if (hash(l, r - d) == hash(l + d, r))
        {
            ans = d;
        }
        len /= minPrimeFactor(len);
    }
    return ans;
}
```

\newpage

## KMP

 其中 $next[i]$  表示字符串以 $i$ 结尾的 和 $s$ 前缀相同的长度小于 $i$ 的最长长度

定义:

+ 定义一个字符串 s 的 border 为 s 的一个**非 s 本身**的子串 t，满足 t 既是 s 的前缀，又是 s 的后缀。  

+ next数组即为每个前缀 s′ 的最长 border t′ 的长度。

```cpp
// 传入的字符串0-base,返回的数组1-base
std::vector<int> kmp(std::string &s) {
    int n = s.size();
    std::vector<int> next(n + 1);
    for (int i = 1, j = 0; i < n; i++) {
        while (j && s[i] != s[j]) j = next[j];
        if (s[i] == s[j]) j++;
        next[i + 1] = j;
    }
    return next;
}

// 在s中找t的位置以及求t的border
void solve(){
    string s,t;cin>>s>>t;
    vector<int> next = kmp(t);
    for(int i = 0,j = 0;i<s.size();i++){
        while(j && s[i] != t[j]) j = next[j];
        if(s[i] == t[j]) j++;
        if(j == t.size()) {
            cout<<i-j+1+1<<"\n"; // 0-base位置+1,看情况
            j = next[j];
        }
    }
    for(int i = 1;i<=t.size();i++){
        cout<<next[i]<<" ";
    }
    cout<<"\n";
}
```

\newpage

## Z函数(扩展KMP)

其中 $z[i]$ 数组表示 $s$ 和 $s$ 以 $i$ 开头的后缀$s.substr(i)$ 的最长公共长度LCP

```cpp
// 传入的字符串 0-base,返回的数组1-base
std::vector<int> Zfunc(const std::string &s) {
    int n = s.size();
    std::vector<int> z(n + 1);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r) z[i] = std::min(z[i - l], r - i + 1);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    for (int i = n; i >= 1; i--) z[i] = z[i - 1];
    return z;
}
```

**例题：**

给定两个字符串 $a,b$，你要求出两个数组：

- $b$ 的 $z$ 函数数组 $z$，即 $b$ 与 $b$ 的每一个后缀的 LCP 长度。
- $b$ 与 $a$ 的每一个后缀的 LCP 长度数组 $p$。

对于一个长度为 $n$ 的数组 $a$，设其权值为 ${xor}_{i=1}^n i \times (a_i + 1)$。

```cpp
void solve1(){
    string a,b;cin>>a>>b;
    int la = a.size(),lb = b.size();
    b += "$"+a;
    vector<int> z = Zfunc(b);
    int ans1 = lb+1,ans2 = 0;
    for(int i = 2;i<=lb;i++) ans1 ^= i*(z[i]+1);
    for(int i = lb+2,j = 1;i<=la+lb+1;i++,j++) ans2 ^= j*(z[i]+1);
    cout<<ans1<<"\n"<<ans2<<"\n";
}
void solve2(){
    string a,b;cin>>a>>b;
    vector<int> z = Zfunc(b);
    int la = a.size(),lb = b.size();
    vector<int> p(la+1);
    for(int i = 0,l = 0,r = -1;i<la;i++){
        // z[i-l+1]中加一是因为z整体右移了一位
        if(i<=r) p[i] = min(z[i-l+1],r-i+1);
        while(p[i]<lb and i+p[i]<la and b[p[i]] == a[i+p[i]]) p[i]++;
        if(i+p[i]-1>r) l = i,r = i+p[i]-1;
    }
    for(int i = la;i>=1;i--) p[i] = p[i-1];
    int ans1 = 0,ans2 = 0;
    for(int i = 1;i<=b.size();i++) ans1 ^= i*(z[i]+1);
    for(int i = 1;i<=a.size();i++) ans2 ^= i*(p[i]+1);
    cout<<ans1<<"\n"<<ans2<<"\n";
}
```

\newpage

## Manacher

其中 $r[i]$ 以位置 $i$ 为中心的最长回文半径,例如对于#b#a#b#，$i$ 为a的位置，此时 $r[i] = 4$  ,对于#a#a#，$i$ 是中间那个#的位置,此时$r[i] = 3$ 

字符 $s$ 中最大的回文串长度为$max\{r[i]-1\}$ 

```cpp
// s 为 0-base
std::vector<int> manacher(std::string s) {
    std::string t = " #";
    for (auto c: s) {
        t += c;
        t += '#';
    }
    int n = t.size();
    std::vector<int> r(n);
    for (int i = 0, j = 0; i < n; i++) {
        if (2 * j - i >= 0 && j + r[j] > i) {
            r[i] = std::min(r[2 * j - i], j + r[j] - i);
        }
        while (i - r[i] >= 0 && i + r[i] < n && t[i - r[i]] == t[i + r[i]]) {
            r[i] += 1;
        }
        if (i + r[i] > j + r[j]) {
            j = i;
        }
    }
    return r;
}
// 使用manacher构造r数组时传入的s为0-base
// 调用时 l, r  为1-base
bool isPalindrome(std::vector<int> &R, int l, int r) {
    int p = (l + (r - l + 1) / 2) * 2;
    if ((r - l + 1) % 2 == 0) p--;
    return R[p] >= (r - l + 1);
}
```
