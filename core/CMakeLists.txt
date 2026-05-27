cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent)

add_library(NisCore SHARED
    AppEvent.h
    AppContext.h
    AppContext.cpp

    plugin/IPlugin.h
    plugin/PluginManager.h
    plugin/PluginManager.cpp

    logger/Logger.h
    logger/Logger.cpp

    settings/BaseSettingsWindow.h
    settings/BaseSettingsWindow.cpp

    dirs/DirectoryManager.h
    dirs/DirectoryManager.cpp


    ui/IUiHost.h
    ui/ToolWidgetBase.h
    ui/ToolWidgetBase.cpp
    ui/RibbonGroup.h
    ui/RibbonGroup.cpp
)

target_include_directories(NisCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/plugin
    ${CMAKE_CURRENT_SOURCE_DIR}/logger
    ${CMAKE_CURRENT_SOURCE_DIR}/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_BINARY_DIR}/generated
)

target_link_libraries(NisCore PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Concurrent
)
