cmake_minimum_required(VERSION 3.11)

project(Cache)

SET(SRC_DIR src)

SET(SRCS_IC ${SRC_DIR}/ideal_cache.cpp)

add_library(iclib STATIC ${SRCS_IC})
target_include_directories(iclib PUBLIC inc)

add_executable(IC ${SRC_DIR}/ICmain.cpp)
target_include_directories(IC PUBLIC inc)
target_link_libraries(IC iclib)