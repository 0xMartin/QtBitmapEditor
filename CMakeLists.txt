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
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS PrintSupport)


# TOOL
set(QBE_TOOL
    tool/pencil.h
    tool/pencil.cpp
    tool/eraser.h
    tool/eraser.cpp
    tool/brush.h
    tool/brush.cpp
    tool/fillcolor.h
    tool/fillcolor.cpp
    tool/eyedropper.h
    tool/eyedropper.cpp
    tool/text.h
    tool/text.cpp
)

# LAYER
set(QBE_LAYER
    layer/bitmaplayer.h
    layer/bitmaplayer.cpp
    layer/textlayer.h
    layer/textlayer.cpp
    layer/imagelayer.h
    layer/imagelayer.cpp
)

# UTILITY
set(QBE_UTILITY
    utility/mouseeventhelper.h
    utility/mouseeventhelper.cpp
    utility/colorpicker.h
    utility/colorpicker.cpp
    utility/gradientwidget.h
    utility/gradientwidget.cpp
    utility/coloritem.h
    utility/coloritem.cpp
    utility/fontselector.h
    utility/fontselector.cpp
)

# UI
set(QBE_UI
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    projectwizard.h
    projectwizard.cpp
    projectwizard.ui
    newproject.h
    newproject.cpp
    newproject.ui
    openproject.h
    openproject.cpp
    openproject.ui
    projectfromimage.h
    projectfromimage.cpp
    projectfromimage.ui
    importimage.h
    importimage.cpp
    importimage.ui
    exportproject.h
    exportproject.cpp
    exportproject.ui
    about.h
    about.cpp
    about.ui
)

# RESOURCES
set(QBE_RES
    icons.qrc
    theme.qrc
    splash.qrc
)

# BASE
set(QBE_BASE
    base/app_context.h
    base/app_context.cpp
    base/module.h
    base/config.h
    base/dialogs.h
    base/project.h
    base/project.cpp
    base/layer.h
    base/layer.cpp
    base/layermanager.h
    base/layermanager.cpp
    base/toolcontroller.h
    base/toolcontroller.cpp
    base/workspace.h
    base/workspace.cpp
    base/tool.h
    base/tool.cpp
)

# PROJECT
set(PROJECT_SOURCES
    main.cpp
    ${QBE_BASE}
    ${QBE_LAYER}
    ${QBE_TOOL}
    ${QBE_UTILITY}
    ${QBE_UI}
    ${QBE_RES}
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
target_link_libraries(QtBitmapEditor PRIVATE Qt${QT_VERSION_MAJOR}::PrintSupport)

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
