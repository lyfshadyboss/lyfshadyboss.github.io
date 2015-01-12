---
layout: post
title: "大数运算算法"
excerpt: ""
categories: articles
tags: Algorithm
---

### 大数乘法
![数字乘法运算过程](/images/number_multiply.png)

如图所示，数字乘法运算可以将被乘数与乘数看成是两个整数数组，分别记为 A， B，结果也是一个整数数组，记为C．将 A 的第
i 位乘以 B 的第 j 位，结果存放在 C 的第 i + j -1 位．然后对 C 进行进位操作．

当两个大数相乘时，普通的数据类型已经无法表示结果，就需要模拟数字运算过程，算法为：
{% highlight c %}
void multiply(string a, string b, vector<int>& c) {
  int i, j;

  for(i = 0; i < a.size(); i++) {
    for(j = 0; j < b.size(); j++) {
      c[i + j + 1] += (a[i] - '0') * (b[j] - '0');
    }
  }

  for(i = c.size() - 1; i >= 0; i--) {
    if (c[i] >= 10) {
      c[i - 1] += c[i] / 10;
      c[i] %= 10;
    }
  }
}
{% endhighlight %}

### 大数除法
    // TODO
### 大数加减法
    //TODO
