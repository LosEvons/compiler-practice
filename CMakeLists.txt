cmake_minimum_required(VERSION 3.25.1)

project(test-compiler)

include_directories(src)
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})