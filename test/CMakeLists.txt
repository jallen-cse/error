include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v2.13.10)
FetchContent_MakeAvailable(Catch2)

add_executable(jack_test_reason reason.cpp)
add_executable(jack_test_error error.cpp)
target_link_libraries(jack_test_reason PRIVATE error Catch2::Catch2)
target_link_libraries(jack_test_error PRIVATE error Catch2::Catch2)