cmake_minimum_required(VERSION 3.10)
project(cpp_tutorial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O2")

# pkg-config nutzen
find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

add_subdirectory(libcia402)
# Slave in C
add_executable(slave bsp/bsp_can.c sdev.c ulog/ulog.c slave.c)
target_include_directories(slave PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(slave PRIVATE ${LELY_LIBRARIES} cia402)
target_compile_options(slave PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS slave
        RUNTIME DESTINATION bin)

add_subdirectory(apps)