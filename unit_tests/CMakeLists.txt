cmake_minimum_required(VERSION 3.17)
project(unit_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wall")

add_executable(unit_tests
    ../src/data/image.cpp
    ../src/data/pixel.cpp

    ../src/filters/filter_factory.cpp

    ../src/filters/all_filters.cpp

    ../src/io/binary_reader.cpp
    ../src/io/binary_writer.cpp
    ../src/io/bmp.cpp

    unit_tests.cpp
)

target_include_directories(unit_tests PUBLIC include)