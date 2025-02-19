cmake_minimum_required(VERSION 3.31)

project(system_errc 0.5.3)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#Require C++23 without compiler extensions
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(Fetch)

fetch_project(hpp https://github.com/SanyaNya/hpp v1.5.0)

add_library(system_errc INTERFACE)
target_link_libraries(system_errc INTERFACE hpp)
target_include_directories(system_errc INTERFACE ${system_errc_SOURCE_DIR}/include)
