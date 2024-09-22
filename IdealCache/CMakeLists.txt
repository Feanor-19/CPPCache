SET(SRC_DIR src)

SET(SRCS ${SRC_DIR}/ideal_cache.cpp)

add_executable(IC ${SRC_DIR}/main.cpp)
target_include_directories(IC PUBLIC inc)
