package com.demo.lyf;

/**
 * Created by yifengliu on 16/2/29.
 */
@DBTable(name = "Member")
public class Member {
    //在使用注解过程中，如果有元素是value，并且只有value需要赋值，
    //则只需要在()中将值写入
    @SQLString(30)
    private String firstName;
    @SQLString(50)
    private String lastName;
    @SQLInteger
    private Integer age;
    @SQLString(value = 30, constraints = @Constraints(primaryKey = true))
    private String handle;

    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getHandle() {
        return handle;
    }

    public void setHandle(String handle) {
        this.handle = handle;
    }
}
