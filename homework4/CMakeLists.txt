cmake_minimum_required(VERSION 3.27)
project(homework4)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(homework4
        DNAseq.cpp
        DNAseq.h
        main.cpp)
