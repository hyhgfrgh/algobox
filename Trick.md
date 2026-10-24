整理自www.algowiki.cn/competitions?tab=tricks

## popcount

```cpp
__builtin_popcount(x); // int
__builtin_popcountll(x); // long long
```

## 连续按位异或和

设

$$
S(n)=1\oplus 2\oplus 3\oplus \cdots \oplus n
$$

则有：

$$
S(n)=
\begin{cases}
n, & n\bmod 4=0 \\
1, & n\bmod 4=1 \\
n+1, & n\bmod 4=2 \\
0, & n\bmod 4=3
\end{cases}
$$

## 计算n!的末尾的0的个数

公式如下：
$$ans = \lfloor \frac{n}{5} \rfloor + \lfloor \frac{n}{25} \rfloor + \lfloor \frac{n}{125} \rfloor + ...$$

计算 $n!$ 的末尾 $0$ 的个数，实际上就是计算 $1 * 2 * 3 * ... * n - 1 * n$ 中因子 $10$ 的个数

$10 = 2 * 5$ ，且在 $n!$ 中，因子 $2$ 的个数远远多于因子 $5$ 的个数 ，所以只需要统计 $n!$ 中所有因子 $5$ 的个数即可

已知 $1$ 到 $n$ 中 $5$ 的倍数的个数为 $\lfloor \frac{n}{5} \rfloor$，但如果直接将 $\lfloor \frac{n}{5} \rfloor$ 当作答案会漏掉 $5$ 的个数
因为 $25 = 5 * 5$ 贡献了 $2$ 个 $5$ , $125 = 5 * 5 * 5$ 贡献了 $3$ 个 $5$ ...

可以通过重复计数来补上这些漏掉的 $5$
- $\lfloor \frac{n}{5} \rfloor$ 统计所有 $5$ 的倍数贡献的第一个 $5$
- $\lfloor \frac{n}{25} \rfloor$ 统计所有 $25$ 的倍数贡献的第二个 $5$
- $\lfloor \frac{n}{125} \rfloor$ 统计所有 $125$ 的倍数贡献的第三个 $5$
- ......

这样一来就可以完整地统计出 $n!$ 中所有因子 $5$ 的个数了

## 圆周率值

```cpp
using f64 = long double;
const f64 pi = acosl(-1);
```

## 平方和公式展开恒等式

基于**平方和**与**和平方**之间的恒等式
$$
(\sum_{i=1}^ns_i)^2=\sum_{i=1}^ns_i^2+\sum_{i\neq j}s_is_j
$$

容易发现：
$$
\sum_{i\neq j}s_is_j=2\sum_{1\le i<j\le n}s_is_j
$$

所以有：

$$
\sum_{1 \le i < j \le n} s_i s_j = \frac{(\sum_{i=1}^n s_i)^2 - \sum_{i=1}^n s_i^2}{2}
$$
应用：数组两两之间的乘积和（$\cal O(n^2)$ 降为 $\cal O(n)$）

普通求$\sum_{1 \le i < j \le n} s_i s_j$直接利用前缀和就可以做到${\cal O}(n)$

但是如果需要**区间查询+带修**就可以用这个恒等式用线段树来简单实现

线段树维护**区间查询+带修** 也可以不使用这个恒等式，每个节点维护：

- `sum`：区间元素和。
- `ans`：区间两两乘积之和。

合并左右儿子：

 \[ \begin{aligned} sum &= sum_L+sum_R\\ ans &= ans_L+ans_R+sum_L\cdot sum_R \end{aligned} \]

(当然利用前缀和ddp也可以，不过会增加代码实现难度)

## 无序(/指定顺序)集合哈希


判断两个集合是否相同，使用满足交换律的运算$\sum rng(a[i])$ , $\prod$  ,$xor$ ....等操作哈希一下就行(注意避免哈希冲突)

**区间查询实现** 

- 不带修，使用前缀和维护，单次查询时间复杂度：${\cal O}(1)$ 。
- 带修，使用数据结构（如树状数组/线段树）维护区间和，单次查询时间复杂度：${\cal O}(\log n)$ 。

**扩展问题**（指数哈希）

