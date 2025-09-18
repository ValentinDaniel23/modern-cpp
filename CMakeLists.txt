cmake_minimum_required(VERSION 3.18)
project(modercpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SOURCES
        ch1.cpp
        item10.cpp
        item11.cpp
        item12.cpp
        item13.cpp
        item16.cpp
        item17.cpp
        item18.cpp
        item19.cpp
        item20.cpp
        item21.cpp
        item22.cpp
        item23.cpp
        item24.cpp
        item25.cpp
        item26.cpp
)

foreach(src ${SOURCES})
    get_filename_component(exename ${src} NAME_WE)
    add_executable(${exename} ${src})
endforeach()
