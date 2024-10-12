SET(SRC_DIR src)

add_executable(LFUC ${SRC_DIR}/main.cpp)
target_include_directories(LFUC PUBLIC inc)


# Benchmarking using google benchmark

find_package(benchmark REQUIRED)

add_executable(LFUC_benchmark ${SRC_DIR}/benchmark.cpp)
target_include_directories(LFUC_benchmark PUBLIC inc)
target_link_libraries(LFUC_benchmark benchmark::benchmark)


# E2E tests
enable_testing()

SET(E2E_TESTS_DIR ${CMAKE_SOURCE_DIR}/E2E_tests)

file(GLOB E2E_TESTS_FILES "${E2E_TESTS_DIR}/*.in")

foreach(file ${E2E_TESTS_FILES})
    cmake_path(GET file STEM test_stem)
    SET(test_name LFUC_${test_stem})
    add_test(NAME ${test_name}
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/scripts/runE2Etest.sh $<TARGET_FILE:LFUC> ${E2E_TESTS_DIR}/${test_stem}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()