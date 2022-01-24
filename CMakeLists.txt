cmake_minimum_required(VERSION 3.20)
project(redblacktree C)

set(CMAKE_C_STANDARD 99)
add_subdirectory(rbtree)

add_executable(redblacktree main.c)

target_link_libraries(redblacktree PUBLIC rbtree)
