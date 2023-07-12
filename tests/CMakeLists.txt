cmake_minimum_required(VERSION 3.20)
project(tests)
add_executable(tests main.cpp scope_guard_tests.cpp)
set_target_properties(tests
    PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)
target_link_libraries(tests PRIVATE scope_guard)
target_include_directories(tests PRIVATE include)

