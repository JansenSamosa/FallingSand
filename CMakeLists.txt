cmake_minimum_required(VERSION 3.27)
project(FallingSand)

set(CMAKE_CXX_STANDARD 17)

add_executable(FallingSand src/main.cpp
        src/Misc/glad.c
        src/Rendering/Shader.cpp
        src/Rendering/Shader.h
        src/World.cpp
        src/World.h
        src/Rendering/WorldRenderer.cpp
        src/Rendering/WorldRenderer.h
        src/ElementDefinitions.h
        src/Engine.cpp
        src/Engine.h
)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

include_directories("/home/jansensamosa/Projects/learnOpenGL/Libraries/Include")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(FallingSand glfw OpenGL )
