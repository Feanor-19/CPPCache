SET(SRC_DIR src)

add_library(Utilities ${SRC_DIR}/utilities.cpp)
target_include_directories(Utilities PUBLIC inc)