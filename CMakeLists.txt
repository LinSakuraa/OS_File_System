cmake_minimum_required(VERSION 3.17.5)
project(OS_BETA)
set(CMAKE_CXX_STANDARD 14)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(
        OS_BETA
        GlobalVariable.h
        main.cpp
        blocksControl.h blocksControl.cpp
        errorNotify.h errorNotify.cpp
        fileSystem.h fileSystem.cpp
        fileDirectory.h fileDirectory.cpp
        index.h index.cpp interface.h interface.cpp
        list.h list.cpp nodeList.h nodeList.cpp
        supportControl.h supportControl.cpp
        userControl.h userControl.cpp
)