SET(SRC_DIR src)

SET(SRCS ${SRC_DIR}/ideal_cache.cpp)

add_executable(IC ${SRC_DIR}/main.cpp)
target_include_directories(IC PUBLIC inc)

SET(E2E_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/E2E_tests)

#message("E2E_TESTS_DIR: ${E2E_TESTS_DIR}")

file(GLOB E2E_TESTS_FILES "${E2E_TESTS_DIR}/*")

#message("E2E_TESTS_FILES: ${E2E_TESTS_FILES}")

# отобрать уникальные??

foreach(file ${E2E_TESTS_FILES})
    cmake_path(GET file FILENAME base_name)
    add_test(NAME ${file}
        COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./LC.x"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_tests_properties(${file} PROPERTIES DEPENDS LC.x)
endforeach()