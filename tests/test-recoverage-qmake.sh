#!/bin/bash

# SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: LGPL-3.0-or-later

BUILD_DIR=build
REPORT_DIR=report

# 编译 src 目录给 tests 目录使用
cd ../
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ..
make
cd ../tests/

# 编译 tests 目录
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../

# 1.执行编译后的测试文件
make check

# 2.收集测试文件运行后产生的覆盖率文件
lcov -d ./ -c -o coverage_all.info

# 3.过滤不需要关注的源文件路径和信息
lcov --remove coverage_all.info "*/tests/*" "/usr/include*" "*build/src*" "*/src/private/*" "QtCore/*" "QtGui/*" "QtQml/*" "QtQuick/*" --output-file coverage.info
cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info

# 删除 build 产生的文件
rm -rf $BUILD_DIR
rm -rf ../$BUILD_DIR
