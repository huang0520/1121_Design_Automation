cmake_minimum_required(VERSION 3.25.0)
project(project1 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(project1 main.cpp)

target_compile_features(project1 PRIVATE cxx_std_17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CPack)
