include(NisPlugin)

nis_add_plugin(NavigationInfoPlugin
    SOURCES
        NavigationInfoPlugin.h
        NavigationInfoPlugin.cpp
        NavigationInfoPanel.h
        NavigationInfoPanel.cpp
        NavigationInfo.h
        NavigationInfo.cpp
        SettingsNavigation.h
        SettingsNavigation.cpp
    DEPS
        CircularValueWidgets
)

set_target_properties(NavigationInfoPlugin PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}"
)
