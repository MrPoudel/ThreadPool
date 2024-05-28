cmake_minimum_required(VERSION 3.10)
project(example_project)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

add_executable(app main.cpp)

target_link_libraries(app PUBLIC Logger ThreadPool)
