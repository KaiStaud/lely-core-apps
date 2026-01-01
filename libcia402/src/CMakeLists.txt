add_library(cia402
    statemachine.c
    homing.c
    digital_inputs.c
)

target_include_directories(cia402
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
