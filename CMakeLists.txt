cmake_minimum_required(VERSION 3.2)
project(signald)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(signald ${SOURCE_FILES} Signald.cpp Signald.h)