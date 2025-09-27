cmake_minimum_required(VERSION 3.14)
project(StaticAnalyzer VERSION 0.1 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(static-analyzer
    src/main.cpp
    src/analyzer/Analyzer.cpp
    src/checks/TodoCheck.cpp
    src/checks/NamingConventionCheck.cpp
    src/checks/MagicNumberCheck.cpp
    src/checks/LongFunctionCheck.cpp
    src/checks/IncludeGuardCheck.cpp
    src/checks/FunctionParameterCountCheck.cpp
    src/checks/ConstCorrectnessCheck.cpp
)


target_include_directories(static-analyzer
PRIVATE ${CMAKE_SOURCE_DIR}/include
)


# Optional: enable warnings
if(MSVC)
target_compile_options(static-analyzer PRIVATE /W4 /permissive-)
else()
target_compile_options(static-analyzer PRIVATE -Wall -Wextra -Wpedantic)
endif()