---
layout: post
title: "maven 快速预览"
categories: tools maven
tags: tool maven
---

### 安装
maven 是一个 java 工具，所以必须先安装 java．

1. 下载，[apache maven download](http://maven.apache.org/download.html)
2. 安装，参照 [maven install installation](http://maven.apache.org/download.html#Installation)

安装完成后输入：

    mvn --version

输出当前的 maven 信息，例如：

    Apache Maven 3.1.1 (NON-CANONICAL_2013-11-08_14-32_mockbuild; 2013-11-08 22:32:41+0800)
    Maven home: /usr/share/maven
    Java version: 1.7.0_55, vendor: Oracle Corporation
    Java home: /usr/lib/jvm/java-1.7.0-openjdk-1.7.0.60-2.4.7.0.fc20.x86_64/jre
    Default locale: zh_CN, platform encoding: UTF-8
    OS name: "linux", version: "3.14.3-200.fc20.x86_64", arch: "amd64", family: "unix"

### 创建工程
在终端执行下面的命令：

    mvn archetype:generate -DgroupId=com.mycompany.app -DartifactId=my-app \
                           -DarchetypeArtifactId=maven-archetype-quickstart \
                           -DinteractiveMode=false

如果是第一次安装 maven，会需要等待一段时间，是因为要下载一些常用的资源（插件和常用的库，放在 ~/.m2/）．执行完成后，
会创建一个工程目录，名称就是输入的 `my-app`．进入目录：

    cd my-app

工程的目录结构如下：

    my-app
    |-- pom.xml
    `-- src
        |-- main
        |   `-- java
        |       `-- com
        |           `-- mycompany
        |               `-- app
        |                   `-- App.java
        `-- test
            `-- java
                `-- com
                    `-- mycompany
                        `-- app
                            `-- AppTest.java


`src/main/java/` 目录包含工程源码，`src/test/java/` 包含工程测试代码，`pom.xml` 就是工程的 *工程对象模型*，
简称 POM．


#### POM
POM 文件在 maven 中是一种核心的配置文件，包含了大量构建工程的信息．这个文件可能非常巨大而复杂，不过即使不用了解它的
全部也能顺利的使用 maven．初始的 POM 文件内容是：

    <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
      <modelVersion>4.0.0</modelVersion>

      <groupId>com.mycompany.app</groupId>
      <artifactId>my-app</artifactId>
      <version>1.0-SNAPSHOT</version>
      <packaging>jar</packaging>

      <name>Maven Quick Start Archetype</name>
      <url>http://maven.apache.org</url>

      <dependencies>
        <dependency>
          <groupId>junit</groupId>
          <artifactId>junit</artifactId>
          <version>4.8.2</version>
          <scope>test</scope>
        </dependency>
      </dependencies>
    </project>


#### 工程如何产生的？
之所以会生成工程目录，是因为使用了 *archetype:generate*，前缀 *archetype* 是 *plugin*， *generate* 是
*goal*．这很类似 Ant 的任务，或者 Rake 的任务．这个方法根据 *archetype* 创建了一个简单的工程．基本上，一个插件包含一系列
的 goal，用于处理特定的一组问题，比如插件 *jboss-maven-plugin* 就是用来处理各种与 jboss 相关的问题的．

#### 构建工程

    mvn package

输出信息：

     ...
    [INFO] ------------------------------------------------------------------------
    [INFO] BUILD SUCCESSFUL
    [INFO] ------------------------------------------------------------------------
    [INFO] Total time: 2 seconds
    [INFO] Finished at: Thu Jul 07 21:34:52 CEST 2011
    [INFO] Final Memory: 3M/6M
    [INFO] ------------------------------------------------------------------------

和前面的 `archetype:generate` 不同，这不是一个 *goal*，而是一个 *phase*，是 *build lifecycle* 中的一步．
*build livecycle* 是系列有序的 *phase*，当执行其中一个 *phase* 时，这个 *phase* 前面的也会被依次执行．比如执行
*compile phase*，实际上依次执行的是：

1. validate
2. generate-sources
3. process-sources
4. generate-resources
5. process-resources
6. compile


可以测试刚才构建出的结果：

    java -cp target/my-app-1.0-SNAPSHOT.jar com.mycompany.app.App

输出：

    Hello World!

### 使用 maven 工具

#### maven phases
列举一些常用的 *lifecycle phase*：

* `validate` : 验证工程是否正确，需要的信息是否存在
* `compile`  : 编译工程的源码
* `test`     : 执行测试
* `package`  : 打包，比如打包成 JAR 文件
* `integration-test`  : 将构建出的包文件发布到集成开发环境测试
* `verity`   : 验证包是否如何规范
* `install`  : 将包安装到本地仓库（~/.m2/repository），可以被其他工程使用
* `deploy`   : 发布到集成测试环境或者互联网，可以被其他的开发者使用

还有两个 *phase*：

* `clean`    : 清理构建过程产生的文件
* `site`     : 生成文档

其实在 *phase* 的底层还是调用其他的 *goal*，例如 *package phase* 被执行时，如果工程类型是 jar，则执行 *jar:jar*，
如果是 war，则执行 *war:war*

*goal* 和 *phase* 可以顺序执行：

    mvn clean dependency:copy-dependencies package

#### 生成文档

    mvn site

会根据工程的 pom.xml 中的信息生成 html 文档，以及各种报告，生成的结果在 target/site 下．

### 结论
这只是一些最基本的介绍，更详细的介绍参见 [maven getting started guide](mvn clean dependency:copy-dependencies package)