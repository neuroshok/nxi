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
- Custom Qt6 build binaires
- c++17 compiler

## Cmake options
**user-config.cmake**
```cmake
if (NXI_TARGET_DESKTOP)
    set(NXI_WINDOW_GENERIC ON) # user generic/platform windows
    set(NXI_CONSOLE ON) # display/hide the console

    module_build(nxi.module adk) # build the adk static module from *nxi.module* module pack
    module_build(nxi.module example.rust) # build the example.rust dynamic module from *nxi.module* module pack
endif()
```

## Commands
```
git clone --recurse-submodules https://github.com/neuroshok/nxi.git && cd nxi
```

```
mkdir build && cd build
```

```
cmake -DNXI_QT_ROOT=/path/to/qt/version/compiler  ..
```

```
cmake --build . --target nxi_deploy --config Release
```