cmake_minimum_required(VERSION 2.8)
project(points-runtime)

option(USE_MUTEX "Compile pool with mutex lock" OFF)
option(USE_PTHREAD "Compile pool with mutex lock" ON)

message(STATUS "USE_MUTEX:" ${USE_MUTEX})
message(STATUS "USE_PTHREAD:" ${USE_PTHREAD})

include_directories(.)

add_library(points-runtime pr_pool_blocklist.c pr_countdownlatch.c)

if(USE_PTHREAD)
    target_sources(points-runtime PRIVATE
            unix/pr_mutex.c
            unix/pr_condition.c)
else()
    target_sources(points-runtime PRIVATE
            windows/pr_mutex.c
            windows/pr_condition.c)
endif()

if(USE_MUTEX)
    target_sources(points-runtime PRIVATE pr_mutexpool.c)
else()
    target_sources(points-runtime PRIVATE pr_pool.c)
endif()

###############################################################################
# TEST
add_executable(pr_mutex.t pr_mutex.t.c)
target_link_libraries(pr_mutex.t points-runtime pthread)

add_executable(pr_pool.t pr_pool.t.c)
target_link_libraries(pr_pool.t points-runtime pthread)

add_executable(pr_countdownlatch.t pr_countdownlatch.t.c)
target_link_libraries(pr_countdownlatch.t points-runtime pthread)
