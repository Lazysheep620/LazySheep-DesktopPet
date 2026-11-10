cmake_minimum_required(VERSION 3.21)
project(LazySheep-DesktopPet VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(LazySheep-DesktopPet WIN32
    src/main.cpp
    src/PetWindow.cpp
    src/PetWindow.h
    src/Sheep.cpp
    src/Sheep.h
)

set_target_properties(LazySheep-DesktopPet PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_link_libraries(LazySheep-DesktopPet PRIVATE Qt6::Widgets)

qt_add_resources(LazySheep-DesktopPet sheep_sprites
    PREFIX "/sprites"
    BASE "assets/sprites"
    FILES
        assets/sprites/idle_0.png
        assets/sprites/idle_1.png
        assets/sprites/idle_2.png
        assets/sprites/idle_3.png
        assets/sprites/walk_0.png
        assets/sprites/walk_1.png
        assets/sprites/walk_2.png
        assets/sprites/walk_3.png
        assets/sprites/sleep_0.png
        assets/sprites/sleep_1.png
        assets/sprites/sleep_2.png
        assets/sprites/sleep_3.png
        assets/sprites/eat_0.png
        assets/sprites/eat_1.png
        assets/sprites/eat_2.png
        assets/sprites/eat_3.png
        assets/sprites/happy_0.png
        assets/sprites/happy_1.png
        assets/sprites/happy_2.png
        assets/sprites/happy_3.png
)
