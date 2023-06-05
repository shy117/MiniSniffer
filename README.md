# MiniSniffer

个人网络嗅探器

## 系统功能模块

![image-20230605224830415](/README.assets/image-20230605224830415.png)

## 流程图

### WinPcap系统处理流程

![image-20230605224924616](./README.assets/image-20230605224924616.png)

### 程序总流程

![img](./README.assets/clip_image002.png)

### 包重组

![img](./README.assets/clip_image002-1685976588529.png)

## 主要数据结构

![img](./README.assets/clip_image002-1685976606544.png)

## analyz_frame数据包分析

![image-20230605225126677](./README.assets/image-20230605225126677.png)

## 界面设计

![image-20230605225041233](./README.assets/image-20230605225041233.png)

## 测试

### 抓包设置

#### 网卡选择

![img](./README.assets/clip_image002-1685976744013.png)

#### 过滤器设置

过滤规则样例：ether src 源MAC地址

​       ether dst 目的MAC地址

​       ip src 源IP地址

​       ip dst 目的IP地址

​       udp and (ip src 源IP地址 or ip dst 目的IP地址)

​       port 443 or port 80

![img](./README.assets/clip_image004.png)

图 8 侦听指定来源IP 地址的数据包

![img](./README.assets/clip_image006.png)

图 9 侦听指定目的 IP 地址的数据包

### 数据显示

#### 流量显示

![img](./README.assets/clip_image008.png)

#### 数据包列表

![img](./README.assets/clip_image010.png)

###  解析显示数据包

![img](./README.assets/clip_image012.png)

#### 显示原始数据

![img](./README.assets/clip_image014.png)

#### 统计数据包个数

![img](./README.assets/clip_image016.png)

### 数据包操作

#### 文件另存为

![img](./README.assets/clip_image018.png)

#### 打开源有数据包

![img](./README.assets/clip_image020.png)

####  包重组

先选择虚拟网卡

![img](./README.assets/clip_image022.png)

开始抓包

并在虚拟机里使用命令 ping 192.168.31.11 -s 10000 

![img](./README.assets/clip_image024.png)

结束抓包

选择分段数据包

点击IP重组

![img](./README.assets/clip_image026.png)

![img](./README.assets/clip_image028.png)
