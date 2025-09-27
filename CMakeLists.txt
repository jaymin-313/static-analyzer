cmake_minimum_required(VERSION 3.14)
project(StaticAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
add_executable(static-analyzer
    src/main.cpp
)

target_include_directories(static-analyzer
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)
