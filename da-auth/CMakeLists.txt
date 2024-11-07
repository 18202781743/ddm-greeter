# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# find_package(PAM REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SSL REQUIRED IMPORTED_TARGET libcrypto libssl openssl)
pkg_check_modules(PAM REQUIRED IMPORTED_TARGET pam)
find_package(Dtk6 REQUIRED COMPONENTS Core Tools)

add_library(ds-greeter-daauth SHARED
    daauthapplet.h
    daauthapplet.cpp
    daauth.h
    daauth.cpp
    libdde-auth/authcommon.h
    libdde-auth/deepinauthframework.cpp
    libdde-auth/deepinauthframework.h
    global_util/dbus/dbuslogin1manager.cpp
    global_util/dbus/dbuslogin1manager.h
    global_util/dbus/dbusvariant.cpp
    global_util/dbus/dbusvariant.h
    global_util/dbus/login1sessionself_interface.h
    global_util/dbus/login1sessionself_interface.cpp
    global_util/dbus/types/mfainfolist.cpp
    global_util/dbus/types/mfainfolist.h
    global_util/dbus/types/arrayint.cpp
    global_util/dbus/types/arrayint.h
)

target_include_directories(ds-greeter-daauth PRIVATE
    ${PAM_INCLUDE_DIR}
)

target_link_libraries(ds-greeter-daauth PRIVATE
    ${PAM_LIBRARIES}
    Dde::Shell
    PkgConfig::SSL
)

dtk_add_dbus_interface(
    DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/xml/org.deepin.dde.Authenticate1.xml
    authenticate_interface
)

dtk_add_dbus_interface(
    DBUS_INTERFACES
    ${CMAKE_CURRENT_SOURCE_DIR}/xml/org.deepin.dde.Authenticate1.Session2.xml
    authenticatesession2_interface
)

target_sources(ds-greeter-daauth PRIVATE
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.h
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.cpp
    ${DBUS_INTERFACES}
)

target_include_directories(ds-greeter-daauth PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libdde-auth
    ${CMAKE_CURRENT_SOURCE_DIR}/global_util/dbus/
)

ds_install_package(PACKAGE org.deepin.ds.greeter.daauth TARGET ds-greeter-daauth)
