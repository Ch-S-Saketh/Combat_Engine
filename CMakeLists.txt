cmake_minimum_required(VERSION 3.16)

project(RLEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(
    RLEngine
    ${SOURCES}
)

target_include_directories(
    RLEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_directories(
    RLEngine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_link_libraries(
    RLEngine
    PRIVATE
    raylib
    GL
    m
    pthread
    dl
    rt
    X11
)