cmake_minimum_required(VERSION 2.6)
project(stockpiletests)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")

add_subdirectory("../lib" "lib")
add_subdirectory("../../thirdparty/snappy" "thirdparty/snappy/build")
add_subdirectory("../../thirdparty/googletest/googletest" "thirdparty/googletest")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++1z")

include_directories(
	"../"
	"../include"
	"../../thirdparty/googletest/googletest/include"
)

set(SRCS
	tests.cpp
)

add_executable(stockpiletests "${SRCS}")

target_link_libraries(stockpiletests pile snappy gtest gtest_main)

file(COPY "pile.json" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test.txt" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test1.txt" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "test2.txt" DESTINATION "${CMAKE_BINARY_DIR}")
