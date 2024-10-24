cmake_minimum_required(VERSION 3.29)
project(Practica2 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/maryt/OneDrive/Documentos/SDL2-2.30.7/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(Practica2 main.c
        array.c array.h
        display.c display.h
        matrix.c matrix.h
        vector.c vector.h
        mesh.c mesh.h
        triangle.c triangle.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
