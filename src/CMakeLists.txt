include_directories(../inc)
set(SOURCES
        wnFilter.cpp
        Image.cpp
        FileHandlerTGA.cpp
        FileHandlerPPM.cpp
        FileHandlerPGM.cpp
        FileHandlerPBM.cpp
        ConsoleInterface.cpp
        FileHandlerFactory.cpp
        ../main.cpp)

add_executable(FiltrObrazu ${SOURCES})