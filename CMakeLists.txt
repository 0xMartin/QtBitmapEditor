cmake_minimum_required(VERSION 3.5)

project(QtBitmapEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
        main.cpp
        app_context.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        component/layermanager.h
        component/layermanager.cpp
        component/toolcontroller.h
        component/toolcontroller.cpp
        component/workspace.h
        component/workspace.cpp
        utility/colorpicker.h
        utility/colorpicker.cpp
        objects/layer.h
        objects/layer.cpp
        newproject.h
        newproject.cpp
        newproject.ui
        objects/project.h
        objects/project.cpp
        objects/bitmaplayer.h
        objects/bitmaplayer.cpp
        icons.qrc
        theme.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QtBitmapEditor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET QtBitmapEditor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(QtBitmapEditor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(QtBitmapEditor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(QtBitmapEditor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(QtBitmapEditor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS QtBitmapEditor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QtBitmapEditor)
endif()
