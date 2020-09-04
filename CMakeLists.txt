cmake_minimum_required(VERSION 2.8)
project( graphen32 )

file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_subdirectory(./plane)



add_executable(gr32 ${SRC})

target_include_directories(gr32 PUBLIC ./plane)
target_include_directories(gr32 PUBLIC ./plane/vect2)


target_link_libraries(gr32 plane)

target_link_libraries( gr32 gdi32.dll user32.dll kernel32.dll comctl32.dll )
