cmake_minimum_required(VERSION 3.22)
project(OS_Beta)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OS_Beta
        GlobalVariable.h
        main.cpp
        memory.h memory.cpp gcm.h gcm.cpp directory.h directory.cpp help.cpp help.h ram.cpp ram.h inode.cpp inode.h fileSys.cpp fileSys.h user.cpp user.h os.cpp os.h)
