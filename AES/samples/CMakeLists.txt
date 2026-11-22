cmake_minimum_required(VERSION 3.16)

project(samples LANGUAGES CXX)

add_executable(samples
    main.cpp
)

target_link_libraries(samples  Implementation)

install(TARGETS samples DESTINATION bin)