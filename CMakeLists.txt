cmake_minimum_required(VERSION 3.24)
project(FiltrObrazu)

set(CMAKE_CXX_STANDARD 20)

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/inc")
include_directories(${INCLUDE_DIR})

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME}
        main.cpp
        ${SOURCES})

add_custom_target(test1 ./${PROJECT_NAME} -f ${DATA_DIR}/lena.ascii.pgm filtr.txt ${DATA_DIR}/lena2.pgm 8)

