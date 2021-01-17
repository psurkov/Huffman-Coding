cmake_minimum_required(VERSION 3.12)
project(Huffman_Coding)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Werror")

include_directories(include)
include_directories(src)

add_executable(huffman
        include/CLI.h
        include/HuffmanArchiver.h
        include/HuffmanIO.h
        include/Huffman.h

        src/main.cpp
        src/CLI.cpp
        src/HuffmanArchiver.cpp
        src/HuffmanIO.cpp
        src/Huffman.cpp
        )

add_executable(test_huffman
        include/CLI.h
        include/HuffmanArchiver.h
        include/HuffmanIO.h
        include/Huffman.h

        src/CLI.cpp
        src/HuffmanArchiver.cpp
        src/HuffmanIO.cpp
        src/Huffman.cpp

        test/TestMain.cpp
        test/TestCLI.cpp
        test/TestHuffmanArchiver.cpp
        test/TestHuffman.cpp
        test/TestHuffmanIO.cpp
        test/doctest.h
        )