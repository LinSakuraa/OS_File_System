cmake_minimum_required(VERSION 3.22)
project(OS_Beta)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OS_Beta
        GlobalVariable.h
        index.h index.cpp blocksControl.h blocksControl.cpp fileDirectory.h fileDirectory.cpp ayxiliaryMod.cpp ayxiliaryMod.h list.cpp list.h nodeList.cpp nodeList.h fileSystem.cpp fileSystem.h userControl.cpp userControl.h interface.cpp interface.h
        main.cpp)
