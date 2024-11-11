# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(PkgConfig REQUIRED)
pkg_check_modules(SSL REQUIRED IMPORTED_TARGET libcrypto libssl openssl)
pkg_check_modules(PAM REQUIRED IMPORTED_TARGET pam)
find_package(Dtk6 REQUIRED COMPONENTS Core Tools)
find_package(DDM REQUIRED COMPONENTS Common Auth)

add_library(ds-auth SHARED
    authmanager.h
    authmanager.cpp
    authinterface.h
    authinterface.cpp
    daauth.h
    daauth.cpp
    ddmauth.h
    ddmauth.cpp
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

target_include_directories(ds-auth PRIVATE
    ${PAM_INCLUDE_DIR}
)

target_link_libraries(ds-auth PRIVATE
    ${PAM_LIBRARIES}
    PkgConfig::SSL
    Qt6::Core
    Qt6::DBus
    Dtk6::Core
    DDM::Auth
)

target_include_directories(ds-auth PUBLIC
    ${PROJECT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/auth/
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

target_include_directories(ds-auth PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/libdde-auth
    ${CMAKE_CURRENT_SOURCE_DIR}/global_util/dbus/
)

target_sources(ds-auth PRIVATE
    ${DBUS_INTERFACES}
)
