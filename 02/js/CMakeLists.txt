cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(aoc2_part1 2_part1.cpp)
set_property(TARGET aoc2_part1 PROPERTY CXX_STANDARD 23)
target_include_directories(aoc2_part1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_executable(aoc2_part2 2_part2.cpp)
set_property(TARGET aoc2_part2 PROPERTY CXX_STANDARD 23)
target_include_directories(aoc2_part2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)