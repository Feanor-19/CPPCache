cmake_minimum_required(VERSION 3.11)
project(Cache)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Utilities)
add_subdirectory(IdealCache)
add_subdirectory(LFUCache)