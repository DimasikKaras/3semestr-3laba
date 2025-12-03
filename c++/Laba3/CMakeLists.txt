cmake_minimum_required(VERSION 4.0)
project(Laba3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Laba3
        avlTree.h
        chainHashTable.cpp
        doublyList.cpp
        doublyList.h
        dyArr.cpp
        dyArr.h
        HashTable.h
        main.cpp
        OpenAdressingHashTable.cpp
        queue.cpp
        queue.h
        simplyList.cpp
        simplyList.h
        stack.cpp
        stack.h
        avlTree.cpp
        executingCommand.cpp
        executingCommands.h
        menu.cpp
        menu.h)
