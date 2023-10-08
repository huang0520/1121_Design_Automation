cmake_minimum_required(VERSION 3.25.0)
project(project1 VERSION 0.1.0 LANGUAGES C CXX)

add_executable(project1 main.cpp)

target_compile_features(project1 PRIVATE cxx_std_17)

# Where the executable is located
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)
