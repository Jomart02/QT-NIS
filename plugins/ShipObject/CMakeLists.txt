include(NisPlugin)

nis_add_plugin(ShipObjectPlugin
    SOURCES
        ShipObjectPlugin.h
        ShipObjectPlugin.cpp
        ShipPanel.h
        ShipPanel.cpp
        ShipObjectControls.h
        ShipObjectControls.cpp
        ShipObject.h
        ShipObject.cpp
    QRC
        resources/ship.qrc
)


find_package(OpenGL REQUIRED)
find_library(GLU_LIBRARY NAMES GLU glu32)

target_link_libraries(ShipObjectPlugin PRIVATE
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    ${OPENGL_LIBRARIES}
    ${GLU_LIBRARY}
)

set_target_properties(ShipObjectPlugin PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
)
