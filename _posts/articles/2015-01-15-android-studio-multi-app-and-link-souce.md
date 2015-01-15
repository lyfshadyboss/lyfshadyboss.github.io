---
layout: post
title: "在 Android Studio 中管理多个 module， 以及 module 之间 link source"
excerpt: ""
categories: articles
tags: [Android Studio]
---

### Android Project 和 Module
在 Android Studio 中， `project` 和 `module` 的概念和 Eclipse 有所不同：

> module in Android Studio is like a project in Eclipse,
  project in Android Studio is like a workspace in Eclipse
  
也就是说，在一个 Android Studio 工程里，是可以有多个应用的，这些应用的表现形式是 `module`, `module` 除了可以作为 `application`, 也可以是 
`library`，至于如何指定，通过设置界面和 `gradle` 配置都可以完成。新建一个工程后，默认包含一个名字为 "app" 的 `application module`。

#### 新建 module
`File -> New Module`,  选择 module 的类型
  ![new module](/images/android-studio-new-module.png)
  
#### 修改已有的 module 类型
  修改 `build.gradle` :
  
* application module

    {% highlight  groovy %}
    apply plugin: 'com.android.application'
    {% endhighlight %}
  
* library module

    {% highlight  groovy %}
    apply plugin: 'com.android.library'
    {% endhighlight %}

### module 之间 link source
Eclipse 里，可以很方便的建立一个 `source link` 将工程外的源码目录加入编译，在 Android Studio 中想要完成类似的任务呢？ 比如现有 3 个
`module` :

1. engine, 公共模块
2. demo, 基于 engine 的一个示例
3. app, 基于 engine 的一个应用

为了最大限度的复用代码，可以有不同的方案

#### 方案一
将 engine 作为 `library module`, demo 和 app 作为 `application module` 并依赖于 engine module。添加一个 `module` 作为依赖的方法是在 `module 
setting` 里操作：

![add dependency](/images/android-studio-add-dependency.png)

作为依赖的 module 只能以 `library module` 的类型存在, 所以 engine 不能作为一个单独的应用运行, 但可以创建 activity 后在 app 或 demo 
的 `AndroidManifest.xml` 里引用后运行（作为 '宿主' 的一部分）。

#### 方案二
如果想 engine 也作为一个独立的应用运行，比如跑一些测试等等，需要将 engine 作为一个 `application module`, 但不能作为其他模块的 `dependency`,
不能共享代码。

可以修改其他 module 的 `build.gradle` 将 engine 的 src 加入编译过程，达到共享的目的。

{% highlight groovy %}
android {
    ...

    sourceSets {
        main.java.srcDirs += 'src/../../engine/src/main/java/'
    }
    
    ...
}
{% endhighlight %}
    
各个 `application module` 之间都可以使用这种方式共享。
    
