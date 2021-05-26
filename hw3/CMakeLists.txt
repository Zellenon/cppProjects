cmake_minimum_required(VERSION 3.14)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw3 main.cpp Rectangle.cpp Rectangle.h)
configure_file(data.txt data.txt COPYONLY)