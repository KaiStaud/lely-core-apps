cmake_minimum_required(VERSION 3.10)
project(cpp_tutorial LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -O2")

# pkg-config nutzen
find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

add_executable(client master.cpp )
target_include_directories(client PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(client PRIVATE ${LELY_LIBRARIES})
target_compile_options(client PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS client
        RUNTIME DESTINATION bin)

add_executable(device slave.cpp )
target_include_directories(device PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(device PRIVATE ${LELY_LIBRARIES})
target_compile_options(device PRIVATE ${LELY_CFLAGS_OTHER})

install(TARGETS device
        RUNTIME DESTINATION bin)


set (MASTER_YML cpp-tutorial.yml)
add_custom_target(
    configure
    COMMAND dcfgen -r ${MASTER_YML}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running dcfgen to generate DCF files..."
)