cmake_minimum_required(VERSION 3.5)

project(BDOPSGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        tsharkwrapper.cpp
        tsharkwrapper.h
        PackageData.h
        packagedatatablemodel.h
        packagedatatotalmodel.cpp
        packagedatatotalmodel.h
        packagedatatablemodel.cpp
        mainwindow.ui
        ignorelisttablemodel.h
        ignorelisttablemodel.cpp
        opcodenametablemodel.h
        opcodenametablemodel.cpp
        errorswindow.ui
        errorswindow.h
        errorswindow.cpp
        packagedetailwindow.h
        packagedetailwindow.cpp
        packagedetailwindow.ui
        appsettings.h
        appsettings.cpp
        appsettingswindow.h
        appsettingswindow.cpp
        appsettingswindow.ui
        database.h
        database.cpp
        resources.qrc
        helpers.h
        helpers.cpp
        selectedopcodemodel.h
        selectedopcodemodel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BDOPSGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET BDOPSGUI APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(BDOPSGUI SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(BDOPSGUI
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(BDOPSGUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(BDOPSGUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BDOPSGUI)
endif()