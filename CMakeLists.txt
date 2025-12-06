cmake_minimum_required(VERSION 3.10)
project(cpp_tutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O2")

# pkg-config nutzen
find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

# Master
add_executable(master master.cpp)
target_include_directories(master PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(master PRIVATE ${LELY_LIBRARIES})
target_compile_options(master PRIVATE ${LELY_CFLAGS_OTHER})

# Slave
add_executable(slave slave.cpp)
target_include_directories(slave PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(slave PRIVATE ${LELY_LIBRARIES})
target_compile_options(slave PRIVATE ${LELY_CFLAGS_OTHER})
