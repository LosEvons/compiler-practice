cmake_minimum_required(VERSION 3.22.1)

project(test-compiler)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck)
if (ENABLE_CPPCHECK)
	if (CMAKE_CXX_CPPCHECK)
		list(
			APPEND CMAKE_CXX_CPPCHECK
			--quiet
			--enable=all
			--inconclusive
			--inline-suppr
			--suppress=missingIncludeSystem
			--template="{file}:{line}: {severity}: {message}\\n{code}"
			--template-location="{file}:{line}: note: {info}\\n{code}"
			--output-file=../cppcheck-report.txt
			${SOURCES}
		)
	endif()
endif()

add_executable(${PROJECT_NAME} ${SOURCES})