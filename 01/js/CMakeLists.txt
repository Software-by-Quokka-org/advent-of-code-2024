cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(aoc1 1.cpp)
set_property(TARGET aoc1 PROPERTY CXX_STANDARD 23)
target_include_directories(aoc1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
