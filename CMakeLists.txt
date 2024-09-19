cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Cache)

SET(SRC_DIR src)

SET(SRCS_IC ${SRC_DIR}/ideal_cache.cpp)

add_executable(IC ${SRC_DIR}/ICmain.cpp)
target_include_directories(IC PUBLIC inc)