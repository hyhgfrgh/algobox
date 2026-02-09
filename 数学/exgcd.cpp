// 设当前递归层为 a,b,x',y'
// 在该层内完成exgcd后返回的x,y满足 b*y + (a%b)*x = gcd
// 换一下形式就是 b*y + (a-a/b*b)*x = gcd
//              a*x + b*(y-a/b*x) = gcd
// 令 x' = x,y' = y-a/b*x 
// 即当前层的x不变，y-=a/b*x即可得到 a*x‘+b*y’ = gcd

long long exgcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long d = exgcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
long long getInv(long long a, long long mod) {
    long long x, y;
    long long d = exgcd(a, mod, x, y);
    if (d != 1) return -1;
    return (x % mod + mod) % mod;
}
