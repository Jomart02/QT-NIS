cmake_minimum_required(VERSION 3.16)

find_package(Qt6 REQUIRED COMPONENTS
    Core Gui Widgets Network
    Quick QuickWidgets
    Multimedia MultimediaWidgets
    Location Positioning
    3DCore 3DRender 3DExtras
    OpenGL OpenGLWidgets
)

set(APP_SOURCES
    main.cpp
    NisApplication.h
    NisApplication.cpp
    MainWindow/MainWindow.h
    MainWindow/MainWindow.cpp
    MainWindow/MainWindow.ui
    GridWidget/GridWidget.h
    GridWidget/GridWidget.cpp
    DockContainer/DockContainer.h
    DockContainer/DockContainer.cpp
    DockContainer/TitleWidget.h
    DockContainer/TitleWidget.cpp
    DockContainer/TitleWidget.ui
    Ribbon/RibbonBar.h
    Ribbon/RibbonBar.cpp
    resources/app.qrc
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow
    ${CMAKE_CURRENT_SOURCE_DIR}/DockContainer
)

add_executable(QT-NIS ${APP_SOURCES})

target_include_directories(QT-NIS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow
    ${CMAKE_CURRENT_SOURCE_DIR}/GridWidget
    ${CMAKE_CURRENT_SOURCE_DIR}/DockContainer
    ${CMAKE_CURRENT_SOURCE_DIR}/Ribbon
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseGridWidgetItem
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseHandlerNaviData
)

target_link_libraries(QT-NIS PRIVATE
    NisCore
    ToolBoxWidget
    QssStyleManager
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Quick
    Qt6::QuickWidgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::Location
    Qt6::Positioning
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)
