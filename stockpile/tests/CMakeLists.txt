cmake_minimum_required(VERSION 3.0)
project(stockpiletests)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../lib" "lib")
add_subdirectory("../../thirdparty/googletest/googletest" "thirdparty/googletest")

include_directories(
	"../"
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
file(COPY "moredata/test3.txt" DESTINATION "${CMAKE_BINARY_DIR}/moredata")
