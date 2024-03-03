cmake_minimum_required(VERSION 3.24)
project(FiltrObrazu)

set(CMAKE_CXX_STANDARD 20)

set(DATA_DIR "${PROJECT_SOURCE_DIR}/data")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/inc")
set(EFFECTS_DIR "${DATA_DIR}/Effects")
include_directories(${INCLUDE_DIR})

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME}
        main.cpp
        ${SOURCES})

# Obrazy czarno-białe
add_custom_target(Lena10 ./${PROJECT_NAME} -f ${DATA_DIR}/lena.ascii.pgm ${DATA_DIR}/filtr10.txt
        ${EFFECTS_DIR}/lena10_filtered.pgm 8)
add_custom_target(LenaGauss ./${PROJECT_NAME} -f ${DATA_DIR}/lena.ascii.pgm ${DATA_DIR}/filtrgauss.txt
        ${EFFECTS_DIR}/lenagauss_filtered.pgm 8)
add_custom_target(Lena1024 ./${PROJECT_NAME} -rs ${DATA_DIR}/lena.ascii.pgm 1024 1024 ${EFFECTS_DIR}/lena1024.pgm)

add_custom_target(Buffalo10 ./${PROJECT_NAME} -f ${DATA_DIR}/buffalo.pgm ${DATA_DIR}/filtr10.txt
        ${EFFECTS_DIR}/buffalo10_filtered.pgm 8)
add_custom_target(BuffaloGauss ./${PROJECT_NAME} -f ${DATA_DIR}/buffalo.pgm ${DATA_DIR}/filtrgauss.txt
        ${EFFECTS_DIR}/buffalogauss_filtered.pgm 8)
add_custom_target(Columns10 ./${PROJECT_NAME} -f ${DATA_DIR}/columns.pgm ${DATA_DIR}/filtr10.txt
        ${EFFECTS_DIR}/columns10_filtered.pgm 8)

# Obrazy kolorowe
add_custom_target(Blackbuck10 ./${PROJECT_NAME} -f ${DATA_DIR}/blackbuck.ascii.ppm ${DATA_DIR}/filtr10.txt
        ${EFFECTS_DIR}/blackbuck10_filtered.ppm 8)
add_custom_target(Underwater10 ./${PROJECT_NAME} -f ${DATA_DIR}/underwater_bmx.ppm ${DATA_DIR}/filtr10.txt
        ${EFFECTS_DIR}/underwaterbmx10_filtered.ppm 8)
