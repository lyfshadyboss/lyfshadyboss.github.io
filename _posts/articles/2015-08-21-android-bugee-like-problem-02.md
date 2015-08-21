---
layout: post
title: "Android 疑难杂症分析 02 - Color Drawable"
excerpt: ""
categories: articles
tags: [Android]
---

# 问题概述

这是一个与 *Android* 版本有关的问题，存在于 *Android 5.0* 以上的系统中，原因是 *Android 5.0* 对 *Color Drawable* 资源文件做了一些优化，
与我们在项目中的一些使用场景发生了冲突。

现象是：在 5.0 以上系统上，应用的一些背景颜色异常消失了，而在 5.0 以下系统则完全正常。而且问题的影响是全局的，一个模块发生问题，另一个毫无
关联的模块也出现异常(他们唯一的共同点就是使用的背景颜色相同)。

为了简化问题，以一个简单的 demo 为例说明 ：
定义一个 *Color* 资源 :
{% highlight xml%}
<resources>
    <color name="white">#ffffffff</color>
</resources>
{% endhighlight %}

在 layout 中使用刚才定义的资源，有两个相同大小的 *ImageView*，image1 和 image2，image1 无背景色，image2 使用 *white* 
作为背景色:
{% highlight xml%}
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@android:color/black"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity">

    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="match_parent"
        android:orientation="vertical">

        <ImageView
            android:id="@+id/image1"
            android:layout_width="match_parent"
            android:layout_height="0dp"
            android:layout_weight="1" />

        <ImageView
            android:id="@+id/image2"
            android:layout_width="match_parent"
            android:layout_height="0dp"
            android:layout_marginTop="10dp"
            android:layout_weight="1"
            android:background="@color/white" />
    </LinearLayout>
</RelativeLayout>
{% endhighlight %}

在 MainActivity 中对 image1 进行一些操作，同样为 image1 设置 *white* 为背景色，但同时修改了 *alpha* 值为 50(即 float alpha = 
50 / 255 = 0.196，Drawable 的 alpha 计算不一样，是用整数表示的):
{% highlight java%}
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    ImageView image = (ImageView) findViewById(R.id.image1);
    image.setBackgroundResource(R.color.white);

    // 以下两行代码是导致问题出现的原因
    Drawable background = image.getBackground();
    background.setAlpha(50);
}
{% endhighlight %}

以下是分别运行在 *Android 4.4* 和 *Android 5.0* 上的效果：

![on 4.4](/images/android/android-color-demo-good.png)
![on 5.0](/images/android/android-color-demo-bad.png)

从这个 demo 可以看出，在 *Android 5.0* 上，image1 的改动会影响 image2 的背景色，如果是在一个复杂的项目中，出现这种现象会更难以查找原因。

# 问题分析

首先需要说明，这种修改既定资源的做法本身是欠妥的! 

从应用层面上，很难想像有什么场景会需要修改一个 *Color Drawable* 的 *alpha* 值。以实际项目中的这个问题为例，目的是为了改变一个 *View* 的
透明度，直接设置*View* 的透明度就能达到目的了，但是因为 *Android 3.0* 以下系统没有给 *View* 提供 *setAlpha* 这种接口(居然真的没有! 当初
google 为了抢占市场真够拼的)，导致修改了 *Color Drawable* 的属性来达到目的，其实用诸如 *nine old android* 这种兼容库就可以了，底层用的
还是 *Android 3.0* 以下就提供的 *View Animation* 来做的。

既然用法不对，换成操纵 View 就成了，问题的解法倒是非常简单。但弄清楚这个现象的产生原因也是很有意义的，这涉及到调试 *Android* 的源码，看看 *Android 5.0* 
做了哪些优化引起的。

一些实验：

- 再声明一个 *Color* 资源，让 image2 使用 *white2*，结果 -> 有问题
{% highlight xml%}
<color name="white2">#ffffffff</color>
{% endhighlight %}

- layout 中让 image2 直接使用 literal color resource，结果 -> 有问题
{% highlight xml%}
...
android:background="#ffffffff"
...
{% endhighlight %}

- 在以上的两个实验里，把颜色值改掉任意一位，比如 #fffffffd，结果 -> 正常了


## 调试过程
准备好 *Android 5.0 (API 21)* 源码，以上面的 demo 为例来调试。

先看以下 setBackgroundResource(resId) 的调用流程 ：

View.java :
{% highlight java%}
public void setBackgroundResource(int resid) {
    ...
    if (resid != 0) {
        d = mContext.getDrawable(resid);
    }
    ...
}
{% endhighlight %}

--> 

Context.java :
{% highlight java%}
public final Drawable getDrawable(int id) {
    return getResources().getDrawable(id, getTheme());
}
{% endhighlight %}

-->

Resources.java :
{% highlight java%}
public Drawable getDrawable(int id, @Nullable Theme theme) throws NotFoundException {
    ...
    final Drawable res = loadDrawable(value, id, theme);
    ...
}
{% endhighlight %}

-->

