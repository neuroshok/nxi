# nxi

## Overview 

**nxi** is a highly customizable et modulable data explorer (browser / file explorer)

**Features**

- Everything is parameterizable and modulable
- Generic pages (web, explorer, custom) managed by a graph
- Commands by modules (nxi is the main module) stored in a graph
- Advanced shortcuts (sequential, contextual)
- Custom properties on pages (colors, names, ...)
- Session system
- Modules : static, dynamic and scripted (c++, c, rust, js ...)
- W3C webextensions and themes support (partial)
- Context system to adapt the behaviour of nxi based on active / focused context
- File explorer to ease web/local interactions
- Configuration per page / page node
- And many other ..

![image1](https://data.neuroshok.com/nxi/g/overview1.png)
![image2](https://data.neuroshok.com/nxi/g/overview2.png)

# Build

## Prerequisites

- [Custom Qt6 build binaries](https://github.com/neuroshok/fork.qt/releases/tag/latest)
- C++17 compiler
- [xmake](https://github.com/xmake-io/xmake/releases)

## Build options

- `nxi_window` use **generic** or **platform** windows
- `nxi_console` display a console

## Commands

```
git clone --recurse-submodules https://github.com/neuroshok/nxi.git && cd nxi
```

```
xmake config --qt=/path/to/qt/version/compiler 
```

```
xmake build
```

```
xmake install
```