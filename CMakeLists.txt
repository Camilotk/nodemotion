cmake_minimum_required(VERSION 3.26)
project(NodeMotion LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED COMPONENTS graphics window system)

add_library(NodeMotion
    src/render/SfmlRenderer.cpp
    src/export/FfmpegExporter.cpp
)
target_include_directories(NodeMotion PUBLIC include)
target_link_libraries(NodeMotion PUBLIC sfml-graphics sfml-window sfml-system)

add_executable(ball_demo examples/ball_demo.cpp)
target_link_libraries(ball_demo PRIVATE NodeMotion)
