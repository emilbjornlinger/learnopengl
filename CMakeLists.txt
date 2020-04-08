cmake_minimum_required( VERSION 3.0)

# Project name
project( test )

# flags

# files

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

# include
include_directories( ./include ./src  ${GLFW_INCLUDE_DIRS})

# target
add_executable( binary ./src/main.cpp ./src/glad.c ./src/Shader.cpp)

target_link_libraries( binary ${GLFW_STATIC_LIBRARIES})
target_link_libraries( binary -lGL -lX11 -lpthread -lXrandr -lXi -ldl)
