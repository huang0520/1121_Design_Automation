cmake_minimum_required(VERSION 3.0.0)
project(project3 VERSION 0.1.0 LANGUAGES C CXX)

message(${PROJECT_SOURCE_DIR})

add_executable(project3 main.cpp)

target_compile_features(project3 PRIVATE cxx_std_17)
target_include_directories(project3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
