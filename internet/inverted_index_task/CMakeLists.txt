cmake_minimum_required(VERSION 3.13)  

project(inverted_index)               

set(CMAKE_CXX_STANDARD 20)            
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(SOURCE_FILES src/main.cpp src/inverted_index.cpp)

add_executable(inverted-index ${SOURCE_FILES})

target_link_libraries(inverted-index pugixml gumbo curl)
include_directories(${Gumbo_INCLUDE_DIRS})
