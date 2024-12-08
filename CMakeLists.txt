cmake_minimum_required(VERSION 3.20)

project(system_errc)

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(system_errc INTERFACE)
target_include_directories(system_errc INTERFACE ${system_errc_SOURCE_DIR}/include)
