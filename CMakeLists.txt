cmake_minimum_required(VERSION 3.30)
project(SOprojekt1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(SOprojekt1 main.cpp
        GenerateGraph.cpp
        GenerateGraph.h
        FileHandler.cpp
        FileHandler.h)
