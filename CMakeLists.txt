cmake_minimum_required(VERSION 3.10)

set(DS_VERSION "0.0.1" CACHE STRING "Define project version")
project(ddm-greeter
    VERSION "${DS_VERSION}"
    DESCRIPTION "ddm-greeter"
    HOMEPAGE_URL "https://github.com/linuxdeepin/ddm-greeter"
    LANGUAGES CXX C
)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(QT_VERSION_MAJOR 6)
set(DTK_VERSION_MAJOR 6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}" CACHE STRING "Library install path")
set(QML_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml" CACHE STRING "Qml plugin install directory")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH};${ECM_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Qml Quick Gui QuickControls2 LinguistTools)
find_package(DDEShell REQUIRED)
find_package(Dtk${DTK_VERSION_MAJOR} REQUIRED COMPONENTS SystemSettings Declarative)

qt_add_executable(ddm-greeter
    main.cpp
    appletloader.h
    appletloader.cpp
)

target_link_libraries(ddm-greeter PRIVATE
    Dde::Shell
    Qt${QT_VERSION_MAJOR}::Gui
    Dtk${DTK_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::DBus
)

install(TARGETS ddm-greeter DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

add_subdirectory(main)
add_subdirectory(auth)
add_subdirectory(ddm-auth)
add_subdirectory(lock)
add_subdirectory(da-auth)
