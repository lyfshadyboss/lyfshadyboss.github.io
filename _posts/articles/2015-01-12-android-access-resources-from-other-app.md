---
layout: post
title: "在 Android 应用中使用其他 App 的资源"
excerpt: "在不设置相同 shareUserId 的情况下，也可以加载其他 app 的资源(image, string, layout, code ...)"
categories: articles
tags: Android
---

### 有关 sharedUserId

> android:sharedUserId
>
>  The name of a Linux user ID that will be shared with other applications. By default, Android assigns each application
>  its own unique user ID. However, if this attribute is set to the same value for two or more applications, they will 
>  all share the same ID — provided that they are also signed by the same certificate. Application with the same user ID
>  can access each other's data and, if desired, run in the same process.

如果多个应用有相同的 `sharedUserId` (而且有相同的签名)，它们可以互访问数据 (`/data/data/xxx`)，也 *可以* 设置成同一进程。这对于同一系列的
应用来说，数据交换会变得方便许多。否则，应用的数据 `/data/data/xxx` 是相互隔离的，这也合理。


### 没有相同的 sharedUserId 情况下互访资源

虽然没有相同的 `sharedUserId` 不能互访数据，不过可不可以互访资源（比如 string, image, layout ...）呢？ 实际上是可以的，实验证明：
[resource access demo](https://github.com/lyfshadyboss/android-demo-playground/tree/master/access-external-resources)

两个应用 : App1, App2, 如果在 App2 中访问 App1 的资源，做法如下：

{% highlight java %}
String externalAppName = "";
View externalLayout = null;

try {
    Context externalCxt = this.createPackageContext("com.demo.liuyifeng.app1",
            Context.CONTEXT_IGNORE_SECURITY | Context.CONTEXT_INCLUDE_CODE);
    externalCxt.setTheme(R.style.AppTheme);

    Resources res = externalCxt.getResources();
    int appname_id = res.getIdentifier("app_name", "string", externalCxt.getPackageName());
    externalAppName = res.getString(appname_id);

    int content_id = res.getIdentifier("content", "layout", externalCxt.getPackageName());
    LayoutInflater inflater = (LayoutInflater) externalCxt.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    externalLayout = inflater.inflate(content_id, null);
} catch (Exception e) {
    e.printStackTrace();
}
{% endhighlight %}
    
除了可以取得 string 和 layout 外，如果 App1 的 layout 包含了 App1 的源码，也是可行的，只需加上 `Context.CONTEXT_IGNORE_SECURITY 
| Context.CONTEXT_INCLUDE_CODE` 。


### 意义
1. 对于同一系列的应用，使用统一的 `sharedUserId` 可以方便应用之间的沟通
2. 对于没有相同 `sharedUserId` 的应用，似乎访问资源没什么意义
3. 设计 `launcher` 应用的主题机制时，可以利用它来访问主题资源，不过既然是同一系列，当然可以设置同一 `sharedUserId` 了。不过主题包一般都不
需要安装，大多采用解析压缩包的方法
4. 综上，这篇文字似乎也没什么意义了...
