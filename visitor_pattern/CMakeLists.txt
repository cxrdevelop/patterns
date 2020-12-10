cmake_minimum_required(VERSION 3.5)

project(visitor_pattern LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(visitor_pattern
    main.cpp
    polymorphic_visitor.cpp
    polymorphic_visitor.h
    visitor.cpp
    visitor.h
    make_visitor.h
    make_visitor_std11.h)
