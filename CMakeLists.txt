cmake_minimum_required(VERSION 3.11) # fetch content from 3.11

project(jack-error
    VERSION 0.0.0
    LANGUAGES CXX
    DESCRIPTION "Basic types for representing erroneous circumstances in modern C++."
    HOMEPAGE_URL https://github.com/jallen-cse/error)

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

option(JACK_ERROR_BUILD_TESTS "Build test executables" OFF)
option(JACK_ERROR_BUILD_EXAMPLES "Build example executables" OFF)

add_library(error INTERFACE)
target_include_directories(error INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (JACK_ERROR_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JACK_ERROR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
