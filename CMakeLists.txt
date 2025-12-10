cmake_minimum_required(VERSION 3.25)
project(ejs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_subdirectory(qjsx)

add_executable(ejs
        src/app.cpp
        src/ejs.cpp
        src/plugin.cpp
        src/plugin_manager.cpp
)

target_link_libraries(ejs PRIVATE spdlog::spdlog yaml-cpp::yaml-cpp qjsx)
