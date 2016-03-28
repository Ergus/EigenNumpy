cmake_minimum_required (VERSION 3.4)

project (Jimmy CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
set_source_files_properties(${PROJECT_SOURCE_DIR}/src/*.cc PROPERTIES COMPILE_FLAGS "-fPIC -std=c++11")

# Build library ################################

add_library(cnumpy SHARED ${PROJECT_SOURCE_DIR}/src/cnumpy.cc)

add_executable(main.x main.cc)
target_link_libraries(main.x cnumpy)
