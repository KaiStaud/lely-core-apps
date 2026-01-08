cmake_minimum_required(VERSION 3.10)
project(cpp_tutorial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O2")

# pkg-config nutzen
find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

add_subdirectory(libcia402)
add_subdirectory(callbacks)
# Slave in C
add_executable(slave bsp/bsp_can.c sdev.c ulog/ulog.c slave.c)
target_include_directories(slave PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(slave PRIVATE ${LELY_LIBRARIES} cia402 callbacks)
target_compile_options(slave PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS slave
        RUNTIME DESTINATION bin)


# Slave in C
add_executable(masterc bsp/bsp_can.c master_sdev.c ulog/ulog.c master.c)
target_include_directories(masterc PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(masterc PRIVATE ${LELY_LIBRARIES} cia402 callbacks)
target_compile_options(masterc PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS masterc
        RUNTIME DESTINATION bin)

add_subdirectory(apps)