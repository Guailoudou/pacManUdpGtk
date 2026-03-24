<!--
 * @Date: 2026-03-24 10:21:29
 * @LastEditors: Guailoudou Guailoudou@outlook.com
 * @LastEditTime: 2026-03-24 10:23:02
 * @FilePath: /2JIEDUAN/README.md
-->
# 🍬 超级吃豆人追逐战 (Super Pac-Man Chase)

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Guailoudou/pacManUdpGtk/blob/main/LICENSE)
[![GTK](https://img.shields.io/badge/GTK-3.0-lightgrey.svg)](https://www.gtk.org/)
[![Network](https://img.shields.io/badge/Protocol-UDP-orange.svg)]()

一款基于 **UDP 协议** 和 **GTK3.0** 开发的轻量级、低延迟双人联机吃豆人游戏。支持远程联机、随机地图生成以及极致的帧同步体验。

---

## ✨ 核心特色

*   🚀 **极致低延迟网络架构**
    *   采用 **UDP 协议** 进行通信，相比 TCP 大幅降低延迟，适合快节奏竞技。
    *   支持通过端口映射，轻松实现 **远程联机**。
    *   **50ms 帧同步** 机制，每帧仅需 **20 Bytes** 的微小数据包，带宽需求极低，在网络波动环境下依然流畅。
*   🗺️ **动态随机地图**
    *   每回合地图随机生成，充满不确定性，“运气也是实力的一部分”。
    *   内置 **地图传输重发包机制**，有效防止因 UDP 丢包导致的地图数据损坏或不同步问题。
*   🎮 **双端操作支持**
    *   **键盘党**：支持 `WASD` 键位移动。
    *   **触屏党**：内置虚拟按钮，支持触屏点击移动，随时随地畅玩。
*   ⚡ **高性能渲染引擎**
    *   基于 **GTK3.0** 图形库，轻量高效。
    *   采用 **地图区块渲染技术**，仅渲染玩家视野附近的区块，显著降低 CPU/GPU 负载，提升运行帧率。

---

## 🎮 游戏规则

本游戏为 **1v1 非对称竞技**，两名玩家轮流扮演“吃豆人”与“鬼”。

### 角色与目标
| 颜色 | 角色 | 说明 |
| :---: | :--- | :--- |
| 🟢 **绿色** | **吃豆人** | 目标是尽可能多地吃掉金币。游戏开始后即可移动。 |
| 🔴 **红色** | **鬼** | 目标是追捕吃豆人。**游戏开始后需等待 5 秒** 方可移动（给予吃豆人逃跑时间）。 |
| ⬛ **黑色** | **墙壁** | 不可通过的障碍物。 |
| ⬜ **白色** | **空地** | 可自由通行的区域。 |
| 🟡 **黄色** | **金币** | 每个金币价值 **1 分**。 |

### 胜负判定
1.  **回合结束条件**：当“鬼”捉到“吃豆人”时，当前回合立即结束。
2.  **身份互换**：回合结束后，双方交换身份（原吃豆人变鬼，原鬼变吃豆人），并重新生成一张新地图。
3.  **最终胜利**：经过若干回合后，**累计获得金币总数更多** 的一方获胜。

---

## 🛠️ 编译与运行

本项目依赖 `GTK+ 3.0` 开发库。请确保您的系统已安装相关依赖。

### 前置依赖
*   GCC 编译器
*   GTK+ 3.0 开发包 (`libgtk-3-dev` 或 `gtk3-devel`)
*   pkg-config

### 编译命令
在项目根目录下执行以下命令：

```bash
gcc -o game *.c $(/usr/bin/pkg-config --cflags --libs gtk+-3.0)
```

### 运行游戏
编译成功后，运行生成的可执行文件：

```bash
./game
```

> **提示**：如果是远程联机，请确保主机已配置好 UDP 端口映射，并将公网 IP 告知对方。

---

## 📦 技术栈

*   **语言**: C
*   **GUI 框架**: GTK 3.0
*   **网络协议**: UDP (User Datagram Protocol)
*   **同步机制**: 锁步帧同步 (Lockstep Frame Synchronization)
*   **渲染优化**: 视锥剔除 / 局部区块渲染

---

## 📄 开源地址

欢迎提交 Issue 和 Pull Request！

🔗 **GitHub Repository**: [https://github.com/Guailoudou/pacManUdpGtk](https://github.com/Guailoudou/pacManUdpGtk)

---

## 📝 License

本项目采用 MIT 协议开源。

---

*享受追逐的乐趣，体验速度的激情！* 🏃💨👻