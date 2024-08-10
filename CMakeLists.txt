project(CppUnitProj)
cmake_minimum_required(VERSION 3.22)

add_library(CppUnit INTERFACE)
target_include_directories(CppUnit INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(testapp testapp.cpp)
target_link_libraries(testapp CppUnit)