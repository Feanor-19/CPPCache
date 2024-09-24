SET(SRC_DIR src)

SET(SRCS ${SRC_DIR}/ideal_cache.cpp)

add_executable(IC ${SRC_DIR}/main.cpp)
target_include_directories(IC PUBLIC inc)

SET(E2E_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/E2E_tests)

#message("E2E_TESTS_DIR: ${E2E_TESTS_DIR}")

file(GLOB E2E_TESTS_FILES "${E2E_TESTS_DIR}/*.in")

#message("E2E_TESTS_FILES: ${E2E_TESTS_FILES}")

foreach(file ${E2E_TESTS_FILES})
    cmake_path(GET file STEM test_stem)
    message("arg : ${E2E_TESTS_DIR}/${test_stem}")
    message("curr bin dir: ${CMAKE_CURRENT_BINARY_DIR}")
    add_test(NAME ${test_stem}
        COMMAND bash -c "${CMAKE_CURRENT_SOURCE_DIR}/runtest.sh $<TARGET_FILE:IC> ${E2E_TESTS_DIR}/${test_stem}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(${test_stem} PROPERTIES DEPENDS IC)
endforeach()