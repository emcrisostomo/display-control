cmake_minimum_required(VERSION 3.8)
# Add DESCRIPTION when migrating to CMake 3.9
project(display-control VERSION 1.1.0)

#@formatter:off
set(PACKAGE           "display-control")
set(PACKAGE_NAME      "${PACKAGE}")
set(PACKAGE_VERSION   "${PROJECT_VERSION}-develop")
set(PACKAGE_STRING    "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set(PACKAGE_AUTHOR    "enrico.m.crisostomo@gmail.com")
set(PACKAGE_BUGREPORT "${PACKAGE_AUTHOR}")
set(PACKAGE_TARNAME   "${PACKAGE}")
set(PACKAGE_URL       "https://github.com/emcrisostomo/display-control")
set(LOCALEDIR         "${CMAKE_INSTALL_PREFIX}/share/locale" CACHE FILEPATH "locale dir")
#@formatter:on

if (NOT APPLE)
    message(FATAL_ERROR "Unsupported operating system ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION}")
endif ()

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES
        src/display_control.cpp
        src/display_control.h
        src/gettext.h
        src/gettext_defs.h
        src/display.cpp src/display.h
        src/object_guard.h
        src/display_user_settings.cpp src/display_user_settings.h)
add_executable(display-control ${SOURCE_FILES})

include(CheckIncludeFileCXX)
include(CheckSymbolExists)
include(FindGettext)
include(FindIntl)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${Intl_INCLUDE_DIRS}")

add_definitions(-DHAVE_CMAKE_CONFIG_H)
check_include_file_cxx(display-control_config.h HAVE_CONFIG_H)

# platform checks
check_include_file_cxx(getopt.h HAVE_GETOPT_H)

if (HAVE_GETOPT_H)
    check_symbol_exists(getopt_long getopt.h HAVE_GETOPT_LONG)
endif (HAVE_GETOPT_H)

# gettext and intl
if (Intl_FOUND AND GETTEXT_FOUND)
    option(USE_NLS "Use NLS" ON)
endif ()

if (USE_NLS)
    set(ENABLE_NLS 1)

    if (Intl_LIBRARIES)
        target_link_libraries(display-control ${Intl_LIBRARIES})
    endif ()

    # gettext configuration
    set(LINGUAS en en@quot en@boldquot it es)

    foreach (language ${LINGUAS})
        configure_file(${PROJECT_SOURCE_DIR}/po/${language}.po ${PROJECT_BINARY_DIR}/${language}.po COPYONLY)
    endforeach ()

    gettext_process_pot_file(po/display-control.pot ALL INSTALL_DESTINATION share/locale LANGUAGES ${LINGUAS})
endif ()

# check for required frameworks
find_library(IOKIT_LIBRARY IOKit)
find_library(APPLICATION_SERVICES_LIBRARY ApplicationServices)

if (NOT IOKIT_LIBRARY)
    message(FATAL_ERROR "Required library IOKit is missing")
endif ()

if (NOT APPLICATION_SERVICES_LIBRARY)
    message(FATAL_ERROR "Required library ApplicationServices is missing")
endif ()

target_link_libraries(display-control ${IOKIT_LIBRARY} ${APPLICATION_SERVICES_LIBRARY})

configure_file(${PROJECT_SOURCE_DIR}/cmake_config.h.in ${PROJECT_BINARY_DIR}/cmake_config.h)

# install targets
install(TARGETS display-control DESTINATION bin)
install(FILES ${PROJECT_BINARY_DIR}/cmake_config.h DESTINATION include)

# tests
include(CTest)

add_test(displayControlRuns display-control)

add_test(displayControlHelp display-control --help)
set_tests_properties(displayControlHelp PROPERTIES PASS_REGULAR_EXPRESSION "${PACKAGE} ${PACKAGE_VERSION}")

# installer
include(InstallRequiredSystemLibraries)
#@formatter:off
# Remove and default to project description when migrated to CMake 3.9
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A macOS utility to enumerate displays and store and set their brightness level")
set(CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR              "${PACKAGE_AUTHOR}")
set(CPACK_RESOURCE_FILE_LICENSE       "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_GENERATOR                   "STGZ;TGZ")
#@formatter:on
include(CPack)
