
cmake_minimum_required(VERSION 3.16)

project(Implementation LANGUAGES CXX)

add_library(Implementation
    src/Transforms.cpp
    src/KeySchedule.cpp
    src/Cipher.cpp
)

target_include_directories(Implementation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
