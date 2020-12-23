cmake_minimum_required(VERSION 3.5)

# Fetch doctest.
include(FetchContent)
FetchContent_Declare(doctest GIT_REPOSITORY https://github.com/onqtam/doctest)
FetchContent_MakeAvailable(doctest)

project(command_pattern LANGUAGES CXX)

find_package(fmt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(command_pattern main.cpp)
target_link_libraries(command_pattern PRIVATE doctest fmt::fmt)
