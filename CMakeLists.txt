cmake_minimum_required(VERSION 3.15)
project(yadro2)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        Simulation.cpp
        Simulation.h
        main.cpp
)

list(TRANSFORM SOURCES PREPEND src/)

# Console app
add_executable(tape_sort ${SOURCES})