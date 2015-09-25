cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE CPPFILES *.cpp)

add_executable(kaleidoscope-test
    ${CPPFILES})

target_link_libraries(kaleidoscope-test
    pthread
    kaleido-lib
    ${Boost_LIBRARIES}
    )

add_test(
    NAME kaleidoscope
    COMMAND $<TARGET_FILE:kaleidoscope-test>
    )

