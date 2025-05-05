project(FiltrObrazu)

cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)

#Dir for executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#Dir for static and dynamic libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#Dir for object files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include_directories(./inc)

#Subdirectories
add_subdirectory(src)
