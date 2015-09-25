cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE CPPFILES *.cpp)

add_library(kaleido-lib STATIC
        ${CPPFILES})

target_link_libraries(kaleido-lib
    ${Boost_LIBRARY})

