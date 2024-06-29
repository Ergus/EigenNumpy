#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..
#
# Simple numpy like example with pybind11. 
# Copyright 2016 Jimmy Aguilar Mena <spacibba@yandex.com>
#
#..ooOO00OOoo....ooOO00OOoo....ooOO00OOoo....ooOO00OOoo..

cmake_minimum_required (VERSION 3.25)

project (EigenNumpy CXX)

find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)
find_package(Eigen3 REQUIRED)

# pybind11 method:
# Python method:
Python_add_library(EigenNumpy ${PROJECT_SOURCE_DIR}/src/EigenNumpy.cpp)
target_link_libraries(EigenNumpy PUBLIC pybind11::headers Eigen3::Eigen)
set_target_properties(EigenNumpy PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION ON
    CXX_VISIBILITY_PRESET ON
    VISIBILITY_INLINES_HIDDEN ON)

# Python executable
configure_file(${PROJECT_SOURCE_DIR}/main.py ${PROJECT_BINARY_DIR})

#enable_testing()
