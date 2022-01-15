cmake_minimum_required( VERSION 3.0)

# Project name
project( test )

# flags

# files

#Test with building from GLFW source (troubles with linking from glfw binary)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(/home/emil/code/cpp/glfw-3.3.6/ /home/emil/code/cpp/glfw-3.3.6/build/)

# include
include_directories( ./include ./src )

# target
add_executable( binary ./src/main.cpp ./src/glad.c ./src/Shader.cpp)

target_link_libraries( binary glfw)
target_link_libraries( binary -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
