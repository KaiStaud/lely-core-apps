cmake_minimum_required(VERSION 3.10)
project(cpp_tutorial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O2")
# pkg-config nutzen
find_package(PkgConfig REQUIRED)
set(microlog_DIR ${PROJECT_SOURCE_DIR}/microlog)
find_package(microlog 7.0.0 REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

add_subdirectory(libcia402)
add_subdirectory(callbacks)
add_subdirectory(osal)

add_executable(slave bsp/bsp_can.c sdev.c slave.c)
target_include_directories(slave PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(slave PUBLIC ${LELY_LIBRARIES} cia402 callbacks microlog::microlog)
target_compile_options(slave PRIVATE ${LELY_CFLAGS_OTHER})
target_compile_definitions(
        microlog
        PRIVATE ULOG_BUILD_COLOR=1
        ULOG_BUILD_TOPICS_MODE=ULOG_BUILD_TOPICS_MODE_STATIC
        ULOG_BUILD_TOPICS_STATIC_NUM=4
)

install(TARGETS slave
        RUNTIME DESTINATION bin)

add_executable(masterc bsp/bsp_can.c master_sdev.c master.c)
target_include_directories(masterc PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(masterc PRIVATE ${LELY_LIBRARIES} cia402 callbacks microlog::microlog osal_canopen)
target_compile_options(masterc PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS masterc
        RUNTIME DESTINATION bin)

add_subdirectory(apps)