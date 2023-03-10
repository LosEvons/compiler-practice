cmake_minimum_required(VERSION 3.22.1)

project(test-compiler)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (CMAKE_CXX_CPPCHECK)
	list(
		APPEND CMAKE_CXX_CPPCHECK
		--quiet
		--enable=all
		--inconclusive
		--inline-suppr
		--suppress=missingIncludeSystem
		${SOURCES}
	)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})