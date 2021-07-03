cmake_minimum_required(VERSION 3.0)
project(stockpile)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("../lib" "lib")

include_directories(
  "../"
  "../../thirdparty/args"
)

set(SRCS
	main.cpp
)

add_executable(stockpile "${SRCS}")

target_link_libraries(stockpile pile snappy)
