cmake_minimum_required(VERSION 3.28)
project(Cyphon LANGUAGES CXX)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure MSVC uses consistent runtime (matches raylib default: /MD)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Add your source files
add_executable(Cyphon
    src/main.cpp
    src/Global/GameScreen.cpp
    src/Screens/MainMenu.cpp
)

# Find raylib (assumes installed or provided by vcpkg)
find_package(raylib CONFIG REQUIRED)

# Link everything required on Windows
target_link_libraries(Cyphon
    PRIVATE
        raylib
        winmm
)

# Improve IDE organization
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source" FILES src/main.cpp)