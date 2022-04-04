cmake_minimum_required(VERSION 3.20)
project(stratum_tcp)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

add_subdirectory(third_party/json)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(test_miner)
