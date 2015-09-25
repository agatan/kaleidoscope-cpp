cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE CPPFILES *.cpp)

add_executable(kaleidoscope-test
    ${CPPFILES})

target_link_libraries(kaleidoscope-test
    gtest
    gtest_main
    pthread
    kaleido-lib
    )

add_test(
    NAME kaleidoscope
    COMMAND $<TARGET_FILE:kaleidoscope-test>
    )

