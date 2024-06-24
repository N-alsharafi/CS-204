cmake_minimum_required(VERSION 3.27)
project(homework3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(homework3
        EmployeeProject2DLL.cpp
        EmployeeProject2DLL.h
        main.cpp
        UndoStack.cpp
        UndoStack.h)
