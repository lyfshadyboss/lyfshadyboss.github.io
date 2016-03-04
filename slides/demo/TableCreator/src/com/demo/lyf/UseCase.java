package com.demo.lyf;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 定义一个注解
 * Created by yifengliu on 16/2/29.
 */
@Target(ElementType.METHOD)//定义该注解将应用于什么地方，方法或者域
@Retention(RetentionPolicy.RUNTIME)//定义该注解在哪一个级别可用
public @interface UseCase {
    //注解元素，可以指定默认值，在使用注解的时候，可以直接给元素赋值如id=5
    int id();

    String description() default "no description";

    // 默认值,在使用注解的时候，只需要为元素赋值
    ParameterType type() default ParameterType.STRING;

    //利用枚举来设置参数类型
    enum ParameterType {
        STRING, SHORT, INT, BOOL, LONG, OBJECT
    }
}