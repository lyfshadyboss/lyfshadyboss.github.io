---
layout: post
title: "安装 Fedora 系统"
categories: jekyll
tags: Fedora
---

下载 fedora iso 文件 ： [fedora project](http://fedoraproject.org/)

使用U盘安装时，准备一个 U 盘，下载 [live usb creator](https://fedorahosted.org/liveusb-creator/) 工具, 制作 usb 启动器后安装。

使用硬盘安装时，下载这些工具： [easyBCD](http://neosmart.net/EasyBCD/) (windows 下可引导 linux, mac 等系统)，
[分区助手](http://www.disktool.cn/) (windows 下的分区工具)。

用分区助手分一块 4G 左右的空间出来（假设为 sdXY，即第 X 块磁盘的第 Y 个分区），格式为 FAT32，将 iso 文件解压到该分区，
提取 vmlinuz0 和 initrd0.img 文件到根目录。

配置 easyBCD 的 NeoGrub 条目，加入一个引导项：

    title install fedora
    root (hdA, B)
    kernel /vmlinuz0 root=live:/dev/sdXY ro liveimg rhgb
    initrd /initrd0.img
    boot

A,B 与 X,Y 的对应关系为：

    A = X - 1
    B = Y - 1

重启电脑，选择 NeoGrub 引导条目，即可看到 `install fedora` 的选项。