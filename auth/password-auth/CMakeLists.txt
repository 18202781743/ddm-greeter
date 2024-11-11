# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# find_package(PAM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SSL REQUIRED IMPORTED_TARGET libcrypto libssl openssl)
pkg_check_modules(PAM REQUIRED IMPORTED_TARGET pam)
find_package(Dtk6 REQUIRED COMPONENTS Core Tools)

add_library(ds-greeter-passwordauth SHARED
    passwordauthapplet.h
    passwordauthapplet.cpp
)

target_include_directories(ds-greeter-passwordauth PRIVATE
    ${PAM_INCLUDE_DIR}
)

target_link_libraries(ds-greeter-passwordauth PRIVATE
    Dde::Shell
    ds-auth
)

target_include_directories(ds-greeter-passwordauth PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ds_install_package(PACKAGE org.deepin.ds.greeter.passwordauth TARGET ds-greeter-passwordauth)
