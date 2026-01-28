cmake_minimum_required(VERSION 3.14)
project(AquaSim)

set(CMAKE_CXX_STANDARD 17)

# Code principal
add_library(aquasim
    src/algae.cpp
    src/carnivoreFish.cpp
    src/cell.cpp
    src/herbivoreFish.cpp
    src/ocean.cpp
)

target_include_directories(aquasim PUBLIC include)

# Tests
add_subdirectory(tests)

# Exécutable principal de l’application
add_executable(AquaSimMain src/main.cpp)
target_link_libraries(AquaSimMain aquasim)