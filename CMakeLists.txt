cmake_minimum_required(VERSION 3.8)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(FindGettext)
project(display_control)

set(CMAKE_CXX_STANDARD 11)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/cmake_config.h)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DHAVE_CMAKE_CONFIG_H)

set(SOURCE_FILES
        src/display-control.cpp
        src/display-control.h
        src/gettext.h
        src/gettext_defs.h
        src/display.cpp src/display.h
        src/object-guard.h)
add_executable(display_control ${SOURCE_FILES})

check_include_files(getopt.h HAVE_GETOPT_H)

if (HAVE_GETOPT_H)
    check_symbol_exists(getopt_long getopt.h HAVE_GETOPT_LONG)
endif (HAVE_GETOPT_H)

if (APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)
    target_link_libraries(display_control ${IOKIT_LIBRARY} ${APPLICATION_SERVICES_LIBRARY})
else ()
    message(FATAL_ERROR "Unsupported operating system ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
endif (APPLE)
