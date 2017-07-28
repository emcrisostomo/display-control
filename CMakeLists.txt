cmake_minimum_required(VERSION 3.8)
project(display_control)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES display-control.cpp gettext.h)
add_executable(display_control ${SOURCE_FILES})

if (APPLE)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)
    target_link_libraries(display_control ${IOKIT_LIBRARY} ${APPLICATION_SERVICES_LIBRARY})
else ()
    message(FATAL_ERROR "Unsupported operating system ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
endif (APPLE)
