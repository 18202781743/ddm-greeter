# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDM REQUIRED COMPONENTS Common)

add_library(ds-greeter-session SHARED
    sessionapplet.h
    sessionapplet.cpp
    sessionmodel.h
    sessionmodel.cpp
)

target_link_libraries(ds-greeter-session PRIVATE
    Dde::Shell
    DDM::Common
)

target_include_directories(ds-greeter-session PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/global_util/dbus/
)

ds_install_package(PACKAGE org.deepin.ds.greeter.sessioncontrol TARGET ds-greeter-session)
