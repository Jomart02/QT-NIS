include(NisPlugin)

nis_add_plugin(MapViewPlugin
    SOURCES
        MapViewPlugin.h
        MapViewPlugin.cpp
        MapViewPanel.h
        MapViewPanel.cpp
        MapView.h
        MapView.cpp
    QRC
        resources/map.qrc
    DEPS
        Qt6::Quick
        Qt6::Location
        Qt6::Positioning
        Qt6::QuickWidgets
)
