cmake_minimum_required(VERSION 3.22.1)

project(test-compiler)

include_directories(src)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})