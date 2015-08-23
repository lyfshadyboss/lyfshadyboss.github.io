---
layout: post
title: "Haskell 中 foldl 和 foldr 的实例推导"
excerpt: ""
categories: articles
tags: [Haskell]
---

虽然 *foldl* 和 *foldr* 是 *Haskell* 中最基本的一种操作，但实际使用中效率低下，转而使用 *foldl'* 和 *foldr'*，
这里只是纪录一些备忘内容。

# foldl

基本实现：
{% highlight haskell %}
foldl :: (a -> b -> a) -> a -> [b] -> a
foldl step zero (x:xs) = foldl step (step zero x) xs
foldl _    zero []     = zero
{% endhighlight %}

推导实例：
{% highlight haskell %}
foldl (+) 0 (1:2:3:[])
          == foldl (+) (0 + 1)             (2:3:[])
          == foldl (+) ((0 + 1) + 2)       (3:[])
          == foldl (+) (((0 + 1) + 2) + 3) []
          ==           (((0 + 1) + 2) + 3)
{% endhighlight %}

# foldr

基本实现：
{% highlight haskell %}
foldr :: (a -> b -> b) -> b -> [a] -> b
foldr step zero (x:xs) = step x (foldr step zero xs)
foldr _    zero []     = zero
{% endhighlight %}

推导实例：
{% highlight haskell %}
foldr (+) 0 (1:2:3:[])
          == 1 +           foldr (+) 0 (2:3:[])
          == 1 + (2 +      foldr (+) 0 (3:[])
          == 1 + (2 + (3 + foldr (+) 0 []))
          == 1 + (2 + (3 + 0))
{% endhighlight %}

# 用 foldr 来实现 foldl
*foldl* 也可以用 *foldr* 来表示 :

{% highlight haskell %}
myFoldl :: (a -> b -> a) -> a -> [b] -> a
myFoldl f z xs = foldr step id xs z
    where step x g a = g (f a x)
{% endhighlight %}

推导实例：
{% highlight haskell %}
myFoldl (+) 0 (1:2:3:[])
          == foldr step1 id (1:2:3:[]) 0
          == step1 1 (foldr step2 id (2:3:[])) 0
          == step1 1 (step2 2 (foldr step3 id (3:[]))) 0
          == step1 1 (step2 2 (step3 3 (foldr step4 id []))) 0
          == step1 1 (step2 2 (step3 3 id)) 0
          == step2 2 (step3 3 id) (+ 0 1)
          == step3 3 id (+ (+ 0 1) 2)
          == id (+ (+ (+ 0 1) 2) 3)
          == (+ (+ (+ 0 1) 2) 3)
{% endhighlight %}
