cmake_minimum_required(VERSION 3.8)
project(display_control)

set(PACKAGE           "display-control")
set(PACKAGE_NAME      "${PACKAGE}")
set(PACKAGE_VERSION   "1.1.0-develop")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "enrico.m.crisostomo@gmail.com")
set(PACKAGE_TARNAME   "${PACKAGE}")
set(PACKAGE_URL       "https://github.com/emcrisostomo/display-control")

if (NOT APPLE)
    message(FATAL_ERROR "Unsupported operating system ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
endif ()

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        src/display-control.cpp
        src/display-control.h
        src/gettext.h
        src/gettext_defs.h
        src/display.cpp src/display.h
        src/object-guard.h)
add_executable(display_control ${SOURCE_FILES})

include(CheckIncludeFileCXX)
include(CheckSymbolExists)
include(FindGettext)
include(FindIntl)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${Intl_INCLUDE_DIRS}")

add_definitions(-DHAVE_CMAKE_CONFIG_H)
# FIXME
add_definitions(-DLOCALEDIR="FIXME")
check_include_file_cxx(display_control_config.h HAVE_CONFIG_H)

check_include_file_cxx(getopt.h HAVE_GETOPT_H)

if (HAVE_GETOPT_H)
    check_symbol_exists(getopt_long getopt.h HAVE_GETOPT_LONG)
endif (HAVE_GETOPT_H)

if (Intl_FOUND)
    set(ENABLE_NLS 1)

    if (Intl_LIBRARIES)
        target_link_libraries(display_control ${Intl_LIBRARIES})
    endif()
endif()


if (APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)

    if (NOT IOKIT_LIBRARY)
        message(FATAL_ERROR "Required library IOKit is missing")
    endif()

    if (NOT APPLICATION_SERVICES_LIBRARY)
        message(FATAL_ERROR "Required library ApplicationServices is missing")
    endif()

    target_link_libraries(display_control ${IOKIT_LIBRARY} ${APPLICATION_SERVICES_LIBRARY})
endif (APPLE)

configure_file(${PROJECT_SOURCE_DIR}/cmake_config.h.in ${PROJECT_BINARY_DIR}/cmake_config.h)
