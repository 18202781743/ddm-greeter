# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDM REQUIRED COMPONENTS Common Auth)

add_library(ds-greeter-ddmsocket SHARED
    ddmsocketapplet.h
    ddmsocketapplet.cpp
)

target_include_directories(ds-greeter-ddmsocket PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ds-greeter-ddmsocket PRIVATE
    Dde::Shell
    DDM::Auth
)

ds_install_package(PACKAGE org.deepin.ds.greeter.ddmsocket TARGET ds-greeter-ddmsocket)
