cmake_minimum_required(VERSION 4.0)
project(p4_part1)

set(CMAKE_CXX_STANDARD 20)

add_executable( p4_part1

        main.cpp
        utils.cpp
        utils.hpp
        input_output/
        HuffmanTree.cpp
        HuffmanTree.cpp
        PriorityQueue.cpp
        PriorityQueue.hpp

)
