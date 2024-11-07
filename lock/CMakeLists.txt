# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(ds-greeter-lock SHARED
    lockapplet.h
    lockapplet.cpp
    dbuslockfrontservice.h
    dbuslockfrontservice.cpp
)

target_include_directories(ds-greeter-lock PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ds-greeter-lock PRIVATE
    Dde::Shell
)

ds_install_package(PACKAGE org.deepin.ds.greeter.lock TARGET ds-greeter-lock)
