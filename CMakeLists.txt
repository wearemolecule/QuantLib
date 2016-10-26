cmake_minimum_required()
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0048 NEW)
project(QuantLib VERSION 1.9.0)

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)
option(DISABLE_SHARED "Do not build shared library" OFF)
# to reference headers via <ql/foo.hpp>, we need to add the root
# directory of the project to includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Boost)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

if (BUILD_EXAMPLES)
    add_subdirectory(Examples)
endif ()
add_subdirectory(ql)

# Building the test suite might require using shared libraries
# for Boost.Test
option(USE_BOOST_DYNAMIC_LIBRARIES
       "Use the shared version of Boost.Test" OFF)
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test-suite)
endif ()

execute_process(
    COMMAND dpkg --print-architecture
    OUTPUT_VARIABLE DPKG_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE)

macro(set_ifndef variable value)
    if(NOT DEFINED ${variable})
        set(${variable} ${value})
    endif()
endmacro()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "Krzysztof Woś <krzysztof.wos@molecule.io>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Quantitative Finance Library")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
if(DEFINED ENV{CI_BUILD_ID})
    set(CPACK_PACKAGE_VERSION_PATCH "${CPACK_PACKAGE_VERSION_PATCH}~$ENV{CI_BUILD_ID}")
    set(PROJECT_VERSION "${PROJECT_VERSION}~$ENV{CI_BUILD_ID}")
endif()
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${PROJECT_VERSION}_${DPKG_ARCHITECTURE}")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://www.molecule.io")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
