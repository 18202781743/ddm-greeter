# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# find_package(PAM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SSL REQUIRED IMPORTED_TARGET libcrypto libssl openssl)
pkg_check_modules(PAM REQUIRED IMPORTED_TARGET pam)
find_package(Dtk6 REQUIRED COMPONENTS Core Tools)

add_library(ds-greeter-faceauth SHARED
    faceauthapplet.h
    faceauthapplet.cpp
)

target_include_directories(ds-greeter-faceauth PRIVATE
    ${PAM_INCLUDE_DIR}
)

target_link_libraries(ds-greeter-faceauth PRIVATE
    Dde::Shell
    ds-auth
)

target_include_directories(ds-greeter-faceauth PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ds_install_package(PACKAGE org.deepin.ds.greeter.faceauth TARGET ds-greeter-faceauth)
