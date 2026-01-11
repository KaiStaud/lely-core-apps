cmake_minimum_required(VERSION 3.14)
project(libcia402 LANGUAGES C CXX)

add_subdirectory(src)

# Tests (nur bauen, wenn GTest vorhanden oder gewünscht)
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
#    add_subdirectory(test)
endif()
