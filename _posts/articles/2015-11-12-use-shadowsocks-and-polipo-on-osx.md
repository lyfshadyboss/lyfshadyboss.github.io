---
layout: post
title: "Shadowsocks 和 polipo 搭配使用 [OS X]"
excerpt: ""
categories: articles
tags: [Shadowsocks]
---

用 [Shadowsocks](www.shadowsocks.com) 可以方便的翻墙，但终端使用 npm 等工具时还是不能下载受限内容，虽然可以用 VPN 来实现全局自由，不过
配合 polipo 就可以利用 Shadowsocks 服务了。


# 安装 polipo

{% highlight bash %}
brew install polipo
{% endhighlight %}

# 开启 polipo

{% highlight bash %}
polipo socksParentProxy=localhost:1080

//默认代理地址为 http://localhost:8123, 测试一下
http_proxy=http://localhost:8123 curl www.google.com
{% endhighlight %}

# 设置 http_proxy, https_proxy

{% highlight bash %}
npm config set proxy http://localhost:8123
npm config set https_proxy http://localhost:8123
{% endhighlight %}

DONE!
