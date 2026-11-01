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
