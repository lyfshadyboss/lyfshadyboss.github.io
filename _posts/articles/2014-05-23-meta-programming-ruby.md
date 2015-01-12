---
layout: post
title: "Ruby 元编程 - Part I [草稿]"
excerpt: "阅读 meta programming ruby 一书的笔记，整理出重要的知识，方便复习．第一部分：Ruby 的对象模型(Object Model)"
categories: articles
tags: Ruby
---

> meta programming ruby is write code that write code that write code ...!

*objects* 在 Ruby 程序中随处可见，在 Ruby 的语言构件( *language constructs* )中, *objects* 只是其中一种，
还有 *classes*, *modules*, *instance variables* 等等．Ruby 的元编程过程本质上是对这些语言构件的操作．

Ruby 的语言构件都依附于另一个东西，在 Ruby 中它叫做 *object model*．深入了解了这个核心概念后，Ruby 的一些高级
功能会看起来很自然．

### Open Classes
在 Ruby 中，`class` 关键字不只是用来定义一个类，还可以修改一个类的定义，这种用法叫 *Open Classes*．例如：

{% highlight ruby %}
def to_alphanumeric(s)
    s.gsub /[^\w\s]/, ''
end

require 'test/unit'
class ToAlphanumericTest < Test::Unit::TestCase
    def test_strips_non_alphanumeric_characters
        assert_equal '3 the Magic Number' , to_alphanumeric('#3, the *Magic, Number*?' )
    end
end
{% endhighlight %}

这个用法不符合 `OO` 的风格，可以改为：

{% highlight ruby %}
class String
    def to_alphanumeric
        gsub /[^\w\s]/, ''
    end
end

require 'test/unit'
class ToAlphanumericTest < Test::Unit::TestCase
    def test_strips_non_alphanumeric_characters
        assert_equal '3 the Magic Number' , '#3, the *Magic, Number*?'.to_alphanumeric
    end
end
{% endhighlight %}

通过关键字 `class` 重新打开了 `String` 的定义，加入了一个方法 `to_alphanumeric`，然后所有 `String` 的
实例都能使用这个方法．

使用 *Open Classes* 意味着可以随时修改一个类，包括系统提供的类，但这样做会引发问题：

1. 对修改的类造成污染
2. 可能会覆盖已经存在的方法，形成 *monkey patch*

使用 *Open Classes* 之前，先考虑：

1. 是否加入的方法足够通用
2. 有没有和已经存在的方法造成冲突

### Objects, Instance Variables and Methods

定义 `MyClass`并生成实例：

{% highlight ruby %}
class MyClass
    def my_method
        @v = 1
    end
end

obj = MyClass.new
obj.class　　# => MyClass

obj.my_method
obj.instance_variables　　# => [:@v]
{% endhighlight %}


![object, instance variables, method](/images/meta-ruby/object-01.jpg)

重点：

1. *instance variables* 在 *objects* 中， *methods* 在 *classes* 中
2. 不同于 `Java`, *class* 与 *instance variables* 是没有联系的， *instance variables* 在 *objects*
中就像 `Hash` 的键值对，所以 *classes* 的实例可以有不一样的 *instane variables*．

理解下面两句话：

{% highlight ruby %}
String.instance_methods == "abc".methods    => true
String.methods == "abc".methods             => false
{% endhighlight %}

关键在于 `String` 也是一个 *object*！
