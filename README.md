# dtkdeclarative

dtkdeclarative is a widget development toolkit based on QtQuick/QtQml, which is a brand new substitute for dtkwidget. dtkdeclarative is developed based on qtdeclarative. It covers all existing QML widgets and adds plenty of DTK friendly visual effects and color schemes. Compared to dtkwidget. It has:

1. **A primitive Qt and Qml code style**
2. **Adapted APIs with traditional Qml**
3. **Simple and quick development interfaces**
4. **Unified widget theme style**
5. **Abundant effects and colors**

[中文文档](README_zh_CN.md)

## Dependencies

+ qml-module-qtquick-shapes
+ qml-module-qtquick-layouts

## Build dependencies

+ qtdeclarative5-dev
+ qtbase5-dev-tools,
+ qtquickcontrols2-5-dev
+ libdtkgui-dev
+ libdtkcore-dev
+ qtdeclarative5-private-dev
+ qtbase5-private-dev

## Installation

### Build from source

1. Please make sure that all dependencies are installed
2. Get source and build

```shell
$ git clone https://github.com/linuxdeepin/dtkdeclarative.git
$ mkdir build
$ cd build
$ qmake ..
$ make
```

3. Install

```shell
$ sudo make install
```

## Getting help

+ Please feel free to report issues if you encounter any problem
+ [Deepin Community](https://bbs.deepin.org/) for generic discussion and help.

## Getting Involved

We encourage you to report issues and contribute changes

+ [Contribution guide for developers](https://github.com/linuxdeepin/developer-center/wiki/Contribution-Guidelines-for-Developers-en)(English)

## License

dtkdeclarative is licensed under the [LGPL-3.0-or-later](LICENSE)

## TODO

1. Add plugins.qmltypes, support code completion
2. Support Qt6
3. Support Vulkan
