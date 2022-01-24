add_library(rbtree include/rbtree/rbtree.h src/rbtree/rbtree.c)
target_include_directories(rbtree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)