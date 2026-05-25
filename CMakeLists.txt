cmake_minimum_required(VERSION 3.16)

project(QT-NIS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Concurrent Quick QuickWidgets OpenGLWidgets Location)

include(version)

add_subdirectory(ToolBoxWidget)
add_subdirectory(internal/NavyComponents)
add_subdirectory(internal/QssStyleManager)

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(plugins)
add_subdirectory(tests)
