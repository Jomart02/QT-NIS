
function(nis_add_plugin PLUGIN_TARGET)
    cmake_parse_arguments(NIS_PLG "" "" "SOURCES;QRC;DEPS" ${ARGN})

    add_library(${PLUGIN_TARGET} SHARED ${NIS_PLG_SOURCES} ${NIS_PLG_QRC})

    target_include_directories(${PLUGIN_TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(${PLUGIN_TARGET} PRIVATE
        NisCore
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        ${NIS_PLG_DEPS}
    )
endfunction()
