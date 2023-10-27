cmake_minimum_required(VERSION 3.5.0)
project(test_task)

set(CMAKE_CXX_STANDART 17)
set(SOURCES main.cpp
            task.cpp
            taskmanager.cpp)
set(HEADERS task.h
            taskmanager.h)

add_executable(test_task ${SOURCES} ${HEADERS})