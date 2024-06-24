cmake_minimum_required(VERSION 3.27)
project(homework5)

set(CMAKE_CXX_STANDARD 17)

add_executable(homework5 main.cpp
        Board.cpp
        Board.h
        Player.cpp
        Player.h)
