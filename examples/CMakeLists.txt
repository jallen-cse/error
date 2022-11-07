add_executable(jack_example_0 ex0.cpp)
add_executable(jack_example_1 ex1.cpp)
target_link_libraries(jack_example_0 PRIVATE error)
target_link_libraries(jack_example_1 PRIVATE error)