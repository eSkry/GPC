cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME GPC)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
elseif(EXISTS ${CMAKE_BINARY_DIR}/conan-dependencies/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conan-dependencies/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        ui/mainwindow.cpp
        ui/mainwindow.h
        PackageData.h
        packagedatatablemodel.h
        packagedatatotalmodel.cpp
        packagedatatotalmodel.h
        packagedatatablemodel.cpp
        ui/mainwindow.ui
        ignorelisttablemodel.h
        ignorelisttablemodel.cpp
        opcodenametablemodel.h
        opcodenametablemodel.cpp
        ui/packagedetailwindow.h
        ui/packagedetailwindow.cpp
        ui/packagedetailwindow.ui
        appsettings.h
        appsettings.cpp
        ui/appsettingswindow.h
        ui/appsettingswindow.cpp
        ui/appsettingswindow.ui
        resources.qrc
        helpers.h
        helpers.cpp
        selectedopcodemodel.h
        selectedopcodemodel.cpp
        pkgcap/networkdevicelist.h
        pkgcap/networkdevicelist.cpp
        pkgcap/rawdatastorage.h
        pkgcap/rawdatastorage.cpp
        models/capturedpackagesmodel.h
        models/capturedpackagesmodel.cpp
        packagecapture.h
        packagecapture.cpp
        packetlistrecord.h
        packetlistrecord.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    PRIVATE Qt${QT_VERSION_MAJOR}::Sql
    PUBLIC ${CONAN_LIBS})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CONAN_INCLUDE_DIRS}
)

message("Conan include dirs: ${CONAN_INCLUDE_DIRS} ")

target_link_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        PUBLIC ${CONAN_LIB_DIRS}
) # target_link_directories

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
