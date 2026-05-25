
find_package(Git QUIET)

if(Git_FOUND OR GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --format=%ci
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_COMMIT_TIME
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(GIT_COMMIT_HASH "unknown")
    set(GIT_COMMIT_TIME "unknown")
endif()

if(NOT GIT_COMMIT_HASH)
    set(GIT_COMMIT_HASH "unknown")
endif()
if(NOT GIT_COMMIT_TIME)
    set(GIT_COMMIT_TIME "unknown")
endif()

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated")

configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/AppVersion.h.in"
    "${CMAKE_BINARY_DIR}/generated/AppVersion.h"
    @ONLY
)

message(STATUS "Version ${PROJECT_VERSION}  commit ${GIT_COMMIT_HASH}  time ${GIT_COMMIT_TIME}")
