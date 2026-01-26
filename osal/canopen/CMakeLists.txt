pkg_check_modules(LELY REQUIRED liblely-coapp)
add_library(osal_canopen csdo.c)
target_include_directories(osal_canopen PRIVATE ${LELY_INCLUDE_DIRS})
target_link_libraries(osal_canopen PUBLIC ${LELY_LIBRARIES})