给定数组 $a$，多次询问：判断区间 $[L_1,R_1]$ 中所有元素加上 $k$ 后，是否与区间 $[L_2,R_2]$ 的可重集合相等。

设常数 $B$，定义哈希函数：$H(S)=\sum B^{a_i}$

则有：$H([L,R])=\sum_{i=L}^{R}B^{a_i}$ 

区间整体加上 $k$ 后：$H'([L,R])=\sum_{i=L}^{R}B^{a_i+k}=B^k\cdot\sum_{i=L}^{R}B^{a_i}=B^k\cdot H([L,R])$

所以仅需要判断 $B^k\cdot H([L_1,R_1])$ 与 $H([L_2,R_2])$ 的关系即可。

## 排列中 mex 与 min 的转化

对于一个排列，前缀 mex 的询问，可以转换为后缀最小值：
$$
\operatorname{mex}_{i = 1}^ja_i = \min_{i = j + 1}^n \limits{a_i}
$$
## 约数个数奇偶性

一个数的约数个数是**奇数**等价于这个数是**完全平方数**。



## Legendre 公式

记 \(v_2(x)\) 为 \(x\) 中质因数 \(2\) 的指数,那么 \[ v_2(n!)=\sum_{k\ge1}\left\lfloor\frac n{2^k}\right\rfloor=n-\operatorname{popcount}(n) \]

**推导 ：**

设 \(n\) 的二进制表示为：

\[ n=\sum_{i=0}^{k}b_i2^i,\quad b_i\in\{0,1\} \]

关键是把每一项的 \(n\) 展开。

\[ \begin{aligned} \sum_{j\ge1}\left\lfloor\frac{n}{2^j}\right\rfloor &=\sum_{j\ge1}\sum_{i\ge j}b_i2^{i-j}\\ &=\sum_{i\ge1}b_i\sum_{j=1}^{i}2^{i-j}\\ &=\sum_{i\ge1}b_i(2^i-1)\\ &=\sum_{i\ge1}b_i2^i-\sum_{i\ge1}b_i\\ &=n-\operatorname{popcount}(n) \end{aligned} \]



## O(1)判断组合数奇偶性

$\binom{n}{m}$为奇数$\Leftrightarrow(m\& (n-m))=0$

上面的方法可以${\cal O}(1)$判断,如果使用lucas定理的话可以${\cal O}(\log)$复杂度求

**推导:**

因为$\binom{n}{m} = \frac{\prod_{m+1}^{n} i}{\prod_{1}^{m} i}$ ,那么$\binom{n}{m}$是不是奇数就是看$\prod_{m+1}^{n} i$的质因数2有没有被${\prod_{1}^{m} i}$ 消尽

记 \(v_2(x)\) 为 \(x\) 中质因数 \(2\) 的指(个)数，那么

\[ v_2\binom nm=v_2(n!)-v_2(m!)-v_2((n-m)!) \]

根据 Legendre 公式：

\[ v_2(n!)=\sum_{k\ge1}\left\lfloor\frac n{2^k}\right\rfloor=n-\operatorname{popcount}(n) \]

其中 \(\operatorname{popcount}(n)\) 表示 \(n\) 的二进制中 \(1\) 的个数。代入可得：

\[ \boxed{ v_2\binom nm= \operatorname{popcount}(m)+ \operatorname{popcount}(n-m)- \operatorname{popcount}(n) } \]

$v_2\binom nm == 0$即
$$
\operatorname{popcount}(m)+ \operatorname{popcount}(n-m) =\operatorname{popcount}(n)
$$
 \(m+(n-m)=n\),两个二进制数相加，只有不发生进位时，二进制中 \(1\) 的总数才不会减少。

也就是n和n-m两个数的二进制中，没有同一位同时为 \(1\) ,故$(m\& (n-m))=0$ 

## 图上重轻点根号分治

设阈值 \(B=\sqrt m\)。

- 轻点：\(\deg(u)\le B\)。邻居最多 \(O(\sqrt m)\)，可以暴力枚举。
- 重点：\(\deg(u)>B\)。因为所有点的度数和为 \(2m\)，重点最多 \(O(\sqrt m)\) 个，可以针对重点预处理或维护信息。

在具有 $m$ 条边的图中，最多只有 $O(\sqrt{m})$ 种度数不同的结点。
