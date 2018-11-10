cmake_minimum_required(VERSION 3.0)
project(stockpile)

set(SNAPPY_BUILD_TESTS OFF CACHE BOOL "")

add_subdirectory("../lib" "lib")
add_subdirectory("../../thirdparty/snappy" "thirdparty/snappy/build")
add_subdirectory("../boostlib" "boostlib")

set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_CXX_FLAGS "-std=c++14")

set(BOOST_DIR $ENV{BOOST_DIR})

include_directories(
  "../"
  "../../thirdparty/args"
  ${BOOST_DIR}
)

set(SRCS
	main.cpp
)

add_executable(stockpile "${SRCS}")

target_link_libraries(stockpile pile snappy boostlib)
