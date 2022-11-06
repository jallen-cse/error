cmake_minimum_required(VERSION 3.11) # fetch content from 3.11

project(jack-error
    VERSION 0.0.0
    LANGUAGES CXX
    DESCRIPTION "Basic types for representing erroneous circumstances in modern C++."
    HOMEPAGE_URL https://github.com/jallen-cse/error)

option(JACK_ERROR_BUILD_TESTS "Build test executables" ON)
option(JACK_ERROR_BUILD_EXAMPLE "Build example executable" ON)

add_library(error INTERFACE)
target_include_directories(error INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

if (JACK_ERROR_BUILD_TESTS)
    add_subdirectory(test)
endif()

if (JACK_ERROR_BUILD_EXAMPLE)
    add_subdirectory(examples)
endif()
