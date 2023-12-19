cmake_minimum_required(VERSION 3.24)
project(FiltrObrazu)

set(CMAKE_CXX_STANDARD 20)

add_executable(FiltrObrazu src/main.cpp src/ImageGray.h src/ImageGray.cpp src/ImageRGB.cpp src/ImageRGB.h)
