# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(ds-greeter-auth SHARED
    authapplet.h
    authapplet.cpp
)

target_link_libraries(ds-greeter-auth PRIVATE
    Dde::Shell
    ds-auth
)

ds_install_package(PACKAGE org.deepin.ds.greeter.auth TARGET ds-greeter-auth)

add_subdirectory(password-auth)
add_subdirectory(face-auth)
