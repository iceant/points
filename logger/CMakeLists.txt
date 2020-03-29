cmake_minimum_required(VERSION 2.8)
project(points-logger)

include_directories(.)
include_directories(../runtime-v2)

add_library(points-logger
        logger_buffer.c
        logger_buffervector.c
        logger.c
        )

##############################################################################
add_executable(logger_buffer.t logger_buffer.t.c)
target_link_libraries(logger_buffer.t points-logger points-runtime-v2)

add_executable(logger_buffervector.t logger_buffervector.t.c)
target_link_libraries(logger_buffervector.t points-logger points-runtime-v2)

add_executable(logger.t logger.t.c)
target_link_libraries(logger.t points-logger points-runtime-v2)
