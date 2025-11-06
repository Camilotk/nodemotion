cmake_minimum_required(VERSION 3.26)
project(NodeMotion LANGUAGES CXX VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED COMPONENTS graphics window system)

add_library(NodeMotion
    src/render/SfmlRenderer.cpp
    src/export/FfmpegExporter.cpp
    src/render/Node.cpp
    src/NodeMotion.cpp
    src/render/Node.cpp
)
target_include_directories(NodeMotion PUBLIC include)
target_link_libraries(NodeMotion PUBLIC sfml-graphics sfml-window sfml-system)

add_executable(ball_demo examples/ball_demo.cpp)
target_link_libraries(ball_demo PRIVATE NodeMotion)

add_executable(node_demo examples/node_demo.cpp)
target_link_libraries(node_demo PRIVATE NodeMotion)

add_executable(int_demo examples/int_demo.cpp)
target_link_libraries(int_demo PRIVATE NodeMotion)

add_executable(list_ops_demo examples/list_ops_demo.cpp)
target_link_libraries(list_ops_demo PRIVATE NodeMotion)
