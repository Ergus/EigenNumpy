cmake_minimum_required (VERSION 3.4)

project (cnumpy CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/pybind11)

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/pybind11/*.h)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

# Build library ################################
find_package(PythonLibs)
add_library(cnumpy SHARED ${PROJECT_SOURCE_DIR}/src/cnumpy.cc)
target_include_directories(cnumpy PUBLIC ${PYTHON_INCLUDE_DIRS})
target_link_libraries(cnumpy ${PYTHON_LIBRARIES})

# C executable

add_executable(main.x main.cc)
target_link_libraries(main.x cnumpy)

# Python executable

configure_file(${PROJECT_SOURCE_DIR}/main.py ${PROJECT_BINARY_DIR})
