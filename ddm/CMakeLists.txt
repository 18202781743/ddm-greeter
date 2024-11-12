# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDM REQUIRED COMPONENTS Common Auth)

add_library(ds-greeter-ddm SHARED
    ddmapplet.h
    ddmapplet.cpp
)

target_include_directories(ds-greeter-ddm PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ds-greeter-ddm PRIVATE
    Dde::Shell
    DDM::Auth
)

ds_install_package(PACKAGE org.deepin.ds.greeter.ddm TARGET ds-greeter-ddm)
