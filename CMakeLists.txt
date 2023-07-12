cmake_minimum_required(VERSION 3.20)
project(scope_guard VERSION 0.0.1
                    DESCRIPTION "RAII Assistance for scoped cleanup"
                    LANGUAGES CXX)

include(GNUInstallDirs)
add_library(scope_guard INTERFACE)

target_include_directories(scope_guard INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                                 $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>) 
target_compile_features(scope_guard INTERFACE cxx_std_11) 

add_subdirectory(tests)
