cmake_minimum_required(VERSION 3.31)

project(system_errc VERSION 0.6.1)

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.5/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:SanyaNya/hpp@1.6.1")

add_library(system_errc INTERFACE)
target_link_libraries(system_errc INTERFACE hpp)
target_include_directories(system_errc INTERFACE ${system_errc_SOURCE_DIR}/include)
