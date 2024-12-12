cmake_minimum_required(VERSION 3.13)  

project(inverted_index)               

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 20)            
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -ffast-math -fomit-frame-pointer")

set(SOURCE_FILES 
    src/main.cpp
    src/inverted_index.cpp
    src/web_crawler.cpp
    src/document_relevance.cpp
    src/term_document.cpp
    src/term.cpp
)

add_compile_options(-Wall -Wextra -Wpedantic -Ofast -march=native -ffast-math -fomit-frame-pointer)

add_executable(inverted-index ${SOURCE_FILES})

find_library(STEMMER_LIB NAMES stemmer)
if (STEMMER_LIB)
    target_link_libraries(inverted-index ${STEMMER_LIB})
else()
    message(FATAL_ERROR "libstemmer not found")
endif()

find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(inverted-index gumbo curl nlohmann_json::nlohmann_json)
include_directories(${Gumbo_INCLUDE_DIRS})

