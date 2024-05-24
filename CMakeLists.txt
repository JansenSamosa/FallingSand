cmake_minimum_required(VERSION 3.27)
project(FallingSand)

set(CMAKE_CXX_STANDARD 17)

add_executable(FallingSand src/main.cpp
        src/glad.c
        src/Shader.cpp
        src/Shader.h
        src/Game.cpp
        src/Game.h
        src/WorldRenderer.cpp
        src/WorldRenderer.h
        src/ElementDefinitions.h
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

include_directories("/home/jansensamosa/Projects/learnOpenGL/Libraries/Include")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(FallingSand glfw OpenGL )
