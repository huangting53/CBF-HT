cmake_minimum_required(VERSION 3.13)
project(CBF-HT)

set(CMAKE_CXX_STANDARD 11)

add_executable(CBF-HT main.cpp PrefixGenerator.cpp hashFuncs.cpp BloomFilter.cpp hashtable.cpp BinarySearch.cpp CBF-HT.cpp)