cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME QT-NIS)
project(${PROJECT_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/MainWindow/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/GridWidget/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/ToolWidgetBase/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/ShipObject/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/NavigationInfo/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/DockContainer/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/BaseSettingsWindow/)

add_subdirectory(./ToolBoxWidget)
add_subdirectory(./internal/NavyComponents)
add_subdirectory(./internal/QssStyleManager)

set(HDRS
    ./include/globalVar.h
    ./include/MainWindow/MainWindow.h
    ./include/GridWidget/GridWidget.h
    ./include/ToolWidgetBase/ToolWidgetBase.h
    ./include/ShipObject/ShipObject.h
    ./include/ShipObject/ShipObjectTool.h
    ./include/NavigationInfo/NavigationInfo.h
    ./include/NavigationInfo/NavigationInfoTool.h
    ./include/NavigationInfo/SettingsNavigation.h
    ./include/DockContainer/DockContainer.h
    ./include/DockContainer/TitleWidget.h
    ./include/BaseSettingsWindow/BaseSettingsWindow.h
)

set(SRCS
    ./src/main.cpp
    ./src/MainWindow/MainWindow.cpp
    ./src/GridWidget/GridWidget.cpp
    ./src/ToolWidgetBase/ToolWidgetBase.cpp
    ./src/ShipObject/ShipObject.cpp
    ./src/ShipObject/ShipObjectTool.cpp
    ./src/NavigationInfo/NavigationInfo.cpp
    ./src/NavigationInfo/NavigationInfoTool.cpp
    ./src/NavigationInfo/SettingsNavigation.cpp
    ./src/DockContainer/DockContainer.cpp
    ./src/DockContainer/TitleWidget.cpp
    ./src/BaseSettingsWindow/BaseSettingsWindow.cpp
)

set(QRCS
    ./resources/res.qrc
)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ui/MainWindow/
    ui/ShipObject/
    ui/NavigationInfo/
    ui/DockContainer/
)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui 3DCore 3DRender 3DExtras OpenGL OpenGLWidgets)
# Find OpenGL and GLU
find_package(OpenGL REQUIRED)
find_library(GLU_LIBRARY NAMES GLU glu32)

find_package(Qt6 COMPONENTS Widgets Network Quick Multimedia MultimediaWidgets REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${QRCS})


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Quick
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ToolBoxWidget
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${GLU_LIBRARY}
    QssStyleManager
    CircularValueWidgets
)