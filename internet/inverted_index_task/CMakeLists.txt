cmake_minimum_required(VERSION 3.13)  

project(inverted_index)               

set(CMAKE_CXX_STANDARD 20)            
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(SOURCE_FILES src/main.cpp src/inverted_index.cpp src/web_crawler.cpp)
set(HEADER_FILES src/inverted_index.hpp src/serialization.hpp src/web_crawler)

add_compile_options(-Wall -Wextra -Wpedantic -Ofast)

add_executable(inverted-index ${SOURCE_FILES})

find_library(STEMMER_LIB NAMES stemmer)
if (STEMMER_LIB)
    target_link_libraries(inverted-index ${STEMMER_LIB})
else()
    message(FATAL_ERROR "libstemmer not found")
endif()

target_link_libraries(inverted-index gumbo curl)
include_directories(${Gumbo_INCLUDE_DIRS})

