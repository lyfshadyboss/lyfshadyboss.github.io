---
layout : post
title : "Parcelable VS Serializable"
excerpt : ""
categories : articles
tags : [Translation, Java, Parcelable, Serializable]
---

翻译自 ： [Developer Phil : Parcelable vs Serializable](http://www.developerphil.com/parcelable-vs-serializable/)

在 *Android* 中，不能直接在 *Activity* 和 *Fragment* 之间传递对象，而是要通过 *Intent/Bundle*. API 说明中，对于要传递的对象格式，有两种选择：

- 传递 Parcelable 对象
- 传递 Serializable 对象

*Serializable* 在 java 中很常见，那么 *Parcelable* 呢 ? 下面来看看两者的区别.

# Serializable, 简洁之道

{% highlight java %}
// access modifiers, accessors and constructors omitted for brevity
public class SerializableDeveloper implements Serializable
    String name;
    int yearsOfExperience;
    List<Skill> skillSet;
    float favoriteFloat;

    static class Skill implements Serializable {
        String name;
        boolean programmingRelated;
    }
}
{% endhighlight %}

使用 *Serializable* 非常简单 : 只需要让你的类实现 *Serializable* 接口即可，剩下的序列化任务就交给 Java 虚拟机了!

问题是这种方法虽然简单易用，但它使用了 *reflection* 机制，导致速度存在瓶颈，而且序列化过程会消耗较多内存，引起频繁的垃圾回收.

# Parcelable, 速度为王
{% highlight java %}
// access modifiers, accessors and regular constructors ommited for brevity
class ParcelableDeveloper implements Parcelable {
    String name;
    int yearsOfExperience;
    List<Skill> skillSet;
    float favoriteFloat;

    ParcelableDeveloper(Parcel in) {
        this.name = in.readString();
        this.yearsOfExperience = in.readInt();
        this.skillSet = new ArrayList<Skill>();
        in.readTypedList(skillSet, Skill.CREATOR);
        this.favoriteFloat = in.readFloat();
    }

    void writeToParcel(Parcel dest, int flags) {
        dest.writeString(name);
        dest.writeInt(yearsOfExperience);
        dest.writeTypedList(skillSet);
        dest.writeFloat(favoriteFloat);
    }

    int describeContents() {
        return 0;
    }


    static final Parcelable.Creator<ParcelableDeveloper> CREATOR
            = new Parcelable.Creator<ParcelableDeveloper>() {

        ParcelableDeveloper createFromParcel(Parcel in) {
            return new ParcelableDeveloper(in);
        }

        ParcelableDeveloper[] newArray(int size) {
            return new ParcelableDeveloper[size];
        }
    };

    static class Skill implements Parcelable {
        String name;
        boolean programmingRelated;

        Skill(Parcel in) {
            this.name = in.readString();
            this.programmingRelated = (in.readInt() == 1);
        }

        @Override
        void writeToParcel(Parcel dest, int flags) {
            dest.writeString(name);
            dest.writeInt(programmingRelated ? 1 : 0);
        }

        static final Parcelable.Creator<Skill> CREATOR
            = new Parcelable.Creator<Skill>() {

            Skill createFromParcel(Parcel in) {
                return new Skill(in);
            }

            Skill[] newArray(int size) {
                return new Skill[size];
            }
        };

        @Override
        int describeContents() {
            return 0;
        }
    }
}
{% endhighlight %}

据传，这段代码运行速度极快. 原因之一是它没有使用 *reflection* 机制，而是显示的声明了序列化过程.

然而，速度是有代价的，它牺牲了代码的简洁性和易读性.

# 性能测试
到底 *Parcelable* 有多快 ? 让我们来测试一番.

## 测试方法
模拟 *Activity* 之间传递数据，用 *Bundle#writeToParcel(Parcel, int)* 存数据，然后再读取数据，每次测试读写 1000 次. 测试运行十次，然后结果取平均值.

测试的类就是上面的 *SerializableDeveloper* 和 *ParcelableDeveloper*

测试的设备和系统版本 :

LG Nexus 4          - Android 4.2.2 

Samsung Nexus 10    - Android 4.2.2

HTC Desire Z        - Android 2.3.3


## 测试结果

![speed test](/images/parcelable-vs-serializable-speed-test.png)

Nexus 10
Serializable: 1.0004ms,  Parcelable: 0.0850ms - 10.16x improvement.

Nexus 4
Serializable: 1.8539ms - Parcelable: 0.1824ms - 11.80x improvement.

Desire Z
Serializable: 5.1224ms - Parcelable: 0.2938ms - 17.36x improvement.

可以看出，*Parcelable* 比 *Serializable* 快十倍以上. 即使是在 *Nexus 10* 上，对一个简单的对象进行 *序列化/反序列化* 操作都会花大概 1ms.

# 实用原则
推荐使用更快的 *Parcelable*，比 *Serializable* 快 10x 倍，对资源的消耗也更小. *Serializable* 虽然比较慢，但大多数情况可以忽略不计，当
数据量少的时候，也可以用它.