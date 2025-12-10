cmake_minimum_required(VERSION 3.25)
project(qjsx)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(qjs CONFIG REQUIRED)

add_library(qjsx STATIC
        include/qjsx/qjsx.h
        src/context.cpp
        src/runtime.cpp
        src/string.cpp
        src/value.cpp
)

target_include_directories(qjsx PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(qjsx PUBLIC qjs)