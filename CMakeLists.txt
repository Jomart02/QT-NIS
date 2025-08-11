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
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/VideoInput/)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/WebcamViewer/)

add_subdirectory(./ToolBoxWidget)

set(HDRS
    ./include/globalVar.h
    ./include/MainWindow/MainWindow.h
    ./include/GridWidget/GridWidget.h
    ./include/ToolWidgetBase/ToolWidgetBase.h
    ./include/VideoInput/VideoInput.h
    ./include/WebcamViewer/WebcamViewer.h
)

set(SRCS
    ./src/main.cpp
    ./src/MainWindow/MainWindow.cpp
    ./src/GridWidget/GridWidget.cpp
    ./src/ToolWidgetBase/ToolWidgetBase.cpp
    ./src/VideoInput/VideoInput.cpp
    ./src/WebcamViewer/WebcamViewer.cpp
)

set(QRCS

)

set(CMAKE_AUTOUIC_SEARCH_PATHS 
    ui/MainWindow/
    ui/VideoInput/
)

find_package(Qt6 COMPONENTS Widgets Network Quick Multimedia MultimediaWidgets REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS} ${QRCS} ${HDRS} ${QRCS})


target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Quick
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    ToolBoxWidget
)

find_package(PkgConfig REQUIRED)
    pkg_check_modules(FFMPEG IMPORTED_TARGET libavcodec libavformat libavutil libswscale libavdevice)
if (FFMPEG_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::FFMPEG)
else()
    message(WARNING "FFmpeg not found. Qt Multimedia may not work correctly.")
endif()