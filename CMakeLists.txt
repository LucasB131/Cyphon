cmake_minimum_required(VERSION 3.15)
project(Cyphon)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib CONFIG REQUIRED)

add_executable(Cyphon
    src/main.cpp
    src/Screens/SKLScreen.cpp
    src/Screens/StartMenu.cpp
    src/Screens/NewAvatar.cpp
    src/Screens/CreateAvatar.cpp
    src/Screens/Sandbox.cpp
    src/SaveFunctions/SaveSystem.cpp
    src/Colors/Colors.cpp
    src/UI/Helpers.cpp
    src/GameState.cpp   
    src/Gameplay/ThirdPersonController.cpp     
)

target_link_libraries(Cyphon PRIVATE raylib)