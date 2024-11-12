# SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

find_package(DDEShell REQUIRED)

set(GREETER_SOURCES
    greeterproxy.h
    greeterproxy.cpp
    usermodel.h
    usermodel.cpp
    user.h
    user.cpp
    logoprovider.h
    logoprovider.cpp
)

find_package(Dtk6 REQUIRED COMPONENTS SystemSettings Declarative)
find_package(DDM REQUIRED COMPONENTS Common Auth)

set_source_files_properties("interfaces/org.freedesktop.DisplayManager.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplayManager
)
set_source_files_properties("interfaces/org.freedesktop.DisplayManager.Seat.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplaySeat
)

set_source_files_properties("interfaces/org.freedesktop.DisplayManager.Session.xml" PROPERTIES
    NO_NAMESPACE ON
    CLASSNAME DisplaySession
)

qt_add_dbus_interface(DDM_DBUS_SOURCES "interfaces/org.freedesktop.DisplayManager.xml"          DisplayManager)
qt_add_dbus_interface(DDM_DBUS_SOURCES "interfaces/org.freedesktop.DisplayManager.Seat.xml"     DisplayManagerSeat)
qt_add_dbus_interface(DDM_DBUS_SOURCES "interfaces/org.freedesktop.DisplayManager.Session.xml"  DisplayManagerSession)

set(QML_FILES
    qml/Greeter.qml
    qml/UserList.qml
    qml/UserInput.qml
    qml/UserView.qml
    qml/LoginAnimation.qml
    qml/RoundBlur.qml
    qml/TimeDateWidget.qml
    qml/HintLabel.qml
    qml/QuickAction.qml
    qml/ControlAction.qml
)

foreach(QML_FILE ${QML_FILES})
    get_filename_component(file_name ${QML_FILE} NAME)
    set_source_files_properties(${QML_FILE}
        PROPERTIES QT_RESOURCE_ALIAS ${file_name}
    )
endforeach()

qt_add_qml_module(ds-greeter
    URI org.deepin.greeter
    VERSION 1.0
    SOURCES
        greeterapp.cpp
        greeterapp.h
        dbusfrontservice.cpp
        dbusfrontservice.h
        ${GREETER_SOURCES}
        ${DDM_DBUS_SOURCES}
    QML_FILES ${QML_FILES}
    OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/plugins/org/deepin/greeter
)

qt_add_resources(ds-greeter "greeter_assets"
    PREFIX "/dsg"
    FILES
        icons/login_capslock.dci
        icons/login_display_password.dci
        icons/login_hidden_password.dci
        icons/login_keyboard.dci
        icons/login_lock.dci
        icons/login_open.dci
        icons/login_power.dci
        icons/login_reboot.dci
        icons/login_sleep.dci
        icons/login_user.dci
        icons/login_hint.dci
        icons/login_logout.dci
        icons/login_poweroff.dci
        icons/login_suspend.dci
        icons/backToLightdm.dci
        icons/logo.svg
)

target_include_directories(ds-greeter PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_sources(ds-greeter PRIVATE
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.h
    ${CMAKE_SOURCE_DIR}/greeterauthinterface.cpp
)

target_link_libraries(ds-greeter PRIVATE
    Dde::Shell
    DDM::Auth
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Qml
    Dtk6::Declarative
    Dtk6::SystemSettings
)

install(DIRECTORY "${PROJECT_BINARY_DIR}/plugins/org/deepin/greeter" DESTINATION "${QML_INSTALL_DIR}/org/deepin")

ds_install_package(PACKAGE org.deepin.ds.greeter TARGET ds-greeter)
# ds_handle_package_translation(PACKAGE org.deepin.ds.greeter)
