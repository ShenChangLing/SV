cmake_minimum_required(VERSION 3.6)
project(SV)

add_subdirectory(RenderSystem)

set(CMAKE_CXX_STANDARD 17)

link_libraries(RenderSystem)

add_executable(SV main.cpp)