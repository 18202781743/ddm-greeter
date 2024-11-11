# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later


# Find the library
find_package(PkgConfig REQUIRED)
find_package(Dtk6Widget REQUIRED)
find_package(Dtk6CMake  REQUIRED)
find_package(Dtk6Core REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets Concurrent DBus Xml Svg Network
REQUIRED)
find_package(DtkTools REQUIRED)

add_library(ds-greeter-lock SHARED
    lockapplet.h
    lockapplet.cpp
    # dbuslockfrontservice.h
    # dbuslockfrontservice.cpp
)

set(XML2CPP_DIR ${PROJECT_SOURCE_DIR}/src/global_util/dbus)

function(generation_dbus_interface xml class_name class_file)
    execute_process(COMMAND ${DTK_XML2CPP} -c ${class_name} -p ${class_file} ${xml}
        WORKING_DIRECTORY ${XML2CPP_DIR})
endfunction(generation_dbus_interface)

generation_dbus_interface(
    xml/org.deepin.dde.Authenticate1.xml
    Authenticate
    ${XML2CPP_DIR}/authenticate_interface
)

# generation_dbus_interface(
#     xml/com.huawei.switchos.xml
#     HuaWeiSwitchOS
#     ${XML2CPP_DIR}/huaweiswitchos_interface
# )

generation_dbus_interface(
    xml/org.deepin.dde.Accounts1.xml
    Accounts
    ${XML2CPP_DIR}/accounts_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.Accounts1.User.xml
    AccountsUser
    ${XML2CPP_DIR}/accountsuser_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.ImageEffect1.xml
    ImageEffect
    ${XML2CPP_DIR}/imageeffect_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.Logined.xml
    Logined
    ${XML2CPP_DIR}/logined_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.PowerManager1.xml
    PowerManager
    ${XML2CPP_DIR}/powermanager_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.SystemPower1.xml
    SystemPower
    ${XML2CPP_DIR}/systempower_interface
)

generation_dbus_interface(
    xml/com.deepin.wm.xml
    wm
    ${XML2CPP_DIR}/wm_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.Authenticate1.Session2.xml
    AuthenticateSession2
    ${XML2CPP_DIR}/authenticatesession2_interface
)

generation_dbus_interface(
    xml/org.deepin.dde.SessionManager1.xml
    SessionManager
    ${XML2CPP_DIR}/sessionmanager_interface
)

# generation_dbus_interface(
#     xml/org.deepin.dde.SoundThemePlayer1.xml
#     SoundThemePlayer
#     ${XML2CPP_DIR}/soundthemeplayer_interface
# )

generation_dbus_interface(
    xml/org.deepin.dde.SystemDisplay1.xml
    SystemDisplay
    ${XML2CPP_DIR}/systemdisplay_interface
)

generation_dbus_interface(
    xml/org.freedesktop.DBus.xml
    FreedesktopDBus
    ${XML2CPP_DIR}/freedesktopdbus_interface
)

generation_dbus_interface(
    xml/org.freedesktop.login1.session.self.xml
    Login1SessionSelf
    ${XML2CPP_DIR}/login1sessionself_interface
)

include_directories(global_util)
include_directories(global_util/dbus)
include_directories(global_util/keyboardmonitor)
include_directories(widgets)
include_directories(session-widgets)
include_directories(libdde-auth)
include_directories(global_util/dbus/types)
include_directories(${DTKCORE_INCLUDE_DIRS})

aux_source_directory(global_util GLOBAL_UTILS)
aux_source_directory(global_util/dbus GLOBAL_UTILS_DBUS)
aux_source_directory(global_util/keyboardmonitor GLOBAL_UTILS_KEYBOARDMONITOR)
aux_source_directory(widgets WIDGETS)
# aux_source_directory(session-widgets SESSION_WIDGETS)
aux_source_directory(libdde-auth AUTHENTICATE)
aux_source_directory(global_util/dbus/types DBUS_DATA_TYPES)

set(QRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
    # widgets/widgetsimages.qrc
    # widgets/widgetstheme.qrc
)
set(SESSION_WIDGETS
    session-widgets/sessionbasemodel.h
    session-widgets/sessionbasemodel.cpp
    session-widgets/auth_password.cpp
    session-widgets/auth_password.h
    session-widgets/authinterface.h
    session-widgets/authinterface.cpp
    session-widgets/userinfo.h
    session-widgets/userinfo.cpp
    session-widgets/auth_module.h
    session-widgets/auth_module.cpp
)

set(LOCK_SRCS
    ${transaction_DBUS_SCRS}
    ${GLOBAL_UTILS}
    ${GLOBAL_UTILS_DBUS}
    # ${GLOBAL_UTILS_KEYBOARDMONITOR}
    # ${WIDGETS}
    ${SESSION_WIDGETS}
    ${AUTHENTICATE}
    ${QT_DBUS_EXTENDED}
    ${DBUS_DATA_TYPES}
    dde-lock/lockframe.cpp
    dde-lock/lockworker.cpp
    dde-lock/dbus/dbuslockagent.cpp
    dde-lock/dbus/dbuslockfrontservice.cpp
    dde-lock/dbus/dbusshutdownagent.cpp
    dde-lock/dbus/dbusshutdownfrontservice.cpp
)

target_include_directories(ds-greeter-lock PUBLIC
    ${PAM_INCLUDE_DIR}
    ${PROJECT_BINARY_DIR}
    dde-lock
    dde-lock/dbus
)
target_link_libraries(ds-greeter-lock PRIVATE
    ${PAM_LIBRARIES}
    ${DEEPIN_PW_CHECK}
    Qt6::Core
    Qt6::Gui
    Qt6::DBus
    Qt6::Widgets
    Qt6::Xml
    Qt6::Svg
    Dtk6::Widget
)

set(DEEPIN_PW_CHECK libdeepin_pw_check.so)

if(CONTRAST_BACKGROUND)
    target_compile_definitions(ds-greeter-lock PRIVATE CONTRAST_BACKGROUND)
endif()

target_include_directories(ds-greeter-lock PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ds-greeter-lock PRIVATE
    Dde::Shell
)

target_sources(ds-greeter-lock PRIVATE
    ${LOCK_SRCS}
    ${QRCS}
)

ds_install_package(PACKAGE org.deepin.ds.greeter.lock TARGET ds-greeter-lock)
