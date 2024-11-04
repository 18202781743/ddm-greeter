# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDM REQUIRED COMPONENTS Common Auth)

add_library(ds-greeter-ddmauth SHARED
    ddmauthapplet.h
    ddmauthapplet.cpp
)

target_sources(ds-greeter-ddmauth PRIVATE
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.h
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.cpp
)

target_include_directories(ds-greeter-ddmauth PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ds-greeter-ddmauth PRIVATE
    Dde::Shell
    DDM::Auth
)

ds_install_package(PACKAGE org.deepin.ds.greeter.ddmauth TARGET ds-greeter-ddmauth)
