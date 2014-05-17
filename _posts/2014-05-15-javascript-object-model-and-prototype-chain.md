---
layout: post
title: "Javascript 对象模型和原型链"
categories: programming javascript
tags: Javascript
---

<center>![prototype chain](/images/javascript-object-layout.jpg)</center>

###重点

* Mozilla/Konqueror/Chrome 中， 对象都有一个属性叫 `__proto__`，这个属性串起了整个原型链

* 所有构造器的实例的 `__proto__` 都指向这个构造器的 `prototype`，默认的，这个 `prototype` 对象也有个
  `constructor` 属性指向构造器

        function Person(name) {
            this.name = name
        }

        person = new Person('tom')
        person.__proto__ == Person.prototype   // => true
        person.__proto__.constructor == Person // => true

* `prototype` 对象只是被构造器的实例所使用 (被 `__proto__` 引用)，构造器本身并不使用这个对象的属性。但是，
  构造器本身也是系统 `Function` 对象的一个实例，所以 `Function` 对象的 `prototype` 是与构造器有联系的

        function Foo() { } ;
        var f1 = new Foo();

        Foo.prototype.x = "hello";
        f1.x    //=> hello
        Foo.x   //=> undefined

        Function.x = 'hello u';
        Foo.x   //=> hello u

* 构造器的 `prototype` 对象可以被替换成开发者指定的对象，这样做的时候，最好也适当的设置 `constructor` 属性

        function foo() { } ;
        var f1 = new foo();
        f1.constructor === foo.prototype.constructor === foo

        //replace the default prototype object
        foo.prototype = new Object();

        //now we have:
        f1.constructor === foo.prototype.constructor === Object

        //so now we say:
        foo.prototype.constructor == foo

        //all is well again
        f1.constructor === foo.prototype.constructor === foo


* 每一个 `prototype` 对象本身都是 `Object` 构造器创建的一个实例，因此 `prototype` 对象的 `__proto__` 属性
  指向 `Object` 的 `prototype` 对象。所以所有的实例(除了 `Object.prototype` 这个最顶层的对象) 都继承了 `Object.prototype`
  对象的属性 (通过原型链继承)

* 访问对象的属性时，会先从这个对象开始，沿着原型链，直至 `Object.prototype` 这个顶层对象

        function Foo() { }
        f1 = new Foo();
        f2 = new Foo();
        Foo.prototype.x = "hello";

        f1.x  => "hello"
        f2.x  => "hello";

        f1.x = "goodbye";   //setting f1.x hides Foo.prototype.x

        f1.x  => "goodbye"  //hides "hello" for f1 only
        f2.x  => "hello"

        delete f1.x
        f1.x  => "hello";   //Foo.prototype.x is visible again to f1.

        Foo.prototype.x = "goodbye";
        //now
        f1.x  => "goodbye"
        f2.x  => "goodbye";


### 其他信息

根据上面的对象关系图，配合在 javascript 终端验证，可以得出更有意思的结论：

* `Function.__proto__` 指向 `Function.prototype`，所以：

        Function.constructor === Function

* Object instanceof Object == true， 因为：

        Object.__proto__.__proto__.constructor == Object

  但是

  Foo instanceof Foo == false，因为在 Foo 的原型链上找不到符合上面的那种关系

### 附录 && 链接
1. [对象模型的另外一种表示法](/images/javascript-prototype-chain.png)
2. [javascript object layout && prototype chain](http://mollypages.org/misc/js.mp)
