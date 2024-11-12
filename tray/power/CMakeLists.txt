# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDM REQUIRED COMPONENTS Common)

add_library(ds-greeter-power SHARED
    powerapplet.h
    powerapplet.cpp
    powerinterface.h
    powerinterface.cpp
    dapower.h
    dapower.cpp
    ddmpower.h
    ddmpower.cpp
    global_util/dbus/sessionmanager_interface.cpp
    global_util/dbus/sessionmanager_interface.h
)

target_link_libraries(ds-greeter-power PRIVATE
    Dde::Shell
    DDM::Common
)

target_include_directories(ds-greeter-power PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/global_util/dbus/
)

ds_install_package(PACKAGE org.deepin.ds.greeter.powertray TARGET ds-greeter-power)
