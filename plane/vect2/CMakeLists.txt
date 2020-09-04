cmake_minimum_required(VERSION 2.8)
project( vect2)

file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_library(vect2 ${SRC})

