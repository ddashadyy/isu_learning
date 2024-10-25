cmake_minimum_required(VERSION 3.13)  
project(inverted_index)               
set(CMAKE_CXX_STANDARD 20)            
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

set(SOURCE_FILES main.cpp inverted_index.cpp)

add_executable(main ${SOURCE_FILES})