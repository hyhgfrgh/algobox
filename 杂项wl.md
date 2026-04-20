# 杂项

## 对拍

```shell
#!/bin/bash

g++ -std=c++2a -O2 test.cpp -o test
# g++ -std=c++2a -O2 check.cpp -o check
g++ -std=c++2a -O2 std.cpp -o std
g++ -std=c++2a -O2 data.cpp -o data

// 普通对拍
for((i=1;;i++))
do
    ./data > in.txt
    ./test < in.txt > out_user.txt
    ./std  < in.txt > out_brute.txt
    if ! diff out_user.txt out_brute.txt > /dev/null
    then
        echo "发现不一致！输入如下："
        cat in.txt
        echo "test 输出："
        cat out_user.txt
        echo "std 输出："
        cat out_brute.txt
        exit
    fi
    echo "通过测试 $i"   
done
// 带check的
for((i=1;;i++))
do
    ./data > in.txt
    cat in.txt > temp.txt 
    ./test < in.txt >> temp.txt
    ./check < temp.txt > out_user.txt
    ./std  < in.txt > out_brute.txt
    if ! diff out_user.txt out_brute.txt > /dev/null
    then
        echo "发现不一致！输入如下："
        cat in.txt
        echo "test 输出："
        cat out_user.txt
        echo "std 输出："
        cat out_brute.txt
        exit
    fi
    echo "通过测试 $i"
done
```

\newpage

## int128输入输出流

```cpp
using i128 = __int128;

// 重载输入运算符以支持__int128类型
std::istream &operator>>(std::istream &is, __int128 &val) {
    std::string s;
    bool flag = true;
    is >> s, val = 0;
    if (s[0] == '-') flag = false, val = std::stoi(s.substr(0, 2)), s = s.substr(2);
    for (char &c: s) val = val * 10 + (c - '0') * (!flag ? -1 : 1);
    return is;
}

//重载输出运算符以支持__int128类型
std::ostream &operator<<(std::ostream &os, __int128 val) {
    if (val < 0) os << "-", val = -val;
    if (val > 9) os << val / 10;
    os << static_cast<char>(val % 10 + '0');
    return os;
}
```

## 随机数rng（）

```cpp
std::mt19937_64 rng(std::chrono::system_clock::now().time_since_epoch().count());
```
