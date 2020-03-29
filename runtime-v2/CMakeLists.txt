cmake_minimum_required(VERSION 2.8)
project(points-runtime-v2)

option(USE_PTHREAD "Compile pool with mutex lock" OFF)

message(STATUS "USE_PTHREAD:" ${USE_PTHREAD})

include_directories(.)

add_library(points-runtime-v2
        pr_debug.c
        pr_pool_blocklist.c
        pr_pool.c
        pr_mutex.c
        pr_condition.c
        pr_countdownlatch.c
        pr_thread_attr.c
        pr_thread.c
        pr_thread_local.c
        pr_scopeguard.c
        pr_future.c
        pr_array.c
        pr_vector.c
        pr_deque.c
        pr_ptrbag.c
        pr_thread_queue.c
        pr_thread_pool.c
        )

if(USE_PTHREAD)
    target_compile_definitions(points-runtime-v2 PRIVATE WITH_PTHREAD)
endif(USE_PTHREAD)


###############################################################################
add_executable(pr_pool_blocklist.t pr_pool_blocklist.t.c)
target_link_libraries(pr_pool_blocklist.t points-runtime-v2)

add_executable(pr_pool.t pr_pool.t.c)
target_link_libraries(pr_pool.t points-runtime-v2 pthread)

add_executable(pr_mutex.t pr_mutex.t.c)
target_link_libraries(pr_mutex.t points-runtime-v2 pthread)

add_executable(pr_countdownlatch.t pr_countdownlatch.t.c)
target_link_libraries(pr_countdownlatch.t points-runtime-v2 pthread)

add_executable(pr_thread.t pr_thread.t.c)
target_link_libraries(pr_thread.t points-runtime-v2 pthread)

add_executable(pr_thread_local.t pr_thread_local.t.c)
target_link_libraries(pr_thread_local.t points-runtime-v2 pthread)

add_executable(pr_array.t pr_array.t.c)
target_link_libraries(pr_array.t points-runtime-v2 pthread)

add_executable(pr_deque.t pr_deque.t.c)
target_link_libraries(pr_deque.t points-runtime-v2 pthread)

add_executable(pr_ptrbag.t pr_ptrbag.t.c)
target_link_libraries(pr_ptrbag.t points-runtime-v2 pthread)

add_executable(pr_thread_pool.t pr_thread_pool.t.c)
target_link_libraries(pr_thread_pool.t points-runtime-v2 pthread)

add_executable(pr_future.t pr_future.t.c)
target_link_libraries(pr_future.t points-runtime-v2 pthread)

add_executable(pr_vector.t pr_vector.t.c)
target_link_libraries(pr_vector.t points-runtime-v2 pthread)

add_executable(pr_debug.t pr_debug.t.c)
target_link_libraries(pr_debug.t points-runtime-v2 pthread)

add_executable(pr_thread_queue.t pr_thread_queue.t.c)
target_link_libraries(pr_thread_queue.t points-runtime-v2 pthread)
