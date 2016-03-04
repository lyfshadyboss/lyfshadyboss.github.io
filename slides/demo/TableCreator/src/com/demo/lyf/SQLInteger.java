package com.demo.lyf;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Created by yifengliu on 16/2/29.
 */
@Target(ElementType.FIELD)
@Retention(RetentionPolicy.RUNTIME)
public @interface SQLInteger {
    String name() default "";

    //嵌套注解的功能,将column类型的数据库约束信息嵌入其中
    Constraints constraints() default @Constraints;
}
