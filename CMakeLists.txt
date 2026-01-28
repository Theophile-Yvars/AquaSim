cmake_minimum_required(VERSION 3.14)
project(AquaSim)

set(CMAKE_CXX_STANDARD 17)

# En début de CMakeLists.txt
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    message(STATUS "Building with coverage flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

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