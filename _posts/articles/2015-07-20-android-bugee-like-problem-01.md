---
layout: post
title: "Android 疑难杂症分析 01 - Layout Animation"
excerpt: ""
categories: articles
tags: [Android, MAT]
---

# 问题概述

Android 在 API Level 11 (3.0) 中引入了一个动画机制 : *Layout Animation* . 参考：

1. [animating layout changes](http://developer.android.com/training/animation/layout.html)
2. [layout transition](http://developer.android.com/reference/android/animation/LayoutTransition.html)

这个机制可以很方便的在 layout 变化(添加 view, 删除 view, 子 view 的 visibility 发生改变)时引入动画，称为 *Layout Transition*. 使用方法：

{% highlight xml %}
<LinearLayout android:id="@+id/container"
    android:animateLayoutChanges="true"
    ...
/>
{% endhighlight %}
    
或者

{% highlight java %}
...
viewGroup.setLayoutTransition(new LayoutTransition());
...
{% endhighlight %}

默认的效果如下，不过用户可以通过自定义 Layout Transition 类来控制动画效果：

<div> <video autoplay loop="true" src="/images/android/android-anim-layout-changes.mp4" /> </div>

## 遇到的问题
*Layout Transition* 的机制一部分实现在 *ViewGroup* 里，但是存在 bug. 也就是这次遇到的问题.

现在为了实现这样一种效果:

1. Activity A 中有一个 ListView，点击 ListView 的某一个 Item 后，其他的 Item 做动画消散，动画完成后，开启 Activity B
2. Activity B 回到 Activity A 时，如果 A 中 ListView 不需要刷新，则保持原状
3. Activity B 回到 Activity A 时，如果 A 中 ListView 需要刷新，则重新刷新数据

demo :

![demo](/images/android/android-listview-anim-demo.gif)

*ListView* 点击事件的定义 :

{% highlight java %}
listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        exitTransition(parent, position, 500);
        postDelayed(new Runnable() {
            @Override
            public void run() {
                Intent intent = new Intent(getContext(), Activity2.class);
                getContext().startActivity(intent);
            }
        }, 500);
    }
});
{% endhighlight %}

exitTransition 对其他的 item 做了动画效果 :
{% highlight java %}
private void exitTransition(AdapterView<?> adapterView, int position, int duration) {
    int firstPosition = adapterView.getFirstVisiblePosition();
    int endPosition = adapterView.getLastVisiblePosition();

    for (int i = firstPosition; i <= endPosition; i++) {
        int index = i - firstPosition;
        View itemView = adapterView.getChildAt(index);
        if (i < position) {
            AnimationUtil.getInstance().hideViewWithAlpha(itemView, AnimationUtil.DOWN_TO_TOP, duration, 200, true);
            transitionViewList.add(itemView);
        } else if (i > position) {
            AnimationUtil.getInstance().hideViewWithAlpha(itemView, AnimationUtil.TOP_TO_DOWN, duration, 200, true);
            transitionViewList.add(itemView);
        }
    }
}
{% endhighlight %}

这里用一个 *List* 保存了做动画的 items，且各个 item 的动画都有 *fillAfter* 属性，所以在 Acitvity A *onResume* 时需要清除 item 的动画 :
{% highlight java %}
protected void onResume() {
    for (View view : transitionViewList) {
        view.clearAnimation();
    }
    transitionViewList.clear();
}
{% endhighlight %}

现在需要的动画效果都属于正常，但是如果 A 中的 ListView 需要刷新数据，在 onResume 中需要加入逻辑后 :

{% highlight java %}
protected void onResume() {
    // 这里为了简便，直接掉用 setAdapter 达到刷新数据的目的
    listView.setAdapter(adapter);
    ...
}
{% endhighlight %}

得到的效果出乎意料的不好，有许多残留的 view 显示在 *ListView* 中 :

![demo](/images/android/android-listview-anim-demo-broken.gif)

WTF!!!!!

# 分析过程
针对最终产生的结果，首先提出几点疑惑：

1. 残留的 view 的 parent 是谁 ?
2. 谁负责绘画了残留的 views ?
3. 如何解决残留问题，且不影响其他功能 ?

能回答以上三个问题，问题就可以得到解决，以下将对这些疑惑作解答.

首先，通过 demo 验证，有这么几个基本事实：

1. 注释掉 listView.setAdapter(adapter) 这句话，也就是不刷新数据，正常
2. 注释掉 clearAnimation() 这句话，也就是不清除动画，正常 
3. 将 clearAnimation() 移到 listView.setAdapter(adapter) 之前，正常

基于这几个基本事实，想从代码直接分析出原因似乎很困难，的确如此，完全无法理解！可以先从 SDK 源码寻找线索.

## setAdapter 的副作用

平常更新 *ListView* 的数据都是用 adapter 的 *notifyDataChanged* 或者 *notifyDataInvalid* 方法，这里为何用 setAdapter ?

看 setAdapter 的源码：

{% highlight java %}
public void setAdapter(ListAdapter adapter) {
    ...
    resetList();
    mRecycler.clear();
    ...
    requestLayout();
}
{% endhighlight %}

主要原因是为了不仅让 *ListView* 刷新数据，而且在 adapter 的 *getView(int position, View convertView, ViewGroup parent)* 中 convertView
为空，让每个 item view 都重新生成，这样做和另一个动画需求有关。convertView 的回收利用是通过 *mRecycler* 处理的，而且 *ListView* 没有提供
接口操作 *mRecycler*，唯一的方法就是掉 setAdapter，让 *mRecycler* 清空.

但不管是用 setAdapter 还是其他方法，只要 *ListView* 中的 items 重新生成，都会引发上面奇怪的现象.

## clearAnimation 的副作用

clearAnimation 用于将 transitionViewList 中的 view 还原成最初的状态，否则他们都将保持不可见 (alpha 为 0). 源码：

{% highlight java %}
public void clearAnimation() {
    if (mCurrentAnimation != null) {
        mCurrentAnimation.detach();
    }
    mCurrentAnimation = null;
    invalidateParentIfNeeded();
}
{% endhighlight %}

完全无害，不会对 *ListView* 造成影响.

## MAT 静态分析
从代码或者 SDK 源码看不出问题的根源，不过还有最后一招：*MAT 内存 dump*. 

# 结论