Resources.java :
{% highlight java%}
Drawable loadDrawable(TypedValue value, int id, Theme theme) throws NotFoundException {
    ...
    
    // First, check whether we have a cached version of this drawable
    // that was inflated against the specified theme.
    if (!mPreloading) {
        final Drawable cachedDrawable = getCachedDrawable(caches, key, theme);
        if (cachedDrawable != null) {
            return cachedDrawable;
        }
    }

    // Next, check preloaded drawables. These are unthemed but may have
    // themeable attributes.
    final ConstantState cs;
    if (isColorDrawable) {
        cs = sPreloadedColorDrawables.get(key);
    } else {
        cs = sPreloadedDrawables[mConfiguration.getLayoutDirection()].get(key);
    }

    final Drawable dr;
    if (cs != null) {
        dr = cs.newDrawable(this, theme); // 这里，4.4 和 5.0 的实现时不一样的
    } else if (isColorDrawable) {
        dr = new ColorDrawable(value.data);
    } else {
        dr = loadDrawableForCookie(value, id, theme);
    }
        
    // cache the drawble

    ...
}
{% endhighlight %}

*loadDrawable* 这个方法是重点，它用到了一个 *cache* 机制。

对 *Color Drawable* 而言，这里的 *key* 参数是颜色值，白色 #ffffffff 的数值表示为 -1，所以在这里可以下一个条件断点，只针对 *white* 来调试。

![设置条件断点](/images/android/android-color-demo-breakpoint1.png)

第一次 hit 的时候，是没有 cache 的:

![hit 1](/images/android/android-color-demo-breakpoint2.png)

接下来会创建 Drawable，根据 loadingDrawable 的代码，用到了一个静态变量，一个全局的 cache *sPreloadedColorDrawables*，此时它的状态是
这样的:

![context 1](/images/android/android-color-demo-context1.png)

这几个 *ColorDrawable$ColorState* 其实表示的是颜色分别是 :

1. 半透明白 0x7dffffff -> -2147483648
2. 半透明黑 0x7d000000 -> -2130706433
3. 全透明白 0x00ffffff -> -16777216
4. 全白 0xffffffff -> -1
5. 全黑 0x00000000 -> 0

创建的 *Color Drawable* *3817* 的 *mColorState* 属性和 *sPreloadedColorDrawables* 的第四个是同一个对象 *3781* :

![context 2](/images/android/android-color-demo-context2.png)

再来看看此时的调用堆栈 ：

![stack 1](/images/android/android-color-demo-stack1.png)

很明显，是从 layout 文件解析 image2 的时候调用的

第二次 hit 的时候，已经可以取到 cache 了 :

![hit 2](/images/android/android-color-demo-breakpoint3.png)

这时候的调用堆栈 ：

![stack 2](/images/android/android-color-demo-stack2.png)

很明显，是为 image1 设置背景的时候调用的，再看看此时的 *Color Drawable* *3864*，其 *mColorState* 和前一个一样，都是 *3781* :

![context 3](/images/android/android-color-demo-context3.png)

所以可以推断，当 image1 改变 *3864* 的 alpha 时，其实更改的是 *3781* 的属性，而 *3781* 属于全局 cache *sPreloadedColorDrawables*，
所有和这个 *3781* *ColorDrawable$ColorState* 相关联的 *Color Drawable* 都会受影响，最终绘画出来的并非是全白色。

有代码为证：

ColorDrawable.java : 
{% highlight java%}
public void setAlpha(int alpha) {
    alpha += alpha >> 7;   // make it 0..256
    final int baseAlpha = mColorState.mBaseColor >>> 24;
    final int useAlpha = baseAlpha * alpha >> 8;
    final int useColor = (mColorState.mBaseColor << 8 >>> 8) | (useAlpha << 24);
    if (mColorState.mUseColor != useColor) {
        mColorState.mUseColor = useColor;
        invalidateSelf();
    }
}
{% endhighlight %}




而之所以 *Android 4.4* 版本的是正常的，是因为在创建 image1 的 *Color Drawable* 时，其 *mColorState* 并非取自全局变量 *sPreloadedColorDrawables*，
而是拷贝了一份。

4.4 代码 ：
{% highlight java%}
private ColorDrawable(ColorState state) {
    mState = new ColorState(state);
}
{% endhighlight %}

5.0 代码 ：
{% highlight java%}
private ColorDrawable(ColorState state, Resources res, Theme theme) {
    if (theme != null && state.canApplyTheme()) {  //state.canApplyTheme() 返回的一直时 false，目前没看出有什么用。
        mColorState = new ColorState(state);
        applyTheme(theme);
    } else {
        mColorState = state;
    }

    mTintFilter = updateTintFilter(mTintFilter, state.mTint, state.mTintMode);
}
{% endhighlight %}


# 总结
1. 调试源码时，下条件断点很有用，否则 hit 次数太多，没有效率
2. 这个问题不算 *Android* 的 bug, 在不断迭代的过程中，可以看出 *Android* 是在做大量优化的，只是由于项目中不当的用法和这个优化有关联，才出现
这个怪现象