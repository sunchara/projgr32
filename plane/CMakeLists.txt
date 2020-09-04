cmake_minimum_required(VERSION 2.8)
project(plane)
add_subdirectory(./vect2)

file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_library(plane ${SRC})
include_directories(plane ./vect2)
target_link_libraries(plane vect2)

