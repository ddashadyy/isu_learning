cmake_minimum_required(VERSION 3.13)  
project(sequence_test)               

set(CMAKE_CXX_STANDARD 17)            
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})


set(HEADER_FILES test.hpp parametrized_test.hpp)
set(SOURCE_FILES main.cpp test.cpp)

add_executable(test ${SOURCE_FILES})

target_link_libraries(test ${GTEST_LIBRARIES} pthread)