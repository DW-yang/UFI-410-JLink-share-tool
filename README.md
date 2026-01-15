# UFI 410 JLink Share Tool
### 简介
本项目为运行Debian系统的410随身wifi棒子提供一个自动配置usb并共享JLink调试器的工具

### 原理
通过NetworkManager检测wlan0是否已经连接，只有在wlan0已连接后才会切换usb至host模式，然后启动JLinkRemoteServer。

### 依赖
NetworkManager
JLink软件

### 编译环境
gcc
libnm-dev

#### 编译
make prepare
make build

#### 安装
make install