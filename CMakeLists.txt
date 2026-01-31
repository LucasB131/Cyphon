cmake_minimum_required(VERSION 3.16)
project(Cyphon)

set(CMAKE_CXX_STANDARD 17)

add_executable(Cyphon
    src/main.cpp
)

target_include_directories(Cyphon PRIVATE external/raylib/include)
target_link_directories(Cyphon PRIVATE external/raylib/lib)

target_link_libraries(Cyphon raylib)